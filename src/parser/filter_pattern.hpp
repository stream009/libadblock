#ifndef PARSER_FILTER_PATTERN_HPP
#define PARSER_FILTER_PATTERN_HPP

#include "type.hpp"
#include "pattern/pattern.hpp"

#include <memory>

namespace adblock { namespace parser {

class FilterPattern
    : public grammar<std::shared_ptr<Pattern>()>
{
public:
    FilterPattern();
    ~FilterPattern();

private:
    rule<std::shared_ptr<Pattern>()> m_start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_FILTER_PATTERN_HPP
