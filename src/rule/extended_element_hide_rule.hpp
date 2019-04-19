#ifndef ADBLOCK_RULE_EXTENDED_ELEMENT_HIDE_RULE_HPP
#define ADBLOCK_RULE_EXTENDED_ELEMENT_HIDE_RULE_HPP

#include "element_hide_rule.hpp"
#include "core/type.hpp"

#include <iosfwd>
#include <memory>
#include <vector>

namespace adblock {

class ExtendedElementHideRule : public ElementHideRule
{
    using Base = ElementHideRule;
    using Domains = Base::Domains;
public:
    ExtendedElementHideRule(StringRange const& selector,
                            std::unique_ptr<Domains> domains)
        : Base { selector, std::move(domains) }
    {}
};

std::ostream& operator<<(std::ostream&, ExtendedElementHideRule const&);


} // namespace adblock

#endif // ADBLOCK_RULE_EXTENDED_ELEMENT_HIDE_RULE_HPP
