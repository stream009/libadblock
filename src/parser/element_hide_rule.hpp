#ifndef PARSER_ELEMENT_HIDE_RULE_HPP
#define PARSER_ELEMENT_HIDE_RULE_HPP

#include "domain.hpp"
#include "type.hpp"
#include "rule/rule.hpp"

#include <memory>

namespace adblock { namespace parser {

class ElementHideRule : public grammar<std::shared_ptr<Rule>()>
{
public:
    ElementHideRule();
    ~ElementHideRule();

private:
    rule<std::shared_ptr<Rule>()> start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_ELEMENT_HIDE_RULE_HPP
