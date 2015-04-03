#ifndef ELEMENT_HIDE_RULE_HPP
#define ELEMENT_HIDE_RULE_HPP

#include "rule.hpp"

#include <vector>

class ElementHideRule : public Rule
{
    using Base = Rule;
public:
    using Base::StringRange;
    using Base::Uri;

public:
    bool match(const Uri&) const;
    const StringRange &cssSelector();

protected:
    using Base::Base;
    void parseDomain(const StringRange&);

    template<typename StrR>
    void setCssSelector(StrR&&);

private:
    StringRange m_cssSelector;
    std::vector<StringRange> m_includeDomains;
    std::vector<StringRange> m_excludeDomains;
};

#include <utility>

template<typename StrR>
void ElementHideRule::
setCssSelector(StrR &&range)
{
    m_cssSelector = std::forward<StrR>(range);
}

#endif // ELEMENT_HIDE_RULE_HPP
