#include "option/elem_hide_option.hpp"

#include "element_hide_rule_base.hpp"
#include "filter_rule_base.hpp"
#include "parser/parser.hpp"
#include "rule/filter_rule.hpp"
#include "rule/basic_element_hide_rule.hpp"

#include <iostream>

#include <gtest/gtest.h>

namespace adblock {

TEST(ElemHide, Elementary)
{
    FilterRuleBase fb;
    ElementHideRuleBase rb { fb };

    const auto &rule1 =
        std::dynamic_pointer_cast<BasicElementHideRule>(
                                                  parser::parse("##div"_r));
    assert(rule1);
    rb.put(*rule1);

    { // elment hide rule "div" should be applied
        const auto &result = rb.query("http://www.adblock.org"_u);
        ASSERT_EQ("div { display: none !important } ", result);
    }

    const auto &disabler =
        std::dynamic_pointer_cast<FilterRule>(
                            parser::parse("@@||adblock.org$elemhide"_r));
    assert(disabler);

    fb.put(*disabler);

    { // element hide rule "div" should be excluded by elemhide rule
        const auto &result = rb.query("http://www.adblock.org"_u);
        ASSERT_TRUE(result.empty()) << result;
    }
}

TEST(ElemHide, Inversed)
{
    FilterRuleBase fb;
    ElementHideRuleBase rb { fb };

    const auto &rule1 =
        std::dynamic_pointer_cast<BasicElementHideRule>(
                                                  parser::parse("##div"_r));
    assert(rule1);
    rb.put(*rule1);

    { // elment hide rule "div" should be applied
        const auto &result = rb.query("http://www.adblock.org"_u);
        ASSERT_EQ("div { display: none !important } ", result);
    }

    const auto &disabler =
        std::dynamic_pointer_cast<FilterRule>(
                            parser::parse("@@||adblock.org$~elemhide"_r));
    assert(disabler);

    fb.put(*disabler);

    { // inverted elemhide rule should be just ignored
        const auto &result = rb.query("http://www.adblock.org"_u);
        ASSERT_EQ("div { display: none !important } ", result);
    }
}

} // namespace adblock
