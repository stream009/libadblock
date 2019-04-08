#include "basic_element_hide_rule.hpp"

namespace adblock {

BasicElementHideRule::
BasicElementHideRule(StringRange const& selector,
                     std::vector<StringRange> const& domains)
    : Base { selector, domains }
{}

std::ostream &
operator<<(std::ostream &os, BasicElementHideRule const& rule)
{
    os << "[BasicElementHideRule]\n";
    return operator<<(os, static_cast<ElementHideRule const&>(rule));

}

} // namespace adblock
