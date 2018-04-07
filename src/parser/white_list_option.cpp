#include "white_list_option.hpp"

#include "make_shared.hpp"
#include "option.hpp"

#include "option/document_option.hpp"
#include "option/elem_hide_option.hpp"
#include "option/generic_hide_option.hpp"
#include "option/generic_block_option.hpp"

#include <boost/spirit/include/phoenix.hpp>

namespace adblock { namespace parser {

namespace phx = boost::phoenix;

struct WhiteListOption::Impl
{
    Impl()
    {
        using qi::_1;
        using qi::_val;
        using qi::lit;
        using phx::make_shared;
        using phx::static_cast_;

        white_list_option
            =   document_option
              | elemhide_option
              | generic_hide_option
              | generic_block_option
              ;

        const auto &inverse = qi::char_('~');

        document_option =
            (-inverse >> "document")
            [ _val = make_shared<DocumentOption>(static_cast_<bool>(_1)) ];
        elemhide_option =
            (-inverse >> "elemhide")
            [ _val = make_shared<ElemHideOption>(static_cast_<bool>(_1)) ];
        generic_hide_option =
            lit("generichide")
            [ _val = make_shared<GenericHideOption>() ];
        generic_block_option =
            lit("genericblock")
            [ _val = make_shared<GenericBlockOption>() ];
    }

    rule<std::shared_ptr<Option>()>
        white_list_option, document_option, elemhide_option,
        generic_hide_option, generic_block_option;
};

WhiteListOption::
WhiteListOption()
    : WhiteListOption::base_type { m_start },
      m_impl { new Impl }
{
    m_start = m_impl->white_list_option;
}

WhiteListOption::~WhiteListOption() = default;

}} // namespace adblock::parser
