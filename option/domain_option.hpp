#ifndef ADBLOCK_OPTION_DOMAIN_OPTION_HPP
#define ADBLOCK_OPTION_DOMAIN_OPTION_HPP

#include "type.hpp"
#include "option.hpp"
#include "restriction_option.hpp"

#include <vector>

#include <boost/range/iterator_range.hpp>

namespace adblock {

class Context;

class DomainOption : public Option, public RestrictionOption
{
public:
    using Domains = std::vector<StringRange>;
    using DomainsRange = boost::iterator_range<Domains::const_iterator>;

public:
    DomainOption(const Domains&);

    DomainsRange includeDomains() const;
    DomainsRange excludeDomains() const;

private:
    bool doMatch(const Uri&, const Context&) const override;

    void validate() const
    {
        assert(!m_includeDomains.empty() ||
               !m_excludeDomains.empty());
    }

private:
    Domains m_includeDomains;
    Domains m_excludeDomains;
};

} // namespace adblock

#endif // ADBLOCK_OPTION_DOMAIN_OPTION_HPP
