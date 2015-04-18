#ifndef ADBLOCK_MOCK_CONTEXT_HPP
#define ADBLOCK_MOCK_CONTEXT_HPP

#include "context.hpp"
#include "type.hpp"

namespace adblock {

class MockContext : public Context
{
public:
    const Uri &origin() const override { return m_uri; }

    bool fromScriptTag() const override { return false; }
    bool fromImageTag() const override { return false; }
    bool fromAudioVideoTag() const override { return false; }
    bool isExternalStyleSheet() const override { return false; }
    bool fromObjectTag() const override { return false; }
    bool fromXmlHttpRequest() const override { return false; }
    bool fromPlugins() const override { return false; }
    bool isSubDocument() const override { return false; }
    bool isPopUp() const override { return false; }

    StringRange siteKey() const override { return m_siteKey; }

private:
    Uri m_uri;
    StringRange m_siteKey { "" };
};

} // namespace adblock

#endif // ADBLOCK_CONTEXT_HPP

