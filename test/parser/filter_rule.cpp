#include "option.hpp"
#include "parser/filter_rule.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace adblock;
namespace qi = boost::spirit::qi;

const static parser::FilterRule grammar;

TEST(BasicFilterRuleParser, Basic)
{
    const auto &line = boost::as_literal("adblock.org");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &rule = std::dynamic_pointer_cast<BasicFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &pattern =
                dynamic_cast<const BasicMatchPattern*>(&(rule->pattern()));
    EXPECT_TRUE(!!pattern);

    const auto &options = rule->options();
    EXPECT_TRUE(options.empty());
}

TEST(BasicFilterRuleParser, OneOption)
{
    const auto &line = boost::as_literal("||adblock.org$match-case");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &rule = std::dynamic_pointer_cast<BasicFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &pattern =
                dynamic_cast<const DomainMatchPattern*>(&(rule->pattern()));
    EXPECT_TRUE(!!pattern);

    const auto &options = rule->options();
    EXPECT_EQ(1, options.size());
    EXPECT_TRUE(!!std::dynamic_pointer_cast<MatchCaseOption>(options[0]));
}

TEST(BasicFilterRuleParser, TwoOptions)
{
    const auto &line = boost::as_literal("adblock.org$~script,match-case");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &rule = std::dynamic_pointer_cast<BasicFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &options = rule->options();
    EXPECT_EQ(2, options.size());

    const auto &scriptOption =
                    std::dynamic_pointer_cast<ScriptOption>(options[0]);
    ASSERT_TRUE(!!scriptOption);
    EXPECT_TRUE(scriptOption->inverse());

    EXPECT_TRUE(!!std::dynamic_pointer_cast<MatchCaseOption>(options[1]));
}

TEST(BasicFilterRuleParser, WrongOption1)
{
    const auto &line = boost::as_literal("adblock.org$");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv);
    EXPECT_TRUE(it != line.end() && *it == '$'); // partial match

    const auto &rule = std::dynamic_pointer_cast<BasicFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &pattern =
                dynamic_cast<const BasicMatchPattern*>(&(rule->pattern()));
    EXPECT_TRUE(!!pattern);

    EXPECT_TRUE(rule->options().empty());
}

TEST(BasicFilterRuleParser, WrongOption2)
{
    const auto &line = boost::as_literal("adblock.org$~script, match-case");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv); // partial match
    EXPECT_TRUE(it != line.end() && *it == ',');

    const auto &rule = std::dynamic_pointer_cast<BasicFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &options = rule->options();
    EXPECT_EQ(1, options.size());

    const auto &scriptOption =
                    std::dynamic_pointer_cast<ScriptOption>(options[0]);
    ASSERT_TRUE(!!scriptOption);
    EXPECT_TRUE(scriptOption->inverse());
}

TEST(BasicFilterRuleParser, WrongOption3)
{
    const auto &line = boost::as_literal("adblock.org$~script,match-case,");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv); // partial match
    EXPECT_TRUE(it != line.end() && *it == ',');

    const auto &rule = std::dynamic_pointer_cast<BasicFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &options = rule->options();
    EXPECT_EQ(2, options.size());

    const auto &scriptOption =
                    std::dynamic_pointer_cast<ScriptOption>(options[0]);
    ASSERT_TRUE(!!scriptOption);
    EXPECT_TRUE(scriptOption->inverse());

    EXPECT_TRUE(!!std::dynamic_pointer_cast<MatchCaseOption>(options[1]));
}

TEST(BasicFilterRuleParser, DocumentOptionIsOnlyAllowedInExceptionRule)
{
    const auto &line = boost::as_literal("adblock.org$document");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv); // partial match
    EXPECT_TRUE(it != line.end() && *it == '$');

    const auto &rule = std::dynamic_pointer_cast<BasicFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &options = rule->options();
    EXPECT_TRUE(options.empty());
}

TEST(BasicFilterRuleParser, ElemHideOptionIsOnlyAllowedInExceptionRule)
{
    const auto &line = boost::as_literal("adblock.org$elemhide");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv); // partial match
    EXPECT_TRUE(it != line.end() && *it == '$');

    const auto &rule = std::dynamic_pointer_cast<BasicFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &options = rule->options();
    EXPECT_TRUE(options.empty());
}

TEST(ExceptionFilterRuleParser, Basic)
{
    const auto &line = boost::as_literal("@@adblock.org");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());
    const auto &rule = std::dynamic_pointer_cast<ExceptionFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &pattern =
                dynamic_cast<const BasicMatchPattern*>(&(rule->pattern()));
    EXPECT_TRUE(!!pattern);

    ASSERT_TRUE(rule->options().empty());
}

TEST(ExceptionFilterRuleParser, Error)
{
    const auto &line = boost::as_literal("@adblock.org");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv);
    const auto &rule = std::dynamic_pointer_cast<BasicFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &pattern =
                dynamic_cast<const BasicMatchPattern*>(&(rule->pattern()));
    EXPECT_TRUE(!!pattern);

    ASSERT_TRUE(rule->options().empty());
}

TEST(ExceptionFilterRuleParser, DocumentOptionIsOnlyAllowedInExceptionRule)
{
    const auto &line = boost::as_literal("@@adblock.org$document");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());

    const auto &rule = std::dynamic_pointer_cast<ExceptionFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &pattern =
                dynamic_cast<const BasicMatchPattern*>(&(rule->pattern()));
    ASSERT_TRUE(!!pattern);
    EXPECT_EQ("adblock.org", boost::lexical_cast<std::string>(*pattern));

    const auto &options = rule->options();
    ASSERT_EQ(1, options.size());
    const auto &option =
                std::dynamic_pointer_cast<DocumentOption>(options[0]);
    ASSERT_TRUE(!!option);
    EXPECT_FALSE(option->inverse());
}

TEST(ExceptionFilterRuleParser, ElemHideOptionIsOnlyAllowedInExceptionRule)
{
    const auto &line = boost::as_literal("@@adblock.org$~elemhide");
    std::shared_ptr<Rule> result;
    auto it = line.begin();
    const auto rv = qi::parse(it, line.end(), grammar, result);

    ASSERT_TRUE(rv && it == line.end());

    const auto &rule = std::dynamic_pointer_cast<ExceptionFilterRule>(result);
    EXPECT_TRUE(!!rule);

    const auto &pattern =
                dynamic_cast<const BasicMatchPattern*>(&(rule->pattern()));
    ASSERT_TRUE(!!pattern);
    EXPECT_EQ("adblock.org", boost::lexical_cast<std::string>(*pattern));

    const auto &options = rule->options();
    ASSERT_EQ(1, options.size());
    const auto &option =
                std::dynamic_pointer_cast<ElemHideOption>(options[0]);
    ASSERT_TRUE(!!option);
    EXPECT_TRUE(option->inverse());
}
