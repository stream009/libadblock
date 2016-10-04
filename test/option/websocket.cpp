#include "../mock_context.hpp"

#include "filter_rule_base.hpp"
#include "parser/parser.hpp"
#include "rule/filter_rule.hpp"

#include <iostream>

#include <gtest/gtest.h>

namespace adblock {

using NormalContext = MockContext;

struct WebSocketContext : MockContext
{
    bool isWebSocket() const override { return true; }
};

TEST(WebSoket, Elementary)
{
    FilterRuleBase rb;

    const auto &rule1 =
        std::dynamic_pointer_cast<FilterRule>(
                                 parser::parse("adblock$websocket"_r));
    assert(rule1);
    rb.put(*rule1);

    { // request via normal channel
        const auto &rv = rb.query("www.adblock.org"_u, NormalContext());
        EXPECT_FALSE(rv.first);
    }

    { // request via WebSocket
        const auto &rv = rb.query("www.adblock.org"_u, WebSocketContext());
        EXPECT_TRUE(rv.first);
    }
}

TEST(WebSoket, Invert)
{
    FilterRuleBase rb;

    const auto &rule1 =
        std::dynamic_pointer_cast<FilterRule>(
                                 parser::parse("adblock$~websocket"_r));
    assert(rule1);
    rb.put(*rule1);

    { // request via normal channel
        const auto &rv = rb.query("www.adblock.org"_u, NormalContext());
        EXPECT_TRUE(rv.first);
    }

    { // request via WebSocket
        const auto &rv = rb.query("www.adblock.org"_u, WebSocketContext());
        EXPECT_FALSE(rv.first);
    }
}

} // namespace adblock
