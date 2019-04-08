#ifndef EXCEPTION_ELEMENT_HIDE_RULE_HPP
#define EXCEPTION_ELEMENT_HIDE_RULE_HPP

#include "element_hide_rule.hpp"
#include "type.hpp"

#include <vector>

namespace adblock {

class ExceptionElementHideRule : public ElementHideRule
{
    using Base = ElementHideRule;
public:
    ExceptionElementHideRule(StringRange const& selector,
                             std::vector<StringRange> const& domains);
};

std::ostream &operator<<(std::ostream&, ExceptionElementHideRule const&);

} // namespace adblock

#endif // EXCEPTION_ELEMENT_HIDE_RULE_HPP
