#include "filter_rule.hpp"

#include "filter_option.hpp"
#include "filter_pattern.hpp"
#include "make_shared.hpp"
#include "white_list_option.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"

#include <boost/spirit/include/phoenix.hpp>

namespace adblock { namespace parser {

namespace phx = boost::phoenix;

struct FilterRule::Impl
{
    Impl()
    {
        filter_rule = exception_filter_rule | basic_filter_rule;

        basic_filter_rule =
            (pattern >> -('$' >> filterOption % ','))
            [
                qi::_val =
                    phx::make_shared<BasicFilterRule>(qi::_1, qi::_2)
            ];

        exception_filter_rule =
            ("@@" >> pattern
                  >> -('$' >> (filterOption | whiteListOption) % ','))
            [
                qi::_val =
                    phx::make_shared<ExceptionFilterRule>(qi::_1, qi::_2)
            ];
    }

    rule<std::shared_ptr<Rule>()> filter_rule;

    rule<std::shared_ptr<Rule>()> basic_filter_rule;
    rule<std::shared_ptr<Rule>()> exception_filter_rule;

    FilterPattern pattern;
    FilterOption filterOption;
    WhiteListOption whiteListOption;
};

FilterRule::
FilterRule()
    : FilterRule::base_type(m_start),
      m_impl { new Impl }
{
    m_start = m_impl->filter_rule;
}

FilterRule::~FilterRule() = default;

}} // namespace adblock::parser
