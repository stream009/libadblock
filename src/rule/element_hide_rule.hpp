#ifndef ELEMENT_HIDE_RULE_HPP
#define ELEMENT_HIDE_RULE_HPP

#include "rule.hpp"
#include "type.hpp"

#include <memory>
#include <vector>

namespace adblock {

class ElementHideRule : public Rule
{
    using Base = Rule;
public:
    using Domains = std::vector<StringRange>;
    using DomainsRange = boost::iterator_range<Domains::const_iterator>;
public:
    // query
    bool match(Uri const&) const;
    StringRange const& cssSelector() const { return m_cssSelector; }

    bool isDomainRestricted() const;
    DomainsRange includeDomains() const { return m_includeDomains; }
    DomainsRange excludeDomains() const { return m_excludeDomains; }

protected:
    ElementHideRule(StringRange const& selector,
                    std::unique_ptr<Domains> domains);
private:
    // @override Rule
    void print(std::ostream&) const override;

    void varidate() const
    {
        assert(!m_cssSelector.empty());
    }

private:
    StringRange m_cssSelector;
    Domains m_includeDomains;
    Domains m_excludeDomains;
};

} // namespace adblock

#endif // ELEMENT_HIDE_RULE_HPP
