#ifndef ADBLOCK_MOCK_CONTEXT_HPP
#define ADBLOCK_MOCK_CONTEXT_HPP

#include "context.hpp"
#include "type.hpp"

#include <boost/range/iterator_range.hpp>

namespace adblock {

class MockContext : public Context
{
public:
    MockContext()
        : m_uri { "http://www.adblock.org" },
          m_siteKey { ""_r }
    {}

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
    bool isFont() const override { return false; }
    bool isWebSocket() const override { return false; }

    StringRange siteKey() const override { return m_siteKey; }

private:
    Uri m_uri;
    StringRange m_siteKey;
};

} // namespace adblock

#endif // ADBLOCK_CONTEXT_HPP

