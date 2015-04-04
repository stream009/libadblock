#include "filter_rule.hpp"

#include "filter_option.hpp"
#include "filter_pattern.hpp"
#include "make_shared.hpp"
#include "rule/basic_filter_rule.hpp"
#include "rule/exception_filter_rule.hpp"

namespace phx = boost::phoenix;

namespace adblock { namespace parser {

template<typename Iterator>
struct FilterRule<Iterator>::Impl
{
    Impl()
    {
        filter_rule = exception_filter_rule | basic_filter_rule;

        basic_filter_rule =
            (pattern >> -('$' >> options))
            [
                qi::_val =
                    phx::make_shared<BasicFilterRule>(qi::_1, qi::_2)
            ];

        exception_filter_rule =
            ("@@" >> pattern >> -('$' >> options))
            [
                qi::_val =
                    phx::make_shared<ExceptionFilterRule>(qi::_1, qi::_2)
            ];

        options = +option;
    }

    template<typename Attr>
    using rule = qi::rule<Iterator, Attr>;

    rule<std::shared_ptr<Rule>()> filter_rule;

    rule<std::shared_ptr<Rule>()> basic_filter_rule;
    rule<std::shared_ptr<Rule>()> exception_filter_rule;

    rule<std::vector<std::shared_ptr<Option>>()> options;

    FilterPattern<Iterator> pattern;
    FilterOption<Iterator> option;
};

template<typename Iterator>
inline FilterRule<Iterator>::
FilterRule()
    : FilterRule::base_type(m_start),
      m_impl { new Impl }
{
    m_start = m_impl->filter_rule;
}

template<typename Iterator>
inline FilterRule<Iterator>::~FilterRule() = default;

}} // namespace adblock::parser
