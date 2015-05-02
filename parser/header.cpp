#include "header.hpp"

#include <algorithm>
#include <iterator>

#include <boost/spirit/include/qi.hpp>

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

template<typename Iterator, typename Str>
void
printParseError(const Iterator begin, const Iterator end,
                const Iterator pos, const Str &message)
{
    std::cout << "Parse error: expected " << message << "\n";
    std::copy(begin, end, std::ostream_iterator<char>(std::cout));
    std::cout << "\n";

    const auto len = std::distance(begin, pos);
    assert(len >= 0);
    for (auto i = 0; i < len; ++i) {
        std::cout << " ";
    }
    std::cout << "^\n";
}

BOOST_PHOENIX_ADAPT_FUNCTION(
    void,
    printParseError,
    printParseError,
    4
)

Header::
Header()
    : base_type(start)
{
    start = qi::eps
          > "[Adblock Plus"
          > qi::raw[+qi::digit % '.']
          > "]" > qi::eoi;

    using namespace qi::labels;
    using phx::construct;
    using phx::val;

    qi::on_error<qi::fail>
    (
        start,
        printParseError(_1, _2, _3, _4)
    );
}

}} // namespace adblock::parser
