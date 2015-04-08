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

    rule<StringRange()> pattern_string;
    rule<char()> url_char;

    Impl()
    {
        pattern = regex_pattern
                | domain_match_pattern
                | begin_match_pattern
                | end_match_pattern
                | basic_match_pattern;

        basic_match_pattern =
            pattern_string
            [
                qi::_val = phx::make_shared<BasicMatchPattern>(qi::_1)
            ];

        begin_match_pattern =
            ("|" >> pattern_string)
            [
                qi::_val = phx::make_shared<BeginMatchPattern>(qi::_1)
            ];

        end_match_pattern =
            (pattern_string >> "|")
            [
                qi::_val = phx::make_shared<EndMatchPattern>(qi::_1)
            ];

        domain_match_pattern =
            ("||" >> subdomain)
            [
                qi::_val = phx::make_shared<DomainMatchPattern>(qi::_1)
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
                qi::_val = phx::make_shared<RegexPattern>(qi::_1)
            ];

        end_of_pattern
            = qi::eoi | options >> qi::eoi;

        options
            = '$' >> option % ',';

        pattern_string
            = qi::raw
              [
                +(url_char | char_("^*"))
              ];

        //url_char = qi::alnum | char_("%~&/:$#=_,."); //TODO think through
        url_char = qi::graph - '$';
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
