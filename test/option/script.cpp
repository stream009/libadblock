#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct ScriptContext : MockContext
{
    ScriptContext(bool const flag)
        : m_flag { flag }
    {}

    bool fromScriptTag() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_ScriptOption, ScriptContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$script"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/script.js" };
    ScriptContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_ScriptOption, NotScriptContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$script"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/script.js" };
    ScriptContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ScriptOption, ScriptContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~script"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/script.js" };
    ScriptContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ScriptOption, NotScriptContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~script"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/script.js" };
    ScriptContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
