#ifndef ADBLOCK_CONTEXT_HPP
#define ADBLOCK_CONTEXT_HPP

#include "type.hpp"

namespace adblock {

class Context
{
public:
    virtual ~Context() = default;

    virtual const Uri &origin() const = 0;

    virtual bool fromScriptTag() const = 0;
    virtual bool fromImageTag() const = 0;
    virtual bool fromAudioVideoTag() const = 0;
    virtual bool isExternalStyleSheet() const = 0;
    virtual bool fromObjectTag() const = 0;
    virtual bool fromXmlHttpRequest() const = 0;
    virtual bool fromPlugins() const = 0;
    virtual bool isSubDocument() const = 0;
    virtual bool isPopUp() const = 0;
    virtual bool isFont() const = 0;

    virtual StringRange siteKey() const = 0;
};

} // namespace adblock

#endif // ADBLOCK_CONTEXT_HPP
