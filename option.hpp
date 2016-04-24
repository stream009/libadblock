#ifndef OPTION_HPP
#define OPTION_HPP

#include "context.hpp"
#include "type.hpp"

#include <ostream>
#include <typeinfo>

namespace adblock {

class Option
{
public:
    virtual ~Option() = default;

    bool match(const Uri &uri, const Context &context) const
    {
        return doMatch(uri, context);
    }

protected:
    virtual bool doMatch(const Uri&, const Context&) const
    {
        return true;
    }
};

inline std::ostream &
operator<<(std::ostream &os, const Option &option)
{
    return os << typeid(option).name() << " ";
}

class InversibleOption : public Option
{
public:
    // @override Option
    bool doMatch(const Uri &uri, const Context &context) const override final
    {
        return m_inverse ? !doMatch2(uri, context)
                         :  doMatch2(uri, context);
    }

    bool inverse() const { return m_inverse; }

protected:
    InversibleOption(const bool inverse)
        : m_inverse { inverse }
    {}

private:
    virtual bool doMatch2(const Uri &uri, const Context &context) const
    {
        return Option::doMatch(uri, context);
    }

private:
    bool m_inverse;
};

class ScriptOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    ScriptOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class ImageOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    ImageOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class StyleSheetOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    StyleSheetOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class ObjectOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    ObjectOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class XmlHttpRequestOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    XmlHttpRequestOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class ObjectSubRequestOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    ObjectSubRequestOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class SubDocumentOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    SubDocumentOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class DocumentOption : public InversibleOption //TODO test
{
    using Base = InversibleOption;
public:
    DocumentOption(const bool inverse) : Base { inverse } {}
};

class ElemHideOption : public InversibleOption //TODO test
{
    using Base = InversibleOption;
public:
    ElemHideOption(const bool inverse) : Base { inverse } {}
};

class OtherOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    OtherOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class ThirdPartyOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    ThirdPartyOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class DomainOption : public Option
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

class SiteKeyOption : public Option
{
public:
    using SiteKeys = std::vector<StringRange>;
    using SiteKeysRange = boost::iterator_range<SiteKeys::const_iterator>;
public:
    SiteKeyOption(const SiteKeys &siteKeys)
        : m_siteKeys { siteKeys }
    {}

    SiteKeysRange siteKeys() const { return m_siteKeys; }

private:
    bool doMatch(const Uri&, const Context&) const override;

    void validate()
    {
        assert(!m_siteKeys.empty());
    }

private:
    SiteKeys m_siteKeys;
};

class MatchCaseOption : public Option {};

class CollapseOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    CollapseOption(const bool inverse) : Base { inverse } {}
};

class DoNotTrackOption : public Option {}; //TODO test

class PopUpOption : public Option
{
private:
    bool doMatch(const Uri&, const Context&) const override;
};

class MediaOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    MediaOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class FontOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    FontOption(const bool inverse) : Base { inverse } {}

private:
    bool doMatch2(const Uri&, const Context&) const override;
};

class GenericHideOption : public Option {};

class GenericBlockOption : public Option {};

} // namespace adblock

#endif // OPTION_HPP
