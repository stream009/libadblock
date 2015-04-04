#ifndef PARSER_FILTER_RULE_HPP
#define PARSER_FILTER_RULE_HPP

#include "domain.hpp"

#include "rule/rule.hpp"

#include <memory>

#include <boost/spirit/include/qi.hpp>

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;

template<typename Iterator>
class FilterRule : public qi::grammar<Iterator, std::shared_ptr<Rule>()>
{
public:
    FilterRule();
    ~FilterRule();

private:
    qi::rule<Iterator, std::shared_ptr<Rule>()> m_start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_FILTER_RULE_HPP
