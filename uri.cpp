#include "uri.hpp"

#include <http_parser.h>

namespace adblock {

Uri::
Uri(std::string const& url)
    : m_value { url }
{
    parse();
}

Uri::
Uri(std::string&& url)
    : m_value { std::move(url) }
{
    parse();
}

std::string_view Uri::
component(Field const field) const
{
    if (m_fieldSet & (1 << field)) {
        auto const& fieldData = m_fieldData[field];

        return { &m_value[fieldData.offset], fieldData.length };
    }
    else {
        return {};
    }
}

void Uri::
parse()
{
    ::http_parser_url url;
    ::http_parser_url_init(&url);

    //TODO http_parser_parse_url can't handle about:blank, data:fkgjf etc
    auto const fail =
        ::http_parser_parse_url(m_value.c_str(), m_value.size(), 0, &url);
    if (fail) {
        return;
    }

    m_fieldSet = url.field_set;
    m_port = url.port;

    for (auto i = 0; i < Field::Max; ++i) {
        m_fieldData[i].offset = url.field_data[i].off;
        m_fieldData[i].length = url.field_data[i].len;
    }

    m_valid = true;
}


} // namespace adblock
