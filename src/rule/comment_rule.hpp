#ifndef COMMENT_RULE_HPP
#define COMMENT_RULE_HPP

#include "rule.hpp"
#include "core/type.hpp"

#include <iosfwd>
#include <memory>

namespace adblock {

class CommentRule : public Rule
{
    using Base = Rule;
public:
    CommentRule(const StringRange &comment);

    // @override Rule
    void print(std::ostream&) const override;

private:
    StringRange m_comment;
};

} // namespace adblock

#endif // COMMENT_RULE_HPP
