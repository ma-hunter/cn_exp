//
// Created by Shiroha on 21/06/10.
//
#include "utils.h"

using namespace std;
using namespace defs;

template <typename integral>
inline const char *int_to_hex(integral x) {
    TYPE_TRAITS_INTEGRAL_CHECK;
    static char buf[24];
    fill(buf, buf + 20, '0');
    itoa(x, buf + sizeof(integral), 16);
    return buf + strlen(buf + sizeof(integral));
}

ostream &utils::operator <<(ostream &os, const ipv4_addr &ip) {
    int cnt = 4;
    for (auto byte : ip.bt) {
        os << byte;
        if (-- cnt) os << '.';
    }
    return os;
}

ostream &utils::operator <<(ostream &os, const ipv6_addr &ip) {
    int cnt = 8;
    for (auto part : ip.pt) {
        os << int_to_hex(part);
        if (-- cnt) os << ':';
    }
    return os;
}

ostream &utils::operator <<(ostream &os, const mac_addr &mac) {
    int cnt = 6;
    for (auto byte : mac.bt) {
        os << int_to_hex(byte);
        if (-- cnt) os << ':';
    }
    return os;
}

ostream &utils::operator <<(ostream &os, const eth_type &pro) {
    switch (pro) {
        case defs::IPv4:
            os << "IPv4";
            break;
        case defs::IPv6:
            os << "IPv6";
            break;
        case defs::ARP:
            os << "ARP";
            break;
        case defs::RARP:
            os << "RARP";
            break;
        default: os << "undefined";
    }
    return os;
}

