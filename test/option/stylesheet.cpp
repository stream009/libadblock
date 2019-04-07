#include "option/style_sheet_option.hpp"
#include "../mock_context.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct StyleSheetContext : MockContext
{
    StyleSheetContext(const bool flag)
        : m_flag { flag }
    {}

    bool isExternalStyleSheet() const override { return m_flag; }

    bool m_flag;
};

const static StyleSheetOption option { false };
const static StyleSheetOption optionInv { true };

TEST(Option_StyleSheetOption, Constructor)
{
    EXPECT_FALSE(option.inverse());
    EXPECT_TRUE(optionInv.inverse());
}

TEST(Option_StyleSheetOption, StyleSheetContext)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const StyleSheetContext context { true };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(Option_StyleSheetOption, NotStyleSheetContext)
{
    const Uri uri { "http://adblock.org/script.js" };
    const StyleSheetContext context { false };

    EXPECT_FALSE(option.match(uri, context));
}

TEST(Option_StyleSheetOption, StyleSheetContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const StyleSheetContext context { true };

    EXPECT_FALSE(optionInv.match(uri, context));
}

TEST(Option_StyleSheetOption, NotStyleSheetContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const StyleSheetContext context { false };

    EXPECT_TRUE(optionInv.match(uri, context));
}

} // namespace adblock
