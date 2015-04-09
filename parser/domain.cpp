#include "domain.hpp"

namespace adblock { namespace parser {

struct Domain::Impl
{
    Impl()
    {
        namespace qi = boost::spirit::qi;
        //const auto &letter = qi::alpha;

        // Domain name specification from RFC 1035
        subdomain
            = qi::raw
              [
                label >> +(qi::char_('.') >> label)
              ];

        // RFC1035 says label must start with letter but
        // RFC1123 relux this rule and allow number on first character.
        label
            = qi::raw
              [
                let_dig >> -(label_body | let_dig)
              ];

        label_body
            = qi::raw
              [
                let_dig_hyp >> (label_body | let_dig)
              ];

        let_dig_hyp = let_dig | qi::char_('-');
        let_dig     = letter | qi::digit;
        letter      = qi::alpha | utf8;
        utf8        = qi::char_ - qi::print - qi::cntrl;
#if 0
        BOOST_SPIRIT_DEBUG_NODE(subdomain);
        BOOST_SPIRIT_DEBUG_NODE(label);
        BOOST_SPIRIT_DEBUG_NODE(let_dig_hyp);
        BOOST_SPIRIT_DEBUG_NODE(let_dig);
        BOOST_SPIRIT_DEBUG_NODE(letter);
        BOOST_SPIRIT_DEBUG_NODE(utf8);
#endif
    }

    rule<StringRange()>
        subdomain, label, label_body;
    rule<char()>
        let_dig, let_dig_hyp, letter, utf8;
};

Domain::
Domain()
: Domain::base_type { start }, m_impl { new Impl }
{
    start = m_impl->subdomain;
}

Domain::~Domain() = default;

}} // namespace adblock::parser
