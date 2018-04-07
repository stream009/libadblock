#include "comment_rule.hpp"

#include <ostream>

namespace adblock {

CommentRule::
CommentRule(const StringRange &comment)
    : m_comment { comment }
{}

void CommentRule::
print(std::ostream &os) const
{
    os << m_comment;
}

} // namespace adblock
