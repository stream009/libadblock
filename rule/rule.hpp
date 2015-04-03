#ifndef RULE_HPP
#define RULE_HPP

#include <string>

#include <boost/network/uri/uri.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/iterator_range.hpp>

class Rule// : boost::noncopyable
{
public:
    using String = std::string;
    using StringRange = boost::iterator_range<String::const_iterator>;
    using Uri = boost::network::uri::uri;

public:
    Rule() = default;
    Rule(const std::string&);

    virtual ~Rule() = default;

    Rule(const Rule&) = default;
    Rule &operator=(const Rule&) = default;

    const String &line() const { return m_line; }

protected:
#if 0
    template<typename Str>
    Rule(Str&&);
#endif

private:
    String m_line;
};

inline Rule::
Rule(const std::string &line)
    : m_line { line }
{}

#include <utility>
#if 0
template<typename Str>
inline Rule::
Rule(Str &&line)
    : m_line { std::forward<Str>(line) }
{}
#endif
#endif // RULE_HPP
