#include "basic_element_hide_rule.hpp"

BasicElementHideRule::
BasicElementHideRule(const String &selector,
                const boost::optional<std::vector<Domain>> &domains)
    : Base { selector, domains }
{}

std::ostream &
operator<<(std::ostream &os, const BasicElementHideRule &rule)
{
    os << "[BasicElementHideRule]\n";
    return operator<<(os, static_cast<const ElementHideRule&>(rule));

}
