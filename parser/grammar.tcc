#include "grammar.hpp"

#include <boost/phoenix.hpp>

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
                    [qi::_val = phx::construct<CommentRule>(qi::_1)];
}

}} // namespace adblock::parser
