#ifndef PARSER_FILTER_OPTION_HPP
#define PARSER_FILTER_OPTION_HPP

#include "option.hpp"
#include "type.hpp"

#include <memory>

class Option;

namespace adblock { namespace parser {

class FilterOption
    : public grammar<std::shared_ptr<Option>()>
{
public:
    FilterOption();
    ~FilterOption();

private:
    rule<std::shared_ptr<Option>()> m_start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_FILTER_OPTION_HPP
