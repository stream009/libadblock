#ifndef OPTION_HPP
#define OPTION_HPP

#include "type.hpp"

#include <ostream>
#include <typeinfo>

namespace adblock {

class Option
{
public:
    using Context = size_t;

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
    DomainOption(const std::vector<StringRange> &domains)
        : m_domains { domains } //TODO split incude and exclude domain
    {}

private:
    std::vector<StringRange> m_domains;
};

class SiteKeyOption : public Option
{
public:
    SiteKeyOption(const StringRange &siteKey)
        : m_siteKey { siteKey }
    {}

private:
    StringRange m_siteKey;
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

class PopUpOption : public Option
{
public:
};

class MediaOption : public InversibleOption
{
    using Base = InversibleOption;
public:
    using Base::Base;
};

} // namespace adblock

#endif // OPTION_HPP
