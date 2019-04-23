#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct PopupContext : MockContext
{
    bool isPopup() const override { return true; }
};

TEST(Option_PopupOption, PopupContext)
{
    auto const rule = parse_rule<FilterRule>("whatever$popup"_r);
    ASSERT_TRUE(rule);

    auto const& uri = "http://idontcare.whatever.com/image.jpg"_u;
    PopupContext const context;

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_PopupOption, NotPopupContext)
{
    auto const rule = parse_rule<FilterRule>("whatever$popup"_r);
    ASSERT_TRUE(rule);

    auto const& uri = "http://idontcare.whatever.com/image.jpg"_u;
    MockContext const context;

    EXPECT_FALSE(rule->match(uri, context));
}

} // namespace adblock
