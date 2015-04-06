#ifndef RULE_HPP
#define RULE_HPP

#include <string>
#include <typeinfo>

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

private:
    virtual void print(std::ostream&) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Rule &rule)
    {
        os << typeid(rule).name() << "\n";
        rule.print(os);
        return os;
    }
};

#endif // RULE_HPP
