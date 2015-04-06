#ifndef COMMENT_RULE_HPP
#define COMMENT_RULE_HPP

#include "rule.hpp"

#include <iosfwd>
#include <memory>

class CommentRule : public Rule
{
    using Base = Rule;
public:
    using Base::String;

public:
    CommentRule(const String &comment);

    // @override Rule
    void print(std::ostream&) const override;

private:
    String m_comment;
};

#include <utility>

#endif // COMMENT_RULE_HPP
