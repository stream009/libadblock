#ifndef ELEMENT_HIDE_RULE_HPP
#define ELEMENT_HIDE_RULE_HPP

#include "rule.hpp"

#include "core/string_range.hpp"

#include <memory>
#include <vector>

namespace adblock {

class Uri;

class ElementHideRule : public Rule
{
    using Base = Rule;
public:
    using Domains = std::vector<StringRange>;
public:
    // accessor
    StringRange const& cssSelector() const { return m_cssSelector; }
    Domains const* domains() const { return m_domains.get(); }

    // query
    bool match(Uri const&) const;

    bool isDomainRestricted() const { return !!m_domains; }

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
    std::unique_ptr<Domains> m_domains;
};

} // namespace adblock

#endif // ELEMENT_HIDE_RULE_HPP
