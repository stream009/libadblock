#ifndef BASIC_ELEMENT_HIDE_RULE_HPP
#define BASIC_ELEMENT_HIDE_RULE_HPP

#include "element_hide_rule.hpp"

#include "core/string_range.hpp"

#include <iosfwd>
#include <memory>

namespace adblock {

class StringRange;

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
