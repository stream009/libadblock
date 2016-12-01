#include "parser.hpp"

#include "grammar.hpp"
#include "header.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace adblock { namespace parser {

StringRange
parseHeader(const StringRange &line)
{
    namespace qi = boost::spirit::qi;

    static const Header grammar;

    StringRange result;
    auto begin = line.begin();
    const auto end = line.end();
    qi::phrase_parse(begin, end, grammar, qi::ascii::space, result);

    return result;
}

std::shared_ptr<Rule>
parse(const StringRange &line)
{
    namespace qi = boost::spirit::qi;

    static const Grammar grammar;
    std::shared_ptr<Rule> rule;

    auto begin = line.begin();
    const auto end = line.end();
    const auto rv = qi::parse(begin, end, grammar, rule);
    if (!rv) { //TODO proper error reporting
        std::cout << "Couldn't parse filter rule: " << line << "\n";
        return nullptr;
    }
    if (begin != end) {
        std::cout << "Couldn't parse filter rule: " << line << " [";
        std::copy(begin, end, std::ostream_iterator<char>(std::cout));
        std::cout << "]\n";
        return nullptr;
    }

    return rule;
}

}} // namespace adblock::parser
