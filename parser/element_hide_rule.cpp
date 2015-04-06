#include "element_hide_rule.hpp"

#include "make_shared.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

struct ElementHideRule::Impl
{
    using IncludeDomain = ::ElementHideRule::IncludeDomain;
    using ExcludeDomain = ::ElementHideRule::ExcludeDomain;
    using DomainT = ::ElementHideRule::Domain;

    rule<std::shared_ptr<Rule>()>
        element_hide_rule, basic_element_hide_rule,
        exception_element_hide_rule;

    rule<IncludeDomain()> include_domain;
    rule<ExcludeDomain()> exclude_domain;
    rule<DomainT()> domain;
    rule<std::vector<DomainT>()> domains;

    rule<std::string()> css_selector;

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
        exclude_domain = '~' >> subdomain;

        css_selector = +qi::char_;

        //BOOST_SPIRIT_DEBUG_NODE(domains);
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
