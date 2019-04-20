#ifndef EXCEPTION_ELEMENT_HIDE_RULE_HPP
#define EXCEPTION_ELEMENT_HIDE_RULE_HPP

#include "element_hide_rule.hpp"

#include "core/string_range.hpp"

#include <iosfwd>
#include <memory>
#include <vector>

namespace adblock {

class StringRange;

class ExceptionElementHideRule : public ElementHideRule
{
    using Base = ElementHideRule;
    using Domains = Base::Domains;
public:
    ExceptionElementHideRule(StringRange const selector,
                             std::unique_ptr<Domains> domains)
        : Base { selector, std::move(domains) }
    {}
};

std::ostream &operator<<(std::ostream&, ExceptionElementHideRule const&);

} // namespace adblock

#endif // EXCEPTION_ELEMENT_HIDE_RULE_HPP
