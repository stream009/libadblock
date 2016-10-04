#include "filter_option.hpp"

#include "make_shared.hpp"
#include "domain.hpp"
#include "option.hpp"

#include <boost/spirit/include/phoenix.hpp>

namespace adblock { namespace parser {

namespace phx = boost::phoenix;

struct FilterOption::Impl
{
    Impl()
    {
        using qi::_1;
        using qi::_val;
        using qi::lit;
        using phx::make_shared;
        using phx::static_cast_;

        option
            =   script_option
              | image_option
              | stylesheet_option
              | xmlhttprequest_option
              | object_subrequest_option
              | object_option
              | subdocument_option
              | other_option
              | third_party_option
              | collapse_option
              | domain_option
              | sitekey_option
              | match_case_option
              | donottrack_option
              | popup_option
              | media_option
              | font_option
              | websocket_option
              ;

        const auto &inverse = qi::char_('~');

        script_option =
            (-inverse >> "script")
            [ _val = make_shared<ScriptOption>(static_cast_<bool>(_1)) ];
        image_option =
            (-inverse >> "image")
            [ _val = make_shared<ImageOption>(static_cast_<bool>(_1)) ];
        stylesheet_option =
            (-inverse >> "stylesheet")
            [
                _val = make_shared<StyleSheetOption>(static_cast_<bool>(_1))
            ];
        object_option =
            (-inverse >> "object")
            [ _val = make_shared<ObjectOption>(static_cast_<bool>(_1)) ];
        xmlhttprequest_option =
            (-inverse >> "xmlhttprequest")
            [
                _val = make_shared<XmlHttpRequestOption>(
                                                static_cast_<bool>(_1))
            ];
        object_subrequest_option =
            (-inverse >> "object-subrequest")
            [
                _val = make_shared<ObjectSubRequestOption>(
                                                static_cast_<bool>(_1))
            ];
        subdocument_option =
            (-inverse >> "subdocument")
            [ _val = make_shared<SubDocumentOption>(static_cast_<bool>(_1)) ];
        other_option =
            (-inverse >> "other")
            [ _val = make_shared<OtherOption>(static_cast_<bool>(_1)) ];
        third_party_option =
            (-inverse >> "third-party")
            [ _val = make_shared<ThirdPartyOption>(static_cast_<bool>(_1)) ];
        domain_option =
            "domain=" >> (domain_option_domain % '|')
            [ _val = make_shared<DomainOption>(qi::_1) ];
        sitekey_option =
            "sitekey=" >> (base64_public_key % '|')
            [ _val = make_shared<SiteKeyOption>(qi::_1) ];
        match_case_option =
            lit("match-case") [ _val = make_shared<MatchCaseOption>() ];
        collapse_option =
            (-inverse >> "collapse")
            [ _val = make_shared<CollapseOption>(static_cast_<bool>(_1)) ];
        donottrack_option =
            lit("donottrack") [ _val = make_shared<DoNotTrackOption>() ];
        popup_option
            = lit("popup")
              [
                _val = make_shared<PopUpOption>()
              ];
        media_option
            = (-inverse >> "media")
              [
                _val = make_shared<MediaOption>(static_cast_<bool>(_1))
              ];
        font_option
            = (-inverse >> "font")
              [
                _val = make_shared<FontOption>(static_cast_<bool>(_1))
              ];
        websocket_option
            = (-inverse >> "websocket")
              [
                _val = make_shared<WebSocketOption>(static_cast_<bool>(_1))
              ];

        domain_option_domain
            = qi::raw
              [
                -qi::char_('~') >> subdomain
              ];

        base64_public_key
            = qi::raw
              [
                +(qi::alnum | qi::char_("/+="))
              ];
    }

    rule<std::shared_ptr<Option>()>
        option, script_option, media_option, font_option,
        domain_option, sitekey_option, match_case_option,
        donottrack_option, image_option, websocket_option,
        stylesheet_option, object_option, xmlhttprequest_option,
        object_subrequest_option, subdocument_option, other_option,
        third_party_option, collapse_option, popup_option;

    rule<StringRange()> domain_option_domain, base64_public_key;

    Domain subdomain;
};

FilterOption::
FilterOption()
    : FilterOption::base_type { m_start },
      m_impl { new Impl }
{
    m_start = m_impl->option;
}

FilterOption::~FilterOption() = default;

}} // namespace adblock::parser
