#include "../parse_rule.hpp"

#include "core/string_range.hpp"
#include "rule/filter_rule.hpp"

#include <memory>

#include <gtest/gtest.h>

using namespace adblock;

TEST(Parser_ParseError, Basic)
{
    auto const line = "ad$foobar"_r;

    ASSERT_THROW(parse_rule<FilterRule>(line), parse_error);

    try {
        parse_rule<FilterRule>(line);
    }
    catch (parse_error const& e) {
        auto const errors = e.errors();

        ASSERT_EQ(1, errors.size());

        auto const& err = errors[0];
        EXPECT_EQ(1, err.line_no);
        EXPECT_EQ(3, err.from);
        EXPECT_EQ(8, err.to);
        EXPECT_EQ(line, err.line);
        EXPECT_EQ("invalid option", err.message);
    }
}

