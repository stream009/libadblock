#include "option/web_rtc_option.hpp"
#include "../mock_context.hpp"

#include "filter_rule_base.hpp"
#include "parser/parser.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

using NormalContext = MockContext;

struct WebRtcContext : MockContext
{
    bool isWebRtc() const override { return true; }
};

TEST(WebRtc, Elementary)
{
    FilterRuleBase rb;

    auto const& rule1 =
        std::dynamic_pointer_cast<FilterRule>(
                                 parser::parse("adblock$webrtc"_r));
    assert(rule1);
    rb.put(*rule1);

    { // request via normal channel
        auto const& rv = rb.query("http://www.adblock.org"_u, NormalContext());
        EXPECT_FALSE(rv.first);
    }

    { // request via WebSocket
        auto const& rv = rb.query("http://www.adblock.org"_u, WebRtcContext());
        EXPECT_TRUE(rv.first);
    }
}

TEST(WebRtc, Invert)
{
    FilterRuleBase rb;

    auto const& rule1 =
        std::dynamic_pointer_cast<FilterRule>(
                                 parser::parse("adblock$~webrtc"_r));
    assert(rule1);
    rb.put(*rule1);

    { // request via normal channel
        auto const& rv = rb.query("http://www.adblock.org"_u, NormalContext());
        EXPECT_TRUE(rv.first);
    }

    { // request via WebSocket
        auto const& rv = rb.query("http://www.adblock.org"_u, WebRtcContext());
        EXPECT_FALSE(rv.first);
    }
}

} // namespace adblock

