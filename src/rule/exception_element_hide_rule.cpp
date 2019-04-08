#include "exception_element_hide_rule.hpp"

namespace adblock {

ExceptionElementHideRule::
ExceptionElementHideRule(StringRange const& selector,
                         std::vector<StringRange> const& domains)
    : Base { selector, domains }
{}

std::ostream &
operator<<(std::ostream &os, ExceptionElementHideRule const& rule)
{
    os << "[BasicElementHideRule]\n";
    return operator<<(os, static_cast<const ElementHideRule&>(rule));

}

} // namespace adblock
