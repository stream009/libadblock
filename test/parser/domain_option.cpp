#include "parser/filter_option.hpp"
#include "option.hpp"
#include "option/domain_option.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace adblock;
namespace qi = boost::spirit::qi;

const static parser::FilterOption grammar;

TEST(FilterOptionParser, Domain_Basic)
{
    const auto &line = boost::as_literal("domain=adblock.org");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &option = std::dynamic_pointer_cast<DomainOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(1, option->includeDomains().size());
    EXPECT_EQ(0, option->excludeDomains().size());
    EXPECT_TRUE(boost::equals("adblock.org", option->includeDomains()[0]));
}

TEST(FilterOptionParser, Domain_ExcludeDomain)
{
    const auto &line = boost::as_literal("domain=~adblock.org");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &option = std::dynamic_pointer_cast<DomainOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(0, option->includeDomains().size());
    EXPECT_EQ(1, option->excludeDomains().size());
    EXPECT_TRUE(boost::equals("adblock.org", option->excludeDomains()[0]));
}

TEST(FilterOptionParser, Domain_MultipleDomain1)
{
    const auto &line = boost::as_literal("domain=adblock.org|google.com");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &option = std::dynamic_pointer_cast<DomainOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(2, option->includeDomains().size());
    EXPECT_EQ(0, option->excludeDomains().size());
    EXPECT_TRUE(boost::equals("adblock.org", option->includeDomains()[0]));
    EXPECT_TRUE(boost::equals("google.com", option->includeDomains()[1]));
}

TEST(FilterOptionParser, Domain_MultipleDomain2)
{
    const auto &line = boost::as_literal("domain=adblock.org|~google.com");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &option = std::dynamic_pointer_cast<DomainOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(1, option->includeDomains().size());
    EXPECT_EQ(1, option->excludeDomains().size());
    EXPECT_TRUE(boost::equals("adblock.org", option->includeDomains()[0]));
    EXPECT_TRUE(boost::equals("google.com", option->excludeDomains()[0]));
}

TEST(FilterOptionParser, Domain_NoOption1)
{
    const auto &line = boost::as_literal("domain");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(FilterOptionParser, Domain_NoOption2)
{
    const auto &line = boost::as_literal("domain=");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(FilterOptionParser, Domain_NoOption3)
{
    const auto &line = boost::as_literal("domain=~");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(FilterOptionParser, Domain_WrongDomain1)
{
    const auto &line = boost::as_literal("domain=a$dblock.org");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_TRUE(rv); // partial match
    EXPECT_FALSE(it == line.end());

    const auto &option = std::dynamic_pointer_cast<DomainOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(1, option->includeDomains().size());
    EXPECT_EQ(0, option->excludeDomains().size());
    EXPECT_TRUE(boost::equals("a", option->includeDomains()[0]));
}

TEST(FilterOptionParser, Domain_WrongDomain2)
{
    const auto &line = boost::as_literal("domain=ad,block.org");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_TRUE(rv); // partial match
    EXPECT_FALSE(it == line.end());

    const auto &option = std::dynamic_pointer_cast<DomainOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(1, option->includeDomains().size());
    EXPECT_EQ(0, option->excludeDomains().size());
    EXPECT_TRUE(boost::equals("ad", option->includeDomains()[0]));
}

TEST(FilterOptionParser, Domain_WrongDomain3)
{
    const auto &line = boost::as_literal("domain=adblock.org|");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_TRUE(rv); // partial match
    EXPECT_FALSE(it == line.end());

    const auto &option = std::dynamic_pointer_cast<DomainOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(1, option->includeDomains().size());
    EXPECT_EQ(0, option->excludeDomains().size());
    EXPECT_TRUE(boost::equals("adblock.org", option->includeDomains()[0]));
}

TEST(FilterOptionParser, Domain_WrongDomain4)
{
    const auto &line = boost::as_literal("domain=|adblock.org");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_FALSE(rv);
}

TEST(FilterOptionParser, Domain_WrongDomain5)
{
    const auto &line = boost::as_literal("domain=adblock.org~");
    std::shared_ptr<Option> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    EXPECT_TRUE(rv); // partial match
    EXPECT_FALSE(it == line.end());

    const auto &option = std::dynamic_pointer_cast<DomainOption>(result);
    EXPECT_TRUE(!!option);
    EXPECT_EQ(1, option->includeDomains().size());
    EXPECT_EQ(0, option->excludeDomains().size());
    EXPECT_TRUE(boost::equals("adblock.org", option->includeDomains()[0]));
}

