#ifndef URI_PARSER_HPP
#define URI_PARSER_HPP

#include <exception>
#include <string>
#include <string_view>

namespace uri {

// type

struct uri_components
{
    std::string_view scheme;
    std::string_view user_info;
    std::string_view host;
    std::string_view port;
    std::string_view path;
    std::string_view query;
    std::string_view fragment;
};

// function

uri_components
    parse(std::string_view input);

// exception

template<typename T> using non_negative = T;

class parse_error : std::exception
{
public:
    parse_error(std::string_view uri, non_negative<int64_t> location);
    ~parse_error();

    // accessor
    std::string const& uri() const { return m_uri; }
    non_negative<int64_t> location() const { return m_location; }

    // override std::exception
    char const* what() const noexcept override;

private:
    std::string m_uri;
    mutable std::string m_message;
    non_negative<int64_t> m_location;

};

} // namespace uri

#endif // URI_PARSER_HPP
