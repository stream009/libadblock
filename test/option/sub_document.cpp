#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "core/uri.hpp"
#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct SubdocumentContext : MockContext
{
    SubdocumentContext(bool const flag)
        : m_flag { flag }
    {}

    bool isSubdocument() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_SubdocumentOption, SubdocumentContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$subdocument"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/doc.html" };
    SubdocumentContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_SubdocumentOption, NotSubdocumentContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$subdocument"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/doc.html" };
    SubdocumentContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_SubdocumentOption, SubdocumentContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~subdocument"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/doc.html" };
    SubdocumentContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_SubdocumentOption, NotSubdocumentContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~subdocument"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/doc.html" };
    SubdocumentContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
