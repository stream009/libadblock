#ifndef PARSER_FILTER_RULE_HPP
#define PARSER_FILTER_RULE_HPP

#include "domain.hpp"
#include "type.hpp"
#include "rule/rule.hpp"

#include <memory>

namespace adblock { namespace parser {

class FilterRule : public grammar<std::shared_ptr<Rule>()>
{
public:
    FilterRule();
    ~FilterRule();

private:
    rule<std::shared_ptr<Rule>()> m_start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_FILTER_RULE_HPP
