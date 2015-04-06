#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <ostream>
#include <string>
#include <typeinfo>

#include <boost/network/uri/uri.hpp>
#include <boost/range/iterator_range.hpp>

class Pattern
{
protected:
    using String = std::string;
    using Range = boost::iterator_range<String::const_iterator>;
    using Uri = boost::network::uri::uri;

public:
    virtual ~Pattern() = default;

    virtual bool match(const Uri &target) const = 0;

private:
    virtual void print(std::ostream&) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Pattern &pattern)
    {
        os << typeid(pattern).name() << " ";
        pattern.print(os);
        return os;
    }
};

#endif // PATTERN_HPP
