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
    CommentRule() = default;
    CommentRule(const std::string &);

    CommentRule(const CommentRule&) = default;
    CommentRule &operator=(const CommentRule&) = default;
#if 0
    template<typename Str>
    CommentRule(Str&&);
#endif
    template<typename Str>
    static bool matchFormat(Str&&);
};

#include <utility>

#if 0
template<typename Str>
inline CommentRule::
CommentRule(Str &&line)
    : Base { std::forward<Str>(line) }
{}
#endif
inline CommentRule::
CommentRule(const std::string &line)
    : Base { line }
{}

template<typename Str>
bool CommentRule::
matchFormat(Str &&line)
{
    return !line.empty() && line.front() == '!';
}

#endif // COMMENT_RULE_HPP
