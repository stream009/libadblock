#ifndef ADBLOCK_CONTEXT_HPP
#define ADBLOCK_CONTEXT_HPP

#include "type.hpp"

namespace adblock {

class Context
{
public:
    virtual ~Context() = default;

    virtual Uri const& origin() const = 0;

    virtual bool fromScriptTag() const { return false; }
    virtual bool fromImageTag() const  { return false; }
    virtual bool fromAudioVideoTag() const { return false; }
    virtual bool isExternalStyleSheet() const { return false; }
    virtual bool fromObjectTag() const { return false; }
    virtual bool fromXmlHttpRequest() const { return false; }
    virtual bool fromPlugins() const { return false; }
    virtual bool isSubDocument() const { return false; }
    virtual bool isPopUp() const { return false; }
    virtual bool isFont() const { return false; }
    virtual bool isWebSocket() const { return false; }
    virtual bool isPing() const { return false; }

    virtual StringRange siteKey() const { return {}; }
};

} // namespace adblock

#endif // ADBLOCK_CONTEXT_HPP
