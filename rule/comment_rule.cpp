#include "comment_rule.hpp"

#include <ostream>

CommentRule::
CommentRule(const String &comment)
    : m_comment { comment }
{}

void CommentRule::
print(std::ostream &os) const
{
    os << m_comment;
}
