#ifndef PARSER_TYPE_HPP
#define PARSER_TYPE_HPP

#include "../type.hpp"

#include <boost/spirit/include/qi.hpp>

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;

template<typename A1 = qi::unused_type,
         typename A2 = qi::unused_type,
         typename A3 = qi::unused_type,
         typename A4 = qi::unused_type>
using grammar = qi::grammar<StringIterator, A1, A2, A3, A4>;

template<typename A1 = qi::unused_type,
         typename A2 = qi::unused_type,
         typename A3 = qi::unused_type,
         typename A4 = qi::unused_type>
using rule = qi::rule<StringIterator, A1, A2, A3, A4>;

}} // namespace adblock::parser

#endif // PARSER_TYPE_HPP
