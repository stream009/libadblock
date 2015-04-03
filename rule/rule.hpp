#ifndef RULE_HPP
#define RULE_HPP

#include <string>

#include <boost/network/uri/uri.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/iterator_range.hpp>

class Rule : boost::noncopyable
{
public:
    using String = std::string;
    using StringRange = boost::iterator_range<String::const_iterator>;
    using Uri = boost::network::uri::uri;

public:
    virtual ~Rule() = default;

    const String &line() const { return m_line; }

protected:
    template<typename Str>
    Rule(Str&&);

private:
    String m_line;
};

#include <utility>

template<typename Str>
inline Rule::
Rule(Str &&line)
    : m_line { std::forward<Str>(line) }
{}

#endif // RULE_HPP
