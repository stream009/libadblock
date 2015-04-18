#include "option.hpp"
#include "type.hpp"
#include "../mock_context.hpp"

#include <gtest/gtest.h>

namespace adblock {

namespace qi = boost::spirit::qi;

struct PopUpContext : MockContext
{
    bool isPopUp() const override { return true; }
};

TEST(PopUpOption, PopUpContext)
{
    const PopUpOption option {};
    const PopUpContext context {};
    const auto &uri = "http://idontcare.whatever.com/image.jpg"_u;

    EXPECT_TRUE(option.match(uri, context));
}

TEST(PopUpOption, NotPopUpContext)
{
    const PopUpOption option {};
    const MockContext context {};
    const auto &uri = "http://idontcare.whatever.com/image.jpg"_u;

    EXPECT_FALSE(option.match(uri, context));
}

} // namespace adblock
