#include "parser.hpp"

#include "grammar.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace adblock { namespace parser {

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
        std::cout << "Couldn't parse: " << line << "\n";
    }
    if (begin != end) {
        std::cout << "Partial match: " << line << " [";
        std::copy(begin, end, std::ostream_iterator<char>(std::cout));
        std::cout << "]\n";
    }

    return rule;
}

}} // namespace adblock::parser
