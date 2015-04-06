#include "element_hide_rule.hpp"

ElementHideRule::
ElementHideRule(const String &selector,
                const boost::optional<std::vector<Domain>> &domains)
    : m_cssSelector { selector }
{
    struct Visitor : boost::static_visitor<> {
        Visitor(std::vector<IncludeDomain> &includes_,
                std::vector<ExcludeDomain> &excludes_)
            : includes { includes_ },
              excludes { excludes_ }
        {}

        void operator()(const IncludeDomain &domain) {
            includes.push_back(domain);
        }

        void operator()(const ExcludeDomain &domain) {
            excludes.push_back(domain);
        }

        std::vector<IncludeDomain> &includes;
        std::vector<ExcludeDomain> &excludes;

    } visitor { m_includeDomains, m_excludeDomains };

    if (domains) {
        for (const auto &domain: domains.get()) {
            boost::apply_visitor(visitor, domain);
        }
    }
}

void ElementHideRule::
print(std::ostream &os) const
{
    os << "CSS selector: " << m_cssSelector << "\n";
    if (!m_includeDomains.empty()) {
        os << "Include domains: ";
        for (const auto domain: m_includeDomains) {
            os << domain << ' ';
        }
        os << "\n";
    }
    if (!m_excludeDomains.empty()) {
        os << "Exclude domains: ";
        for (const auto domain: m_excludeDomains) {
            os << domain << ' ';
        }
        os << "\n";
    }
}
