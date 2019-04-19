#ifndef ADBLOCK_TEST_PARSE_RULE_HPP
#define ADBLOCK_TEST_PARSE_RULE_HPP

#include "core/type.hpp"
#include "parser/parser.hpp"

#include <memory>

namespace adblock {

template<typename T>
std::unique_ptr<T>
parse_rule(StringRange const s)
{
    auto r = parser::parse(s);

    if (dynamic_cast<T*>(r.get())) {
        return std::unique_ptr<T>(dynamic_cast<T*>(r.release()));
    }
    else {
        return nullptr;
    }
}

} // namespace adblock

#endif // ADBLOCK_TEST_PARSE_RULE_HPP
