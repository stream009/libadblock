#include "filter_option.hpp"

#include "make_shared.hpp"
#include "domain.hpp"
#include "option.hpp"

namespace phx = boost::phoenix;

namespace adblock { namespace parser {

template<typename Iterator>
struct FilterOption<Iterator>::Impl
{
    Impl()
    {
        option =
            ((-qi::char_('~') >> inversible_option)
               | domain_option
               | sitekey_option
               | match_case_option
               | donottrack_option)
            [
                qi::_val = phx::make_shared<Option>()
            ];

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
    }

    template<typename Attr>
    using rule = qi::rule<Iterator, Attr>;

    rule<std::shared_ptr<Option>()> option;

    qi::rule<Iterator>
        inversible_option,
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


template<typename Iterator>
inline FilterOption<Iterator>::
FilterOption()
    : FilterOption::base_type { m_start },
      m_impl { new Impl }
{
    m_start = m_impl->option;
}

template<typename Iterator>
inline FilterOption<Iterator>::~FilterOption() = default;

}} // namespace adblock::parser
