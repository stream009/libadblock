#ifndef OPTION_HPP
#define OPTION_HPP

#include <ostream>
#include <string>
#include <typeinfo>

#include <boost/network/uri.hpp>

class Option
{
public:
    using Uri = boost::network::uri::uri;
    using Context = std::string;

public:
    bool match(const Uri &uri, const Context &context) const
    {
        return doMatch(uri, context);
    }

protected:
    virtual bool doMatch(const Uri&, const Context&) const// = 0;
    { return false; }
};

inline std::ostream &
operator<<(std::ostream &os, const Option &option)
{
    return os << typeid(option).name() << " ";
}

class InversibleOption : public Option
{
public:
    InversibleOption(const bool inverse)
        : m_inverse { inverse }
    {}

    // @overload Option
    bool match(const Uri &uri, const Context &context) const
    {
        return m_inverse ? !doMatch(uri, context) : doMatch(uri, context);
    }

    bool inverse() { return m_inverse; }

private:
    bool m_inverse;
};

class ScriptOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class ImageOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class StyleSheetOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class ObjectOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class XmlHttpRequestOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class ObjectSubRequestOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class SubDocumentOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class DocumentOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class ElemHideOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class OtherOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class ThirdPartyOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class DomainOption : public Option
{
public:
    DomainOption(const std::vector<std::string> &domains)
        : m_domains { domains } //TODO split incude and exclude domain
    {}

private:
    std::vector<std::string> m_domains;
};

class SiteKeyOption : public Option
{
public:
    SiteKeyOption(const std::string &siteKey)
        : m_siteKey { siteKey }
    {}

private:
    std::string m_siteKey;
};

class MatchCaseOption : public Option
{
public:
};

class CollapseOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

class DoNotTrackOption : public Option
{
public:
};

#endif // OPTION_HPP
