#include "../mock_context.hpp"

#include "element_hide_rule_base.hpp"
#include "filter_rule_base.hpp"
#include "parser/parser.hpp"
#include "rule/basic_element_hide_rule.hpp"

#include <iostream>

#include <gtest/gtest.h>

namespace adblock {

TEST(GenericHide, Elementary)
{
    ElementHideRuleBase rb;

    const auto &rule1 =
        std::dynamic_pointer_cast<BasicElementHideRule>(
                                                  parser::parse("##div"_r));
    assert(rule1);
    rb.put(*rule1);

    { // elment hide rule "div" should be applied
        const auto &result = rb.query("http://www.adblock.org"_u);
        ASSERT_EQ("div { display: none !important }", result);
    }

    const auto &disabler =
        std::dynamic_pointer_cast<FilterRule>(
                            parser::parse("@@||adblock.org$generichide"_r));
    assert(disabler);
    rb.putGenericDisablerRule(*disabler);

    { // element hide rule "div" should be excluded by generichide rule
        const auto &result = rb.query("http://www.adblock.org"_u);
        ASSERT_TRUE(result.empty()) << result;
    }
}

TEST(GenericHide, OnlyGenericShouldBeHidden)
{
    ElementHideRuleBase rb;

    const auto &rule1 = // generic rule
        std::dynamic_pointer_cast<BasicElementHideRule>(
                                                  parser::parse("##div"_r));
    assert(rule1);
    rb.put(*rule1);

    const auto &rule2 = // domain specific rule
        std::dynamic_pointer_cast<BasicElementHideRule>(
                                      parser::parse("adblock.org##table"_r));
    assert(rule2);
    rb.put(*rule2);

    { // both generic and domain specific rule should be applied
        const auto &result = rb.query("http://www.adblock.org"_u);
        ASSERT_EQ("div, table { display: none !important }", result) << result;
    }

    const auto &disabler =
        std::dynamic_pointer_cast<FilterRule>(
                            parser::parse("@@||adblock.org$generichide"_r));
    assert(disabler);
    rb.putGenericDisablerRule(*disabler);

    { // generic rule should be hidden but domained rule should remain
        const auto &result = rb.query("http://www.adblock.org"_u);
        ASSERT_EQ("table { display: none !important }", result) << result;
    }
}

} // namespace adblock
