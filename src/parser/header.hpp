#ifndef ADBLOCK_PARSER_HEADER_HPP
#define ADBLOCK_PARSER_HEADER_HPP

#include "type.hpp"

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;

struct Header : grammar<StringRange(), qi::ascii::space_type>
{
public:
    Header();

private:
    rule<StringRange(), qi::ascii::space_type> start;
};

}} // namespace adblock::parser

#endif // ADBLOCK_PARSER_HEADER_HPP
