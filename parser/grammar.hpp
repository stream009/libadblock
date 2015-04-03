#ifndef PARSER_GRAMMER_HPP
#define PARSER_GRAMMER_HPP

#include "element_hide_rule.hpp"
#include "filter_rule.hpp"

#include <memory>

#include <boost/spirit/include/qi.hpp>

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;

template<typename Iterator>
struct Grammar : qi::grammar<Iterator, std::shared_ptr<Rule>()>
{
public:
    Grammar();

private:
    qi::rule<Iterator, std::shared_ptr<Rule>()> rule;

    FilterRule<Iterator> filter_rule;
    ElementHideRule<Iterator> element_hide_rule;
    qi::rule<Iterator, std::shared_ptr<Rule>()> comment_rule;
};

}} // namespace adblock::parser

#endif // PARSER_GRAMMER_HPP
