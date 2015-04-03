#ifndef ELEMENT_HIDE_RULE_HPP
#define ELEMENT_HIDE_RULE_HPP

#include "rule.hpp"

#include <vector>

#include <boost/optional.hpp>
#include <boost/serialization/strong_typedef.hpp>
#include <boost/variant.hpp>

class ElementHideRule : public Rule
{
    using Base = Rule;
public:
    using Base::String;
    using Base::Uri;
    //BOOST_STRONG_TYPEDEF(std::string, IncludeDomain);
    //BOOST_STRONG_TYPEDEF(std::string, ExcludeDomain);
    struct IncludeDomain : std::string {};
    struct ExcludeDomain : std::string {};
    using Domain = boost::variant<IncludeDomain, ExcludeDomain>;

public:
    bool match(const Uri&) const;
    const String &cssSelector();

protected:
    ElementHideRule(const String &selector,
            const boost::optional<std::vector<Domain>> &domains);
private:
    friend std::ostream &operator<<(std::ostream&, const ElementHideRule&);

private:
    String m_cssSelector;
    std::vector<IncludeDomain> m_includeDomains;
    std::vector<ExcludeDomain> m_excludeDomains;
};

#endif // ELEMENT_HIDE_RULE_HPP
