#include "grammar.hpp"

#include "make_shared.hpp"
#include "rule/comment_rule.hpp"

namespace adblock { namespace parser {

namespace phx = boost::phoenix;

template<typename Iterator>
inline Grammar<Iterator>::
Grammar()
    : Grammar::base_type(rule)
{
    rule = comment_rule
         | element_hide_rule
         | filter_rule;

    comment_rule = qi::as_string[qi::char_('!') >> *qi::char_]
                    [qi::_val = phx::make_shared<CommentRule>(qi::_1)];
}

}} // namespace adblock::parser
