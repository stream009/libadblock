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
FilterRule(std::unique_ptr<Pattern> pattern,
           Options options)
    : m_pattern { std::move(pattern) }
{
    for (auto& opt: options) {
        if (dynamic_cast<TypeOption*>(opt.get())) {
            m_typeOptions.push_back(std::move(opt));
        }
        else if (dynamic_cast<RestrictionOption*>(opt.get())) {
            auto const& option = *opt;
            if (typeid(option) == typeid(DomainOption)) {
                m_domainSpecific = true;
            }

            m_restrictionOptions.push_back(std::move(opt));
        }
        else if (dynamic_cast<WhiteListOption*>(opt.get())) {
            m_whiteListOptions.push_back(std::move(opt));
        }
        else {
            auto const& option = *opt;
            if (typeid(option) == typeid(MatchCaseOption)) {
                m_caseSensitive = true;
            }

            m_otherOptions.push_back(std::move(opt));
        }
    }

    validate();
}

FilterRule::~FilterRule() = default;

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

Pattern const& FilterRule::
pattern() const
{
    return *m_pattern;
}

FilterRule::OptionRefs FilterRule::
options() const
{
    OptionRefs result;

    for (auto const& o: m_typeOptions) {
        result.push_back(o.get());
    }
    for (auto const& o: m_restrictionOptions) {
        result.push_back(o.get());
    }
    for (auto const& o: m_whiteListOptions) {
        result.push_back(o.get());
    }
    for (auto const& o: m_otherOptions) {
        result.push_back(o.get());
    }

    return result;
}

bool FilterRule::
matchWhiteListOptions(Uri const& uri, Context const& context) const
{
    namespace ba = boost::algorithm;

    auto const rv =  ba::any_of(m_whiteListOptions,
            [&](auto const& option) {
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
            [&](auto const& option) {
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
        [&](auto const& option) {
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
        for (auto const& option: options) {
            os << option << " ";
        }
    }
}

} // namespace adblock
