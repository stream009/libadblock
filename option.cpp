#include "option.hpp"
#include "domain_data_base.hpp"

#include <iterator>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/string.hpp>

namespace adblock {

bool ScriptOption::
doMatch(const Uri&, const Context &context) const
{
    return context.fromScriptTag();
}

bool ImageOption::
doMatch(const Uri&, const Context &context) const
{
    return context.fromImageTag();
}

bool StyleSheetOption::
doMatch(const Uri&, const Context &context) const
{
    return context.isExternalStyleSheet();
}

bool ObjectOption::
doMatch(const Uri&, const Context &context) const
{
    return context.fromObjectTag();
}

bool XmlHttpRequestOption::
doMatch(const Uri&, const Context &context) const
{
    return context.fromXmlHttpRequest();
}

bool ObjectSubRequestOption::
doMatch(const Uri&, const Context &context) const
{
    return context.fromPlugins();
}

bool SubDocumentOption::
doMatch(const Uri&, const Context &context) const
{
    return context.isSubDocument();
}

bool OtherOption::
doMatch(const Uri&, const Context &context) const
{
    return !context.fromScriptTag()
        && !context.fromImageTag()
        && !context.isExternalStyleSheet()
        && !context.fromObjectTag()
        && !context.fromXmlHttpRequest()
        && !context.fromPlugins()
        && !context.isSubDocument()
        // According to the specification,(https://adblockplus.org/en/filters)
        // this option also has to exclude the document option and the
        // elemhide option.
        // But those two options are option to specify whitelist.
        // I don't know how to exclude them.
    ;
}

bool ThirdPartyOption::
doMatch(const Uri &uri, const Context &context) const
{
    DomainDataBase ddb;

    const auto &uriDomain = ddb.query(uri);
    const auto &originDomain = ddb.query(context.origin());

    return !boost::equals(uriDomain, originDomain);
}

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

    const auto &host = origin.host_range();
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

bool SiteKeyOption::
doMatch(const Uri&, const Context &context) const
{
    const auto &theSiteKey = context.siteKey();

    namespace ba = boost::algorithm;
    return ba::any_of(m_siteKeys,
        [&theSiteKey] (const StringRange &aSiteKey) {
            return ba::equals(theSiteKey, aSiteKey);
        }
    );
}

bool PopUpOption::
doMatch(const Uri&, const Context &context) const
{
    return context.isPopUp();
}

bool MediaOption::
doMatch(const Uri&, const Context &context) const
{
    return context.fromAudioVideoTag();
}

bool FontOption::
doMatch(const Uri&, const Context &context) const
{
    return context.isFont();
}

} // namespace adblock
