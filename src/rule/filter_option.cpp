#include "filter_option.hpp"

#include <ostream>

namespace adblock {

bool FilterOptionSet::
test(FilterOption const opt) const
{
    return m_options.test(
        static_cast<size_t>(opt)
    );
}

bool FilterOptionSet::
any() const
{
    return m_options.any();
}

size_t FilterOptionSet::
count() const
{
    return m_options.count();
}

static uint64_t
operator|(uint64_t const lhs, FilterOption const rhs)
{
    return lhs | static_cast<uint64_t>(rhs);
}

static uint64_t
operator|(FilterOption const lhs, FilterOption const rhs)
{
    return static_cast<uint64_t>(lhs) | rhs;
}

bool FilterOptionSet::
typeSpecified() const
{
    static BitSet const mask {
        // request type
          FilterOption::Object
        | FilterOption::Script
        | FilterOption::StyleSheet
        | FilterOption::Image
        | FilterOption::Media
        | FilterOption::Font
        | FilterOption::Object
        | FilterOption::SubDocument
        | FilterOption::WebSocket
        | FilterOption::WebRtc
        | FilterOption::Ping
        | FilterOption::XmlHttpRequest
        | FilterOption::ObjectSubRequest
        // query type
        | FilterOption::Popup
        | FilterOption::Document
        | FilterOption::DocumentInv
        | FilterOption::ElemHide
        | FilterOption::ElemHideInv
        | FilterOption::GenericBlock
        | FilterOption::GenericHide
        | FilterOption::Csp
        | FilterOption::Rewrite
    };

    return (m_options & mask).any();
}

void FilterOptionSet::
set(FilterOption const opt)
{
    m_options.set(
        static_cast<size_t>(opt)
    );
}

std::ostream&
operator<<(std::ostream& os, FilterOptionSet const& options)
{
    os << "[";

    if (options.test(FilterOption::Other)) {
        os << "Other, ";
    }
    if (options.test(FilterOption::Script)) {
        os << "Script, ";
    }
    if (options.test(FilterOption::StyleSheet)) {
        os << "StyleSheet, ";
    }
    if (options.test(FilterOption::Image)) {
        os << "Image, ";
    }
    if (options.test(FilterOption::Media)) {
        os << "Media, ";
    }
    if (options.test(FilterOption::Font)) {
        os << "Font, ";
    }
    if (options.test(FilterOption::Object)) {
        os << "Object, ";
    }
    if (options.test(FilterOption::SubDocument)) {
        os << "SubDocument, ";
    }
    if (options.test(FilterOption::WebSocket)) {
        os << "WebSocket, ";
    }
    if (options.test(FilterOption::WebRtc)) {
        os << "WebRtc, ";
    }
    if (options.test(FilterOption::Ping)) {
        os << "Ping, ";
    }
    if (options.test(FilterOption::XmlHttpRequest)) {
        os << "XmlHttpRequest, ";
    }
    if (options.test(FilterOption::ObjectSubRequest)) {
        os << "ObjectSubRequest, ";
    }
    if (options.test(FilterOption::Popup)) {
        os << "Popup, ";
    }
    if (options.test(FilterOption::Document)) {
        os << "Document, ";
    }
    if (options.test(FilterOption::DocumentInv)) {
        os << "DocumentInv, ";
    }
    if (options.test(FilterOption::ElemHide)) {
        os << "ElemHide, ";
    }
    if (options.test(FilterOption::ElemHideInv)) {
        os << "ElemHideInv, ";
    }
    if (options.test(FilterOption::GenericBlock)) {
        os << "GenericBlock, ";
    }
    if (options.test(FilterOption::GenericHide)) {
        os << "GenericHide, ";
    }
    if (options.test(FilterOption::Csp)) {
        os << "Csp, ";
    }
    if (options.test(FilterOption::Rewrite)) {
        os << "Rewrite, ";
    }
    if (options.test(FilterOption::Domain)) {
        os << "Domain, ";
    }
    if (options.test(FilterOption::SameOrigin)) {
        os << "SameOrigin, ";
    }
    if (options.test(FilterOption::ThirdParty)) {
        os << "ThirdParty, ";
    }
    if (options.test(FilterOption::SiteKey)) {
        os << "SiteKey, ";
    }
    if (options.test(FilterOption::Inverse)) {
        os << "Inverse, ";
    }
    if (options.test(FilterOption::MatchCase)) {
        os << "MatchCase, ";
    }
    if (options.test(FilterOption::AlwaysCollapse)) {
        os << "AlwaysCollapse, ";
    }
    if (options.test(FilterOption::NeverCollapse)) {
        os << "NeverCollapse, ";
    }

    os << "]";

    return os;
}

} // namespace adblock
