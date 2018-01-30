#include "domain_option.hpp"

#include "context.hpp"

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/string.hpp>

namespace adblock {

DomainOption::
DomainOption(const Domains &domains)
{
    assert(!domains.empty());
    for (const auto &domain: domains) {
        assert(!domain.empty());
        if (domain[0] != '~') {
            m_includeDomains.push_back(domain);
        }
        else {
            assert(domain.size() > 1);
            m_excludeDomains.emplace_back(
                        std::next(domain.begin()), domain.end());
        }
    }
    validate();
}

DomainOption::DomainsRange DomainOption::
includeDomains() const
{
    return m_includeDomains;
}

DomainOption::DomainsRange DomainOption::
excludeDomains() const
{
    return m_excludeDomains;
}

bool DomainOption::
doMatch(const Uri&, const Context &context) const
{
    const auto &origin = context.origin();
    if (!origin.is_valid()) return false;

    const auto &host = origin.host();
    if (host.empty()) return false;

    namespace ba = boost::algorithm;

    const auto &predicate =
        [&host] (const StringRange &domain) {
            return ba::ends_with(host, domain);
        };

    // invariant guarantees either include or exclude domain is not empty

    // check exclude domains first
    if (ba::any_of(m_excludeDomains, predicate)) return false;

    if (m_includeDomains.empty()) {
        // In case of options which have only exclude domains,
        // match everything unless the target belongs to exclude domains.
        return true;
    }
    else {
        return ba::any_of(m_includeDomains, predicate);
    }
}

} // namespace adblock
