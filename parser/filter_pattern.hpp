#ifndef PARSER_FILTER_PATTERN_HPP
#define PARSER_FILTER_PATTERN_HPP

#include "pattern/pattern.hpp"

#include <memory>

#include <boost/spirit/include/qi.hpp>

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;

template<typename Iterator>
class FilterPattern
    : public qi::grammar<Iterator, std::shared_ptr<Pattern>()>
{
public:
    FilterPattern();
    ~FilterPattern();

private:
    qi::rule<Iterator, std::shared_ptr<Pattern>()> m_start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_FILTER_PATTERN_HPP
