#ifndef BASIC_ELEMENT_HIDE_RULE_HPP
#define BASIC_ELEMENT_HIDE_RULE_HPP

#include "element_hide_rule.hpp"
#include "type.hpp"

#include <vector>

namespace adblock {

class BasicElementHideRule : public ElementHideRule
{
    using Base = ElementHideRule;
public:
    BasicElementHideRule(StringRange const& selector,
                         std::vector<StringRange> const& domains);
};

std::ostream &operator<<(std::ostream&, BasicElementHideRule const&);

} // namespace adblock

#endif // BASIC_ELEMENT_HIDE_RULE_HPP
