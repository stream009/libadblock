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
    ExceptionElementHideRule(const StringRange &selector,
            const boost::optional<std::vector<StringRange>> &domains);
};

std::ostream &operator<<(std::ostream&, const ExceptionElementHideRule&);

} // namespace adblock

#endif // EXCEPTION_ELEMENT_HIDE_RULE_HPP
