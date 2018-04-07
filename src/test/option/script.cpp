#include "option/script_option.hpp"
#include "../mock_context.hpp"

#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

namespace adblock {

namespace qi = boost::spirit::qi;

struct ScriptContext : MockContext
{
    ScriptContext(const bool flag)
        : m_flag { flag }
    {}

    bool fromScriptTag() const override { return m_flag; }

    bool m_flag;
};

const static ScriptOption option { false };
const static ScriptOption optionInv { true };

TEST(ScriptOption, Constructor)
{
    EXPECT_FALSE(option.inverse());
    EXPECT_TRUE(optionInv.inverse());
}

TEST(ScriptOption, ScriptContext)
{
    const Uri uri { "http://adblock.org/script.js" };
    const ScriptContext context { true };

    EXPECT_TRUE(option.match(uri, context));
}

TEST(ScriptOption, NotScriptContext)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const ScriptContext context { false };

    EXPECT_FALSE(option.match(uri, context));
}

TEST(ScriptOption, ScriptContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/script.js" };
    const ScriptContext context { true };

    EXPECT_FALSE(optionInv.match(uri, context));
}

TEST(ScriptOption, NotScriptContextWithInverseOption)
{
    const Uri uri { "http://adblock.org/image.jpg" };
    const ScriptContext context { false };

    EXPECT_TRUE(optionInv.match(uri, context));
}

} // namespace adblock
