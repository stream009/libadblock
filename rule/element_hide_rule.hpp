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
    using DomainsRange = boost::iterator_range<
                            std::vector<StringRange>::const_iterator>;
public:
    bool match(const Uri&) const;
    const StringRange &cssSelector() const { return m_cssSelector; }

    DomainsRange includeDomains() const;
    DomainsRange excludeDomains() const;

protected:
    ElementHideRule(const StringRange &selector,
            const boost::optional<std::vector<StringRange>> &domains);
private:
    // @override Rule
    void print(std::ostream&) const override;

    void addIncludeDomain(const StringRange&);
    void addExcludeDomain(const StringRange&);

private:
    StringRange m_cssSelector;
    boost::optional<std::vector<StringRange>> m_includeDomains;
    boost::optional<std::vector<StringRange>> m_excludeDomains;
};

} // namespace adblock

#endif // ELEMENT_HIDE_RULE_HPP
