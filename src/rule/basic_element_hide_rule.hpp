#ifndef BASIC_ELEMENT_HIDE_RULE_HPP
#define BASIC_ELEMENT_HIDE_RULE_HPP

#include "element_hide_rule.hpp"
#include "type.hpp"

#include <iosfwd>
#include <memory>
#include <vector>

namespace adblock {

class BasicElementHideRule : public ElementHideRule
{
    using Base = ElementHideRule;
    using Domains = Base::Domains;
public:
    BasicElementHideRule(StringRange const& selector,
                         std::unique_ptr<Domains> domains)
        : Base { selector, std::move(domains) }
    {}
};

std::ostream &operator<<(std::ostream&, BasicElementHideRule const&);

} // namespace adblock

#endif // BASIC_ELEMENT_HIDE_RULE_HPP
