#include "option/object_option.hpp"
#include "../mock_context.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct ObjectContext : MockContext
{
    ObjectContext(const bool flag)
        : m_flag { flag }
    {}

    bool fromObjectTag() const override { return m_flag; }

    bool m_flag;
};

const static ObjectOption option { false };
const static ObjectOption optionInv { true };

TEST(Option_ObjectOption, Constructor)
{
    EXPECT_FALSE(option.inverse());
    EXPECT_TRUE(optionInv.inverse());
}

TEST(Option_ObjectOption, ObjectContext)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const ObjectContext context { true };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(Option_ObjectOption, NotObjectContext)
{
    const Uri uri { "http://adblock.org/script.js" };
    const ObjectContext context { false };

    EXPECT_FALSE(option.match(uri, context));
}

TEST(Option_ObjectOption, ObjectContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const ObjectContext context { true };

    EXPECT_FALSE(optionInv.match(uri, context));
}

TEST(Option_ObjectOption, NotObjectContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const ObjectContext context { false };

    EXPECT_TRUE(optionInv.match(uri, context));
}

} // namespace adblock
