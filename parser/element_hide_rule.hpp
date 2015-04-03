#ifndef PARSER_ELEMENT_HIDE_RULE_HPP
#define PARSER_ELEMENT_HIDE_RULE_HPP

#include "domain.hpp"
#include "variant_rule.hpp"

#include <memory>

#include <boost/spirit/include/qi.hpp>

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;

template<typename Iterator>
class ElementHideRule : public qi::grammar<Iterator, VariantRule()>
{
public:
    ElementHideRule();
    ~ElementHideRule();

private:
    qi::rule<Iterator, VariantRule()> start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_ELEMENT_HIDE_RULE_HPP
