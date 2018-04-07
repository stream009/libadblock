#ifndef URI_URI_HPP
#define URI_URI_HPP

#include <iosfwd>
#include <string>
#include <string_view>

namespace uri {

class uri
{
public:
    using const_iterator = std::string::const_iterator;

public:
    uri(std::string);
    uri(char const* ptr, size_t len);
    ~uri() = default;

    // query
    std::string_view scheme() const;
    std::string_view user_info() const;
    std::string_view host() const;
    std::string_view port() const;
    std::string_view path() const;
    std::string_view query() const;
    std::string_view fragment() const;

    const_iterator begin() const { return m_str.begin(); }
    const_iterator end() const { return m_str.end(); }

    // operator
    bool operator==(uri const& rhs) const { return m_str == rhs.m_str; }

    friend std::ostream& operator<<(std::ostream& oss, uri const& u);

private:
    std::string m_str;
};

} // namespace uri

#endif // URI_URI_HPP
