#include "../mock_context.hpp"
#include "../parse_rule.hpp"

#include "rule/filter_rule.hpp"

#include <gtest/gtest.h>

namespace adblock {

struct SubDocumentContext : MockContext
{
    SubDocumentContext(const bool flag)
        : m_flag { flag }
    {}

    bool isSubDocument() const override { return m_flag; }

    bool m_flag;
};

TEST(Option_SubDocumentOption, SubDocumentContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$subdocument"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/doc.html" };
    SubDocumentContext const context { true };

    EXPECT_TRUE(rule->match(uri, context));
}

TEST(Option_SubDocumentOption, NotSubDocumentContext)
{
    auto const rule = parse_rule<FilterRule>("adblock$subdocument"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/doc.html" };
    SubDocumentContext const context { false };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_SubDocumentOption, SubDocumentContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~subdocument"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/doc.html" };
    SubDocumentContext const context { true };

    EXPECT_FALSE(rule->match(uri, context));
}

TEST(Option_SubDocumentOption, NotSubDocumentContextWithInverseOption)
{
    auto const rule = parse_rule<FilterRule>("adblock$~subdocument"_r);
    ASSERT_TRUE(rule);

    Uri const uri { "http://adblock.org/doc.html" };
    SubDocumentContext const context { false };

    EXPECT_TRUE(rule->match(uri, context));
}

} // namespace adblock
