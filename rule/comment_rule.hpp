#ifndef COMMENT_RULE_HPP
#define COMMENT_RULE_HPP

#include "rule.hpp"

#include <memory>

class CommentRule : public Rule
{
    using Base = Rule;
public:
    using Base::String;

public:
    CommentRule(const String &comment);

    template<typename Str>
    static bool matchFormat(Str&&);

    friend std::ostream &operator<<(std::ostream&, const CommentRule&);

private:
    String m_comment;
};

#include <utility>

template<typename Str>
bool CommentRule::
matchFormat(Str &&line)
{
    return !line.empty() && line.front() == '!';
}

#endif // COMMENT_RULE_HPP
