#include "basic_element_hide_rule.hpp"

namespace adblock {

BasicElementHideRule::
BasicElementHideRule(const StringRange &selector,
                const boost::optional<std::vector<StringRange>> &domains)
    : Base { selector, domains }
{}

std::ostream &
operator<<(std::ostream &os, const BasicElementHideRule &rule)
{
    os << "[BasicElementHideRule]\n";
    return operator<<(os, static_cast<const ElementHideRule&>(rule));

}

} // namespace adblock
