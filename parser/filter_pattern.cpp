#include "filter_pattern.hpp"

#include "make_shared.hpp"
#include "parser/domain.hpp"
#include "parser/filter_option.hpp"
#include "pattern/basic_match_pattern.hpp"
#include "pattern/begin_match_pattern.hpp"
#include "pattern/domain_match_pattern.hpp"
#include "pattern/end_match_pattern.hpp"
#include "pattern/regex_pattern.hpp"

#include <boost/spirit/include/phoenix.hpp>

namespace adblock { namespace parser {

namespace phx = boost::phoenix;

struct FilterPattern::Impl
{
    rule<std::shared_ptr<Pattern>()>
        pattern, regex_pattern, domain_match_pattern, begin_match_pattern,
        end_match_pattern, basic_match_pattern;

    rule<> options, end_of_pattern;

    Domain subdomain;
    FilterOption option;

    rule<StringRange()> pattern_string, domain_string;
    rule<char()> url_char, pattern_char, domain_char, utf8_char;

    Impl()
    {
        using qi::_val;
        using qi::_1;
        using qi::_2;

        pattern = regex_pattern
                | domain_match_pattern
                | begin_match_pattern
                | end_match_pattern
                | basic_match_pattern;

        basic_match_pattern =
            pattern_string
            [
                _val = phx::make_shared<BasicMatchPattern>(_1)
            ];

        begin_match_pattern =
            ('|' >> pattern_string)
            [
                _val = phx::make_shared<BeginMatchPattern>(_1)
            ];

        end_match_pattern =
            qi::raw
            [
                +(pattern_char - '|') >> '|' >> &end_of_pattern
            ]
            [
                _val = phx::make_shared<EndMatchPattern>(_1)
            ];

        domain_match_pattern =
            ("||" >> domain_string >> -pattern_string)
            [
                _val = phx::make_shared<DomainMatchPattern>(_1, _2)
            ];

        using qi::char_;

        regex_pattern =
            qi::raw
            [
                    '/'
                 >> *(
                          ~char_('/')
                        | (char_('/') >> !&(end_of_pattern))
                     )
                 >> '/'
            ]
            [
                _val = phx::make_shared<RegexPattern>(_1)
            ];

        end_of_pattern
            = qi::eoi | options >> qi::eoi;

        options
            = '$' >> option % ',';

        pattern_string
            = qi::raw
              [
                +pattern_char
              ];

        domain_string
            = qi::raw
              [
                +domain_char
              ];

        pattern_char
            = url_char | char_("^*");

        //url_char = qi::alnum | char_("%~&/:$#=_,."); //TODO think through
        url_char
            = qi::graph - '$' | utf8_char;

        domain_char
            = qi::alnum | char_('-') | utf8_char;

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
