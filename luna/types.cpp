//
// luna
//
// Copyright © 2016 D.E. Goodman-Wilson
//

#include "types.h"
#include <regex>
#include <base64.h>

namespace luna
{

std::string to_string(const authorization_kind kind)
{
    switch (kind)
    {
        case authorization_kind::BASIC:
            return "Basic";
        default:
            return "";
    }
}


basic_authorization get_basic_authorization(const request_headers &headers)
{
    // First, find the headers
    if(0 == headers.count("Authorization"))
    {
        return {false};
    }

    // Ensure that the header is of the form "Basic abc", and extract the encoded bit
    std::regex basic_regex("Basic ([a-zA-Z0-9=]+)");
    std::smatch basic_match;
    if (!std::regex_match(headers.at("Authorization"), basic_match, basic_regex) || (basic_match.size() != 2))
    {
        return {false};
    }

    // The first sub_match is the whole string; the next
    // sub_match is the first parenthesized expression.
    std::string userpass = base64_decode(basic_match[1].str());

    // We have a string of the form, probably, of username:password. Let's extract the username and password
    std::regex userpass_regex("(.*?):(.*)");
    std::smatch userpass_match;
    if (!std::regex_match(userpass, userpass_match, userpass_regex) || (userpass_match.size() != 3))
    {
        return {false};
    }

    // extract username and password

    return {true, userpass_match[1].str(), userpass_match[2].str()};
}

} //namespace luna