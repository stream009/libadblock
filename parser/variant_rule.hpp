#ifndef PARSER_VARIANT_RULE_HPP
#define PARSER_VARIANT_RULE_HPP

#include "rule/comment_rule.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"

#include <boost/variant.hpp>

namespace adblock { namespace parser {

using VariantRule = boost::variant<
                    CommentRule, ::BasicFilterRule, ::ExceptionFilterRule,
                    BasicElementHideRule, ExceptionElementHideRule>;

}} // namespace adblock::parser

#endif // PARSER_VARIANT_RULE_HPP
