#ifndef ADBLOCK_TEST_PARSE_RULE_HPP
#define ADBLOCK_TEST_PARSE_RULE_HPP

#include "core/string_range.hpp"
#include "parser/parse_error.hpp"
#include "parser/parser.hpp"

#include <memory>

namespace adblock {

class parse_error : public std::exception
{
public:
    parse_error(std::vector<ParseError> errors)
        : m_errors { std::move(errors) }
    {}

    auto errors() const { return m_errors; }

private:
    std::vector<ParseError> m_errors;
};

template<typename T>
std::unique_ptr<T>
parse_rule(StringRange const s)
{
    std::vector<ParseError> errors;
    auto r = parser::parse(s, errors);

    if (!errors.empty()) {
        throw parse_error { std::move(errors) };
    }

    if (dynamic_cast<T*>(r.get())) {
        return std::unique_ptr<T>(dynamic_cast<T*>(r.release()));
    }
    else {
        return nullptr;
    }
}

} // namespace adblock

#endif // ADBLOCK_TEST_PARSE_RULE_HPP
