#include "option.hpp"
#include "../mock_context.hpp"

#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

namespace adblock {

namespace qi = boost::spirit::qi;

struct SubDocumentContext : MockContext
{
    SubDocumentContext(const bool flag)
        : m_flag { flag }
    {}

    bool isSubDocument() const override { return m_flag; }

    bool m_flag;
};

const static SubDocumentOption option { false };
const static SubDocumentOption optionInv { true };

TEST(SubDocumentOption, Constructor)
{
    EXPECT_FALSE(option.inverse());
    EXPECT_TRUE(optionInv.inverse());
}

TEST(SubDocumentOption, SubDocumentContext)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const SubDocumentContext context { true };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(SubDocumentOption, NotSubDocumentContext)
{
    const Uri uri { "http://adblock.org/script.js" };
    const SubDocumentContext context { false };

    EXPECT_FALSE(option.match(uri, context));
}

TEST(SubDocumentOption, SubDocumentContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const SubDocumentContext context { true };

    EXPECT_FALSE(optionInv.match(uri, context));
}

TEST(SubDocumentOption, NotSubDocumentContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const SubDocumentContext context { false };

    EXPECT_TRUE(optionInv.match(uri, context));
}

} // namespace adblock
