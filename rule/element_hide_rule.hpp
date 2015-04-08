#ifndef ELEMENT_HIDE_RULE_HPP
#define ELEMENT_HIDE_RULE_HPP

#include "rule.hpp"
#include "type.hpp"

#include <vector>

#include <boost/optional.hpp>

namespace adblock {

class ElementHideRule : public Rule
{
    using Base = Rule;
public:
    bool match(const Uri&) const;
    const StringRange &cssSelector();

protected:
    ElementHideRule(const StringRange &selector,
            const boost::optional<std::vector<StringRange>> &domains);
private:
    // @override Rule
    void print(std::ostream&) const override;

    std::vector<StringRange> &includeDomains();
    std::vector<StringRange> &excludeDomains();

private:
    StringRange m_cssSelector;
    boost::optional<std::vector<StringRange>> m_includeDomains;
    boost::optional<std::vector<StringRange>> m_excludeDomains;
};

} // namespace adblock

#endif // ELEMENT_HIDE_RULE_HPP
