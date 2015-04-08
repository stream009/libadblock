#include "element_hide_rule.hpp"

#include "make_shared.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

struct ElementHideRule::Impl
{
    rule<std::shared_ptr<Rule>()>
        element_hide_rule, basic_element_hide_rule,
        exception_element_hide_rule;

    rule<StringRange()> domain, include_domain, exclude_domain;;
    rule<std::vector<StringRange>()> domains;

    rule<StringRange()> css_selector;

    Domain subdomain;

    Impl()
    {
        element_hide_rule = basic_element_hide_rule
                          | exception_element_hide_rule;

        basic_element_hide_rule =
            (-domains >> "##" >> css_selector)
            [
                qi::_val =
                    phx::make_shared<BasicElementHideRule>(qi::_2, qi::_1)
            ];
        exception_element_hide_rule =
            (-domains >> "#@#" >> css_selector)
            [
                qi::_val =
                    phx::make_shared<ExceptionElementHideRule>(qi::_2, qi::_1)
            ];

        domains = domain % ',';

        domain = include_domain | exclude_domain;

        include_domain = subdomain;
        exclude_domain
            = qi::raw
              [
                qi::char_('~') >> subdomain
              ];

        css_selector
            = qi::raw
              [
                +qi::char_
              ];
    }
};

ElementHideRule::
ElementHideRule()
    : ElementHideRule::base_type(start),
      m_impl { new Impl }
{
    start = m_impl->element_hide_rule;
}

ElementHideRule::~ElementHideRule() = default;

}} // namespace adblock::parser
