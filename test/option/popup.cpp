#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct PopUpContext : MockContext
{
    bool isPopUp() const override { return true; }
};

TEST(Option_PopUpOption, PopUpContext)
{
    auto const rule = parse_rule<FilterRule>("whatever$popup"_r);
    ASSERT_TRUE(rule);

    auto const& uri = "http://idontcare.whatever.com/image.jpg"_u;
    PopUpContext const context;

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_PopUpOption, NotPopUpContext)
{
    auto const rule = parse_rule<FilterRule>("whatever$popup"_r);
    ASSERT_TRUE(rule);

    auto const& uri = "http://idontcare.whatever.com/image.jpg"_u;
    MockContext const context;

    EXPECT_FALSE(rule->match(uri, context));
}

} // namespace adblock
