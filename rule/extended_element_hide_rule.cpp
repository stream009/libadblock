#include "extended_element_hide_rule.hpp"

namespace adblock {

ExtendedElementHideRule::
ExtendedElementHideRule(StringRange const& selector,
                boost::optional<std::vector<StringRange>> const& domains)
    : Base { selector, domains }
{}

std::ostream&
operator<<(std::ostream &os, ExtendedElementHideRule const& rule)
{
    os << "[ExtendedElementHideRule]\n";
    return operator<<(os, static_cast<ElementHideRule const&>(rule));

}

} // namespace adblock
