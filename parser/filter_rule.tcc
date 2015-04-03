#include "filter_rule.hpp"

#include <boost/phoenix.hpp>

namespace phx = boost::phoenix;

namespace adblock { namespace parser {

template<typename Iterator>
inline FilterRule<Iterator>::
FilterRule()
    : FilterRule::base_type(filter_rule)
{
    filter_rule = exception_filter_rule | basic_filter_rule;

    basic_filter_rule = (pattern >> -("$" >> options))
                [qi::_val = phx::construct<BasicFilterRule>(qi::_1)];
    exception_filter_rule = ("@@" >> pattern >> -('$' >> options))
                [qi::_val = phx::construct<ExceptionFilterRule>(qi::_1)];

    pattern = regex_pattern
            | domain_match_pattern
            | begin_match_pattern
            | end_match_pattern
            | basic_match_pattern;

    basic_match_pattern = +(url_char | qi::char_("^*"));
    begin_match_pattern = "|" >> basic_match_pattern;
    end_match_pattern = basic_match_pattern >> "|";
    domain_match_pattern = "||" >> subdomain;
    regex_pattern = '/' >> -+(qi::char_ - '/') >> '/';

    options = +option;

    option = (-qi::char_('~') >> inversible_option)
           | domain_option
           | sitekey_option
           | match_case_option
           | donottrack_option;

    inversible_option = script_option
                      | image_option
                      | stylesheet_option
                      | object_option
                      | xmlhttprequest_option
                      | object_subrequest_option
                      | subdocument_option
                      | document_option
                      | elemhide_option
                      | other_option
                      | third_party_option
                      | collapse_option;

    script_option            = "script";
    image_option             = "image";
    stylesheet_option        = "stylesheet";
    object_option            = "object";
    xmlhttprequest_option    = "xmlhttprequest";
    object_subrequest_option = "object-subrequest";
    subdocument_option       = "subdocument";
    document_option          = "document";
    elemhide_option          = "elemehide";
    other_option             = "other";
    third_party_option       = "third-party";
    domain_option            = "domain=" >> domain_option_domain
                                         >> *('|' >> domain_option_domain);
    sitekey_option           = "sitekey=" >> base64_public_key;
    match_case_option        = "match-case";
    collapse_option          = "collapse";
    donottrack_option        = "donottrack";

    domain_option_domain = -qi::char_('~') >> subdomain;
    base64_public_key = +(qi::alnum | qi::char_("/+="));

    //url_char = qi::alnum | qi::char_("%~&/:$#=_,."); //TODO think through
    url_char = qi::graph - '$';
}

}} // namespace adblock::parser
