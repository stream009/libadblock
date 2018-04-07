#include "filter_pattern.hpp"

#include "make_shared.hpp"
#include "parser/filter_option.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace adblock { namespace parser {

namespace phx = boost::phoenix;

struct FilterPattern::Impl
{
    rule<std::shared_ptr<Pattern>()>
        pattern, regex_pattern, domain_match_pattern, basic_match_pattern;

    rule<> options, end_of_pattern;

    FilterOption option;

    rule<StringRange()> pattern_string, domain_string;
    rule<char()> url_char, pattern_char, domain_char, utf8_char;

    Impl()
    {
        using qi::_val;
        using qi::_1;
        using qi::_2;
        using qi::_3;
        using qi::char_;
        using qi::raw;
        using phx::static_cast_;

        pattern = regex_pattern
                | domain_match_pattern
                | basic_match_pattern;

        basic_match_pattern =
            (
                   -char_('|')
                >> pattern_string
                >> -(char_('|'))
            )
            [
                _val = phx::make_shared<BasicMatchPattern>(
                    _2,
                    static_cast_<bool>(_1),
                    static_cast_<bool>(_3)
                )
            ];

        domain_match_pattern =
            ("||" >> domain_string >> pattern_string >> -(char_('|')))
            [
                _val = phx::make_shared<DomainMatchPattern>(
                    _1, _2,
                    static_cast_<bool>(_3)
                )
            ];

        regex_pattern =
            (   '/'
              >> raw
                 [
                   *(
                         ~char_('/')
                       | (char_('/') >> !&(end_of_pattern))
                    )
                 ]
              >> '/'
            )
            [
                _val = phx::make_shared<RegexPattern>(_1)
            ];

        end_of_pattern
            = qi::eoi | options >> qi::eoi;

        options
            = '$' >> option % ',';

        pattern_string
            = raw
              [
                *(
                      (pattern_char - '|')
                    | (char_('|') >> !&(end_of_pattern))
                 )
              ];

        domain_string
            = raw
              [
                +domain_char
              ];

        pattern_char
            = url_char | char_("^*");

        //url_char = qi::alnum | char_("%~&/:$#=_,."); //TODO think through
        url_char
            = (qi::graph - '$') | utf8_char;

        domain_char
            = qi::alnum | char_("-.") | utf8_char;

        utf8_char
            = char_ - qi::print - qi::cntrl;
    }
};

FilterPattern::
FilterPattern()
    : FilterPattern::base_type { m_start },
      m_impl { new Impl }
{
    m_start = m_impl->pattern;
}

FilterPattern::~FilterPattern() = default;

}} // namespace adblock::parser
