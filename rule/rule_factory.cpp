#include "rule_factory.hpp"

#include <boost/xpressive/regex_actions.hpp>
#include <boost/xpressive/xpressive.hpp>

namespace xp = boost::xpressive;

struct emplace_back_impl {
    using result_type = void;

    template<typename Seq, typename... Args>
    void operator()(Seq &seq, Args&&... args) const {
        seq.emplace_back(std::forward<Args>(args)...);
    }
};
//static xp::function<emplace_back_impl>::type const emplace_back = {{}};

static const xp::sregex &
subdomain()
{
    namespace xp = boost::xpressive;

    static xp::sregex letter, digit, let_dig, let_dig_hyp,
                      ldh_str, label, subdomain;

    if (subdomain.regex_id() != 0) return subdomain;

    // Domain name specification from RFC 1035
    letter      = xp::alpha;
    digit       = xp::digit;
    let_dig     = letter | digit;
    let_dig_hyp = let_dig | "-";
    ldh_str     = +let_dig_hyp;
    label       = letter >> !(!ldh_str >> let_dig);
    subdomain   = label >> *("." >> label);
    // static const xp::sregex domain = subdomain | " ";

    return subdomain;
}

//template<typename Str>
//std::unique_ptr<Rule> RuleFactory::
//parse(Str &&line)
void parse()
{
    namespace xp = boost::xpressive;

    const auto &subdomain = ::subdomain();

    static xp::sregex rule,
        comment_rule, basic_filter_rule, exception_filter_rule,
        pattern, basic_match_pattern, begin_match_pattern,
        end_match_pattern, domain_match_pattern, regex_pattern,
        url_char, options, option, inversible_option,
        domain_option, sitekey_option, match_case_option,
        donottrack_option, script_option, image_option,
        stylesheet_option, object_option, xmlhttprequest_option,
        object_subrequest_option, subdocument_option,
        document_option, elemhide_option, other_option,
        third_party_option, collapse_option, domain_option_domain,
        base64_public_key, basic_element_hide_rule,
        exception_element_hide_rule, domains, css_selector,
        domain, include_domain, exclude_domain;

    url_char = xp::alnum | (xp::set= '/', ':', '%', '#', '=', '_', ',');

    domain_option_domain = !xp::as_xpr('~') >> subdomain;
    base64_public_key = +(xp::alnum | (xp::set= '/', '+', '='));

    script_option            = xp::as_xpr("script");
    image_option             = xp::as_xpr("image");
    stylesheet_option        = xp::as_xpr("stylesheet");
    object_option            = xp::as_xpr("object");
    xmlhttprequest_option    = xp::as_xpr("xmlhttprequest");
    object_subrequest_option = xp::as_xpr("object-subrequest");
    subdocument_option       = xp::as_xpr("subdocument");
    document_option          = xp::as_xpr("document");
    elemhide_option          = xp::as_xpr("elemehide");
    other_option             = xp::as_xpr("other");
    third_party_option       = xp::as_xpr("third-party");
    domain_option            = "domain=" >> domain_option_domain
                                         >> *('|' >> domain_option_domain);
    sitekey_option           = "sitekey=" >> base64_public_key;
    match_case_option        = xp::as_xpr("match-case");
    collapse_option          = xp::as_xpr("collapse");
    donottrack_option        = xp::as_xpr("donottrack");

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

    option = (!xp::as_xpr('~') >> inversible_option)
           | domain_option
           | sitekey_option
           | match_case_option
           | donottrack_option;

    options = +option;

    include_domain = subdomain;
    exclude_domain = '~' >> subdomain;
    domain = include_domain | exclude_domain;
    domains = domain >> *(',' >> domain);

    css_selector = +xp::_;

    basic_match_pattern = +(url_char | (xp::set= '^', '*'));
    begin_match_pattern = "|" >> basic_match_pattern;
    end_match_pattern = basic_match_pattern >> "|";
    domain_match_pattern = "||" >> subdomain;
    regex_pattern = '/' >> -+(~xp::as_xpr('/')) >> '/';

    pattern = basic_match_pattern
            | begin_match_pattern
            | end_match_pattern
            | domain_match_pattern
            | regex_pattern;

    comment_rule = "!" >> *xp::_;

    basic_filter_rule = pattern >> !("$" >> options);
    exception_filter_rule = "@@" >> basic_filter_rule;

    basic_element_hide_rule = !domains >> "##" >> css_selector;
    exception_element_hide_rule = !domains >> "#@#" >> css_selector;

    rule = comment_rule
         | basic_element_hide_rule
         | exception_element_hide_rule
         | exception_filter_rule
         | basic_filter_rule;
}

