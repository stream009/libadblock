#include "parser/filter_option.hpp"
#include "option.hpp"
#include "option/site_key_option.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace adblock;
namespace qi = boost::spirit::qi;

const static parser::FilterOption grammar;

TEST(FilterOptionParser, SiteKey_Basic)
{
    const auto &line = boost::as_literal("sitekey=abcdsitekeydcba");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &option = std::dynamic_pointer_cast<SiteKeyOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(1, option->siteKeys().size());
    EXPECT_TRUE(boost::equals("abcdsitekeydcba", option->siteKeys()[0]));
}

TEST(FilterOptionParser, SiteKey_MultipleSiteKey)
{
    const auto &line =
            boost::as_literal("sitekey=abcdsitekeydcba|bcdesitekeyedcb");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &option = std::dynamic_pointer_cast<SiteKeyOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(2, option->siteKeys().size());
    EXPECT_TRUE(boost::equals("abcdsitekeydcba", option->siteKeys()[0]));
    EXPECT_TRUE(boost::equals("bcdesitekeyedcb", option->siteKeys()[1]));
}

TEST(FilterOptionParser, SiteKey_NoOption1)
{
    const auto &line = boost::as_literal("sitekey");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(FilterOptionParser, SiteKey_NoOption2)
{
    const auto &line = boost::as_literal("sitekey=");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(FilterOptionParser, SiteKey_WrongSiteKey1)
{
    const auto &line = boost::as_literal("sitekey=abcd-sitekey~dcba");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_TRUE(rv); // partial match
    EXPECT_FALSE(it == line.end());

    const auto &option = std::dynamic_pointer_cast<SiteKeyOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(1, option->siteKeys().size());
    EXPECT_TRUE(boost::equals("abcd", option->siteKeys()[0]));
}

TEST(FilterOptionParser, SiteKey_WrongSiteKey2)
{
    const auto &line =
            boost::as_literal("sitekey=abcdsitekeydcba,bcdesitekeyedcb");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_TRUE(rv); // partial match
    EXPECT_FALSE(it == line.end());

    const auto &option = std::dynamic_pointer_cast<SiteKeyOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(1, option->siteKeys().size());
    EXPECT_TRUE(boost::equals("abcdsitekeydcba", option->siteKeys()[0]));
}

TEST(FilterOptionParser, SiteKey_WrongSiteKey3)
{
    const auto &line = boost::as_literal("sitekey=abcdsitekeydcba|");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_TRUE(rv); // partial match
    EXPECT_FALSE(it == line.end());

    const auto &option = std::dynamic_pointer_cast<SiteKeyOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(1, option->siteKeys().size());
    EXPECT_TRUE(boost::equals("abcdsitekeydcba", option->siteKeys()[0]));
}

TEST(FilterOptionParser, SiteKey_WrongSiteKey4)
{
    const auto &line = boost::as_literal("sitekey=|abcdsitekeydcba");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}
