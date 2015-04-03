#ifndef OPTION_HPP
#define OPTION_HPP

class Option
{
public:
    virtual bool match(const Uri&, const Context&) const = 0;
};

class ScriptOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        return context.fromScriptTag();
    }
};

class ImageOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        return context.fromImageTag();
    }
};

class StyleSheetOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        return context.fromLinkTagToStyleSheet();
    }
};

class ObjectOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        return context.fromObjectTag();
    }
};

class XmlHttpRequestOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        return context.fromXmlHttpRequest();
    }
};

class ObjectSubRequestOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        return context.fromPluginObject();
    }
};

class SubDocumentOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        return context.fromPluginObject();
    }
};

class DocumentOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        //TODO
    }
};

class ElemHideOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        //TODO
    }
};

class ThirdPartyOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        //TODO
    }
};

class DomainOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        //TODO
    }
};

class SiteKeyOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        //TODO
    }
};

class CollapseOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        //TODO
    }
};

class CollapseOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        //TODO
    }
};

class DoNotTrackOption : public Option
{
public:
    bool match(const Uri&, const Context &context) const override
    {
        //TODO
    }
};

#endif // OPTION_HPP
