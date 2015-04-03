#ifndef PARSER_FILTER_RULE_HPP
#define PARSER_FILTER_RULE_HPP

#include "domain.hpp"

#include "rule/rule.hpp"

#include <memory>

#include <boost/spirit/include/qi.hpp>

namespace adblock { namespace parser {

namespace qi = boost::spirit::qi;

template<typename Iterator>
struct FilterRule : qi::grammar<Iterator, std::shared_ptr<Rule>()>
{
    FilterRule();

    qi::rule<Iterator, std::shared_ptr<Rule>()> filter_rule;

    qi::rule<Iterator, std::shared_ptr<Rule>()> basic_filter_rule;
    qi::rule<Iterator, std::shared_ptr<Rule>()> exception_filter_rule;

    qi::rule<Iterator, std::string()>
        pattern, basic_match_pattern, begin_match_pattern,
        end_match_pattern, domain_match_pattern, regex_pattern,
        url_char;

    qi::rule<Iterator>
        options, option, inversible_option,
        domain_option, sitekey_option, match_case_option,
        donottrack_option, script_option, image_option,
        stylesheet_option, object_option, xmlhttprequest_option,
        object_subrequest_option, subdocument_option,
        document_option, elemhide_option, other_option,
        third_party_option, collapse_option, domain_option_domain,
        base64_public_key, domains, css_selector,
        domain, include_domain, exclude_domain;

    Domain<Iterator> subdomain;
};

}} // namespace adblock::parser

#endif // PARSER_FILTER_RULE_HPP
