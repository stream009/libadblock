#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "filter_rule_base.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

using NormalContext = MockContext;

struct WebSocketContext : MockContext
{
    bool isWebSocket() const override { return true; }
};

TEST(Option_WebSocket, Elementary)
{
    FilterRuleBase rb;

    auto const rule1 = parse_rule<FilterRule>("adblock$websocket"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    { // request via normal channel
        auto const& rv = rb.query("http://www.adblock.org"_u, NormalContext());
        EXPECT_FALSE(rv.first);
    }

    { // request via WebSocket
        auto const& rv = rb.query("http://www.adblock.org"_u, WebSocketContext());
        EXPECT_TRUE(rv.first);
    }
}

TEST(Option_WebSocket, Invert)
{
    FilterRuleBase rb;

    auto const rule1 = parse_rule<FilterRule>("adblock$~websocket"_r);
    ASSERT_TRUE(rule1);

    rb.put(*rule1);

    { // request via normal channel
        auto const& rv = rb.query("http://www.adblock.org"_u, NormalContext());
        EXPECT_TRUE(rv.first);
    }

    { // request via WebSocket
        auto const& rv = rb.query("http://www.adblock.org"_u, WebSocketContext());
        EXPECT_FALSE(rv.first);
    }
}

} // namespace adblock
