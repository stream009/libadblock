#ifndef PARSER_DOMAIN_HPP
#define PARSER_DOMAIN_HPP

#include <memory>
#include <string>

#include <boost/spirit/include/qi.hpp>

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;

template<typename Iterator>
class Domain : public qi::grammar<Iterator, std::string()>
{
public:
    Domain();
    ~Domain();

private:
    qi::rule<Iterator, std::string()> start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_DOMAIN_HPP
