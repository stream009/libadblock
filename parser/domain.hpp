#ifndef PARSER_DOMAIN_HPP
#define PARSER_DOMAIN_HPP

#include "type.hpp"

#include <memory>
#include <string>

namespace adblock { namespace parser {

class Domain : public grammar<std::string()>
{
public:
    Domain();
    ~Domain();

private:
    rule<std::string()> start;

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}} // namespace adblock::parser

#endif // PARSER_DOMAIN_HPP
