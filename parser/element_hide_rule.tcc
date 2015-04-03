#include "element_hide_rule.hpp"

#include "make_shared.hpp"
#include "rule/basic_element_hide_rule.hpp"
#include "rule/exception_element_hide_rule.hpp"

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

template<typename Iterator>
struct ElementHideRule<Iterator>::Impl
{
    using IncludeDomain = ::ElementHideRule::IncludeDomain;
    using ExcludeDomain = ::ElementHideRule::ExcludeDomain;
    using DomainT = ::ElementHideRule::Domain;

    qi::rule<Iterator, std::shared_ptr<Rule>()>
        element_hide_rule, basic_element_hide_rule,
        exception_element_hide_rule;

    qi::rule<Iterator, IncludeDomain()> include_domain;
    qi::rule<Iterator, ExcludeDomain()> exclude_domain;
    qi::rule<Iterator, DomainT()> domain;
    qi::rule<Iterator, std::vector<DomainT>()> domains;

    qi::rule<Iterator, std::string()> css_selector;

    Domain<Iterator> subdomain;

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

template<typename Iterator>
inline ElementHideRule<Iterator>::
ElementHideRule()
    : ElementHideRule::base_type(start),
      m_impl { new Impl }
{
    start = m_impl->element_hide_rule;
}

template<typename Iterator>
inline ElementHideRule<Iterator>::~ElementHideRule() = default;

}} // namespace adblock::parser
