#include "filter_rule.hpp"

#include "option.hpp"

namespace adblock {

FilterRule::
FilterRule(const std::shared_ptr<Pattern> &pattern,
           const boost::optional<
                        std::vector<std::shared_ptr<Option>>> &options)
    : m_pattern { pattern }
    , m_options { options }
{}

bool FilterRule::
match(const Uri &url, const Context&) const
{
    return m_pattern->match(url);
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
