#include "element_hide_rule.hpp"

#include <boost/phoenix.hpp>

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

template<typename Iterator>
struct ElementHideRule<Iterator>::Impl
{
    Impl()
    {
        element_hide_rule = basic_element_hide_rule
                          | exception_element_hide_rule;

        basic_element_hide_rule = (-domains >> "##" >> css_selector)
                [qi::_val = phx::construct<BasicElementHideRule>(qi::_2)];
        exception_element_hide_rule = (-domains >> "#@#" >> css_selector)
                [qi::_val = phx::construct<ExceptionElementHideRule>(qi::_2)];

        domains = domain % ',';

        domain = include_domain | exclude_domain;

        include_domain = subdomain;
        exclude_domain = '~' >> subdomain;

        css_selector = +qi::char_;

        BOOST_SPIRIT_DEBUG_NODE(domains);
    }

    qi::rule<Iterator, VariantRule()> element_hide_rule;

    qi::rule<Iterator, BasicElementHideRule()> basic_element_hide_rule;
    qi::rule<Iterator, ExceptionElementHideRule> exception_element_hide_rule;

    qi::rule<Iterator, std::string()>
        css_selector, domain, include_domain, exclude_domain;

    qi::rule<Iterator, std::vector<std::string>()> domains;

    Domain<Iterator> subdomain;
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
