#include "exception_element_hide_rule.hpp"

namespace adblock {

ExceptionElementHideRule::
ExceptionElementHideRule(const StringRange &selector,
                const boost::optional<std::vector<StringRange>> &domains)
    : Base { selector, domains }
{}

std::ostream &
operator<<(std::ostream &os, const ExceptionElementHideRule &rule)
{
    os << "[BasicElementHideRule]\n";
    return operator<<(os, static_cast<const ElementHideRule&>(rule));

}

} // namespace adblock
