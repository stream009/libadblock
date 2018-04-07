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
    using Domains = std::vector<StringRange>;
    using DomainsRange = boost::iterator_range<Domains::const_iterator>;
public:
    // query
    bool match(const Uri&) const;
    const StringRange &cssSelector() const { return m_cssSelector; }

    bool isDomainRestricted() const;
    DomainsRange includeDomains() const;
    DomainsRange excludeDomains() const;

protected:
    ElementHideRule(const StringRange &selector,
                    const boost::optional<Domains> &domains);
private:
    // @override Rule
    void print(std::ostream&) const override;

    void addIncludeDomain(const StringRange&);
    void addExcludeDomain(const StringRange&);

    void varidate() const
    {
        assert(!m_cssSelector.empty());
    }

private:
    StringRange m_cssSelector;
    boost::optional<Domains> m_includeDomains;
    boost::optional<Domains> m_excludeDomains;
};

} // namespace adblock

#endif // ELEMENT_HIDE_RULE_HPP
