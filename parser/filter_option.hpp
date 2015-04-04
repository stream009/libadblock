#ifndef PARSER_FILTER_OPTION_HPP
#define PARSER_FILTER_OPTION_HPP

#include <memory>

#include <boost/spirit/include/qi.hpp>

class Option;

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;

template<typename Iterator>
class FilterOption
    : public qi::grammar<Iterator, std::shared_ptr<Option>()>
{
public:
    FilterOption();
    ~FilterOption();

private:
    qi::rule<Iterator, std::shared_ptr<Option>()> m_start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_FILTER_OPTION_HPP
