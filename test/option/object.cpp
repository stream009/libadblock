#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct ObjectContext : MockContext
{
    ObjectContext(bool const flag)
        : m_flag { flag }
    {}

    bool fromObjectTag() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_ObjectOption, ObjectContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$object"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/object.swf" };
    ObjectContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_ObjectOption, NotObjectContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$object"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/object.swf" };
    ObjectContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ObjectOption, ObjectContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~object"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/object.swf" };
    ObjectContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_ObjectOption, NotObjectContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~object"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/object.swf" };
    ObjectContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
