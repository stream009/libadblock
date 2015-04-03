#include "comment_rule.hpp"

CommentRule::
CommentRule(const String &comment)
    : Base { "" }, m_comment { comment }
{}

std::ostream &
operator<<(std::ostream &os, const CommentRule &rule)
{
    return os << rule.m_comment;
}
