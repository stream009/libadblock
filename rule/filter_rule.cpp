#include "filter_rule.hpp"

#include "option.hpp"
#include "pattern/pattern.hpp"

#include <boost/algorithm/cxx11/all_of.hpp>

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
match(const Uri &uri, const Context &context) const
{
    if (!m_pattern->match(uri)) return false;
    if (!m_options) return true;

    return boost::algorithm::all_of(*m_options,
        [&uri, &context](const std::shared_ptr<Option> &option) {
            return option->match(uri, context);
        }
    );
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
