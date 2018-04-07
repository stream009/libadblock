#ifndef PARSER_DOMAIN_HPP
#define PARSER_DOMAIN_HPP

#include "type.hpp"

#include <memory>

namespace adblock { namespace parser {

class Domain : public grammar<StringRange()>
{
public:
    Domain();
    ~Domain();

private:
    rule<StringRange()> start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_DOMAIN_HPP
