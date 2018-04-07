#ifndef PARSER_WHITE_LIST_OPTION_HPP
#define PARSER_WHITE_LIST_OPTION_HPP

#include "type.hpp"

#include <memory>

namespace adblock {
class Option;
} // namespace adblock

namespace adblock { namespace parser {

class WhiteListOption
    : public grammar<std::shared_ptr<Option>()>
{
public:
    WhiteListOption();
    ~WhiteListOption();

private:
    rule<std::shared_ptr<Option>()> m_start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_WHITE_LIST_OPTION_HPP
