#ifndef ADBLOCK_RULE_EXTENDED_ELEMENT_HIDE_RULE_HPP
#define ADBLOCK_RULE_EXTENDED_ELEMENT_HIDE_RULE_HPP

#include "element_hide_rule.hpp"
#include "type.hpp"

#include <iosfwd>
#include <vector>

namespace adblock {

class ExtendedElementHideRule : public ElementHideRule
{
    using Base = ElementHideRule;
public:
    ExtendedElementHideRule(StringRange const& selector,
                            std::vector<StringRange> const& domains);
};

std::ostream& operator<<(std::ostream&, ExtendedElementHideRule const&);


} // namespace adblock

#endif // ADBLOCK_RULE_EXTENDED_ELEMENT_HIDE_RULE_HPP
