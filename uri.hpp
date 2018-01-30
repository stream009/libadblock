#ifndef ADBLOCK_URI_HPP
#define ADBLOCK_URI_HPP

#include <stdexcept>
#include <string>
#include <string_view>
#include <iosfwd>

#include <boost/range/iterator_range.hpp>

namespace adblock {

class Uri
{
public:
    using value_type = std::string::value_type;
    using size_type = std::string::size_type;
    using const_reference = std::string::const_reference;
    using const_pointer = std::string::const_pointer;
    using const_iterator = std::string::const_iterator;

    class ParseError;
public:
    // constructors
    Uri(std::string const&);
    Uri(std::string&&);

    Uri(const_pointer const b, const_pointer const e)
        : Uri { std::string(b, e) }
    {}

    Uri(const_iterator const b, const_iterator const e)
        : Uri { std::string(b, e) }
    {}

    Uri(Uri const&) = default;
    Uri(Uri&&) = default;

    Uri& operator=(Uri const&) = default;
    Uri& operator=(Uri&) = default;

    ~Uri() = default;

    // accessor
    const_reference at(size_type pos) const { return m_value.at(pos); }
    const_reference operator[](size_type pos) const { return m_value[pos]; }

    const_pointer c_str() const { return m_value.c_str(); }

    // query
    const_reference front() const { return m_value.front(); }
    const_reference back() const { return m_value.back(); }
    size_type size() const { return m_value.size(); }

    const_iterator begin() const { return m_value.begin(); }
    const_iterator end() const { return m_value.end(); }

    bool is_valid() const { return true; }

    std::string_view schema() const { return component(Field::Schema); }
    std::string_view host() const { return component(Field::Host); }
    std::string_view port() const { return component(Field::Port); }
    std::string_view path() const { return component(Field::Path); }
    std::string_view query() const { return component(Field::Query); }
    std::string_view fragment() const { return component(Field::Fragment); }
    std::string_view userInfo() const { return component(Field::UserInfo); }

private:
    enum Field {
        Schema = 0, Host = 1, Port = 2, Path = 3, Query = 4, Fragment = 5,
        UserInfo = 6, Max = 7
    };

    void parse();
    std::string_view component(Field) const;

    friend std::ostream& operator<<(std::ostream& os, Uri const& url)
    {
        os << url.m_value;
        return os;
    }

private:
    std::string m_value;

    uint16_t m_fieldSet;
    uint16_t m_port;

    struct {
        uint16_t offset;
        uint16_t length;
    } m_fieldData[Field::Max];
};

class Uri::ParseError : public std::runtime_error
{
public:
    ParseError(std::string_view const uri)
        : std::runtime_error { "URI parse error" }
        , m_uri { uri }
    {}

    std::string_view uri() const { return m_uri; }

private:
    std::string m_uri;
};

} // namespace adblock

#endif // ADBLOCK_URI_HPP
