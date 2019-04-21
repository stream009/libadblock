#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct ObjectSubrequestContext : MockContext
{
    ObjectSubrequestContext(bool const flag)
        : m_flag { flag }
    {}

    bool fromPlugins() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_ObjectSubrequestOption, ObjectSubrequestContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$object-subrequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    ObjectSubrequestContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_ObjectSubrequestOption, NotObjectSubrequestContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$object-subrequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    ObjectSubrequestContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ObjectSubrequestOption, ObjectSubrequestContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~object-subrequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    ObjectSubrequestContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ObjectSubrequestOption, NotObjectSubrequestContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~object-subrequest"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/data.json" };
    ObjectSubrequestContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
