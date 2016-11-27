#include "filter_rule.hpp"

#include "pattern/pattern.hpp"
#include "option/match_case_option.hpp"
#include "option/type_option.hpp"
#include "option/restriction_option.hpp"

#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/range/adaptor/filtered.hpp>

namespace adblock {

FilterRule::
FilterRule(const std::shared_ptr<Pattern> &pattern,
           const boost::optional<Options> &options)
    : m_pattern { pattern }
    , m_options { options }
{
    validate();
}

bool FilterRule::
match(const Uri &uri, const Context* const context) const
{
    if (!m_pattern->match(uri, hasOption<MatchCaseOption>())) return false;

    if (context && m_options) {
        return matchTypeOptions(uri, *context)
            && matchRestrictionOptions(uri, *context);
    }

    return true;
}

const Pattern &FilterRule::
pattern() const
{
    assert(m_pattern);
    return *m_pattern;
}

FilterRule::OptionsRange FilterRule::
options() const
{
    if (m_options) {
        return *m_options;
    }
    else {
        return {};
    }
}

bool FilterRule::
matchTypeOptions(Uri const& uri, Context const& context) const
{
    assert(m_options);

    namespace ba = boost::adaptors;

    static auto const& isTypeOption =
        [](std::shared_ptr<Option> const& option) {
            return !!std::dynamic_pointer_cast<TypeOption>(option);
        };

    auto const& typeOptions = *m_options | ba::filtered(isTypeOption);

    return typeOptions.empty() ||
        boost::algorithm::any_of(typeOptions,
            [&](std::shared_ptr<Option> const& option) {
                return option->match(uri, context);
            }
        );
}

bool FilterRule::
matchRestrictionOptions(Uri const& uri, Context const& context) const
{
    assert(m_options);

    namespace ba = boost::adaptors;

    static auto const& isRestrictionOption =
        [](std::shared_ptr<Option> const& option) {
            return !!std::dynamic_pointer_cast<RestrictionOption>(option);
        };

    // all_of return ture if range is empty.
    return boost::algorithm::all_of(
        *m_options | ba::filtered(isRestrictionOption),
        [&](std::shared_ptr<Option> const& option) {
            return option->match(uri, context);
        }
    );
}

void FilterRule::
print(std::ostream &os) const
{
    os << "Pattern: " << *m_pattern << "\n";
    if (m_options) {
        const auto &options = m_options.get();
        os << "Option: ";
        for (const auto option: options) {
            os << *option << " ";
        }
    }
}

} // namespace adblock
