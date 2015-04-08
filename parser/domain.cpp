#include "domain.hpp"

namespace adblock { namespace parser {

struct Domain::Impl
{
    Impl()
    {
        namespace qi = boost::spirit::qi;
        const auto &letter = qi::alpha;

        // Domain name specification from RFC 1035
        subdomain
            = qi::raw
              [
                label >> +(qi::char_('.') >> label)
              ];

        label
            = qi::raw
              [
                letter >> -(label_body >> let_dig)
              ];

        label_body
            = qi::raw
              [
                *(let_dig_hyp >> &let_dig)
              ];

        let_dig_hyp = let_dig | '-';
        let_dig     = letter | qi::digit;

        BOOST_SPIRIT_DEBUG_NODE(subdomain);
        BOOST_SPIRIT_DEBUG_NODE(label);
#if 0
        BOOST_SPIRIT_DEBUG_NODE(let_dig_hyp);
        BOOST_SPIRIT_DEBUG_NODE(let_dig);
#endif
    }

    rule<StringRange()>
        subdomain, label, label_body;
    rule<char()>
        let_dig, let_dig_hyp;
};

Domain::
Domain()
: Domain::base_type { start }, m_impl { new Impl }
{
    start = m_impl->subdomain;
}

Domain::~Domain() = default;

}} // namespace adblock::parser
