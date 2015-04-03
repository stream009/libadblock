#ifndef PARSER_GRAMMER_HPP
#define PARSER_GRAMMER_HPP

#include "element_hide_rule.hpp"
#include "filter_rule.hpp"
#include "variant_rule.hpp"

#include <boost/spirit/include/qi.hpp>

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;

template<typename Iterator>
struct Grammar : qi::grammar<Iterator, VariantRule()>
{
public:
    Grammar();

private:
    qi::rule<Iterator, VariantRule()> rule;

    FilterRule<Iterator> filter_rule;
    ElementHideRule<Iterator> element_hide_rule;
    qi::rule<Iterator, VariantRule()> comment_rule;
};

}} // namespace adblock::parser

#endif // PARSER_GRAMMER_HPP
