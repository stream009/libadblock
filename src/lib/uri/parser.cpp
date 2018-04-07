#include "parser.hpp"

#include <boost/format.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    uri::uri_components,
    scheme,
    user_info,
    host,
    port,
    path,
    query,
    fragment
)

namespace boost::spirit::x3::traits {

template<typename It>
void move_to(It const b, It const e, std::string_view& v)
{
    v = std::string_view(b, e - b);
}

} // namespace boost::spirit::x3::traits

#include <boost/spirit/home/x3.hpp>

namespace {

namespace grammar {

namespace x3 = boost::spirit::x3;

using x3::char_;
using x3::digit;
using x3::raw;
using x3::rule;

struct _;

auto fragment = raw[ *char_ ];

auto query = raw[ *(char_ - '#') ];

auto path = raw[ *(char_ - char_("?#")) ];

auto port = raw[ *digit ];

auto host = raw[ *(char_ - char_(":/?#")) ];

auto user_info = raw[ *(char_ - char_("@/?#")) ];

auto scheme = raw [ *(char_ - char_(":@/?#")) ];

auto uri =
       scheme >> ':'
    >> -( "//"
        >> -(&(user_info >> '@') >> user_info >> '@')
        >> host
        >> -(':' >> port)
    )
    >> path
    >> -('?' >> query)
    >> -('#' >> fragment)
    ;

} // namespace grammar

} // unnamed namespace

namespace uri {

uri_components
parse(std::string_view const input)
{
    namespace x3 = boost::spirit::x3;

    auto it = input.data();
    auto const end = it + input.size();

    uri_components ast;
    auto const success = x3::parse(it, end, grammar::uri, ast);

    if (!success || it != end) {
        throw parse_error { input, it - input.data() };
    }

    return ast;
}

// parse_error

parse_error::
parse_error(std::string_view const uri,
            non_negative<int64_t> const location)
    : m_uri { uri }
    , m_location { location }
{}

parse_error::~parse_error() = default;

char const* parse_error::
what() const noexcept
{
    static boost::format fmt { "URI parse error at %d of '%s'" };

    m_message = str(fmt % m_location % m_uri);

    return m_message.c_str();
}

} // namespace uri
