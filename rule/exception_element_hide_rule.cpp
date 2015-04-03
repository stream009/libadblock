#include "exception_element_hide_rule.hpp"

ExceptionElementHideRule::
ExceptionElementHideRule(const String &selector,
                const boost::optional<std::vector<Domain>> &domains)
    : Base { selector, domains }
{}

std::ostream &
operator<<(std::ostream &os, const ExceptionElementHideRule &rule)
{
    os << "[BasicElementHideRule]\n";
    return operator<<(os, static_cast<const ElementHideRule&>(rule));

}
