#include "uri.hpp"

#include "parser.hpp"

#include <ostream>

namespace uri {

uri::
uri(std::string str)
    : m_str { std::move(str) }
{
    parse(m_str); // throws parse_error
}

uri::
uri(char const* const ptr, size_t const len)
    : uri { std::string(ptr, len) }
{}

std::string_view uri::
scheme() const
{
    return parse(m_str).scheme;
}

std::string_view uri::
user_info() const
{
    return parse(m_str).user_info;
}

std::string_view uri::
host() const
{
    return parse(m_str).host;
}

std::string_view uri::
port() const
{
    return parse(m_str).port;
}

std::string_view uri::
path() const
{
    return parse(m_str).path;
}

std::string_view uri::
query() const
{
    return parse(m_str).query;
}

std::string_view uri::
fragment() const
{
    return parse(m_str).fragment;
}

std::ostream&
operator<<(std::ostream& oss, uri const& u)
{
    oss << u.m_str;
    return oss;
}

} // namespace uri
