#include "filter_rule.hpp"

#include "pattern/pattern.hpp"
#include "option/domain_option.hpp"
#include "option/match_case_option.hpp"
#include "option/restriction_option.hpp"
#include "option/type_option.hpp"
#include "option/white_list_option.hpp"
#include "white_list_query_context.hpp"

#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/range/adaptor/filtered.hpp>

namespace adblock {

FilterRule::
FilterRule(std::shared_ptr<Pattern> const& pattern,
           Options const& options)
    : m_pattern { pattern }
{
    for (auto const& opt: options) {
        if (std::dynamic_pointer_cast<TypeOption>(opt)) {
            m_typeOptions.push_back(opt);
        }
        else if (std::dynamic_pointer_cast<RestrictionOption>(opt)) {
            auto const& option = *opt;
            if (typeid(option) == typeid(DomainOption)) {
                m_domainSpecific = true;
            }

            m_restrictionOptions.push_back(opt);
        }
        else if (std::dynamic_pointer_cast<WhiteListOption>(opt)) {
            m_whiteListOptions.push_back(opt);
        }
        else {
            auto const& option = *opt;
            if (typeid(option) == typeid(MatchCaseOption)) {
                m_caseSensitive = true;
            }

            m_otherOptions.push_back(opt);
        }
    }

    validate();
}

bool FilterRule::
match(Uri const& uri, Context const& context,
                              bool const specificOnly/*= false*/) const
{
    if (specificOnly && !m_domainSpecific) return false;

    if (!m_pattern->match(uri, m_caseSensitive)) return false;

    if (dynamic_cast<WhiteListQueryContext const*>(&context)) {
        return matchWhiteListOptions(uri, context)
            && matchRestrictionOptions(uri, context);
    }
    else {
        return matchTypeOptions(uri, context)
            && matchRestrictionOptions(uri, context);
    }

    return true;
}

const Pattern &FilterRule::
pattern() const
{
    assert(m_pattern);
    return *m_pattern;
}

FilterRule::Options FilterRule::
options() const
{
    Options result;

    auto push_back = [&](Options const options) {
        result.insert(result.end(), options.begin(), options.end());
    };

    push_back(m_typeOptions);
    push_back(m_restrictionOptions);
    push_back(m_whiteListOptions);
    push_back(m_otherOptions);

    return result;
}

bool FilterRule::
matchWhiteListOptions(Uri const& uri, Context const& context) const
{
    namespace ba = boost::algorithm;

    auto const rv =  ba::any_of(m_whiteListOptions,
            [&](std::shared_ptr<Option> const& option) {
                return option->match(uri, context);
            }
        );
    return rv;
}

bool FilterRule::
matchTypeOptions(Uri const& uri, Context const& context) const
{
    namespace ba = boost::algorithm;

    // no type options means match everything
    if (m_typeOptions.empty() && m_whiteListOptions.empty()) {
        return true;
    }

    auto const rv = ba::any_of(m_typeOptions,
            [&](std::shared_ptr<Option> const& option) {
                return option->match(uri, context);
            }
    );

    return rv;
}

bool FilterRule::
matchRestrictionOptions(Uri const& uri, Context const& context) const
{
    namespace ba = boost::algorithm;

    return ba::all_of(m_restrictionOptions,
        [&](std::shared_ptr<Option> const& option) {
            return option->match(uri, context);
        }
    );
}

void FilterRule::
print(std::ostream &os) const
{
    os << "Pattern: " << *m_pattern << "\n";
    auto const& options = this->options();
    if (!options.empty()) {
        os << "Option: ";
        for (const auto option: options) {
            os << *option << " ";
        }
    }
}

} // namespace adblock
