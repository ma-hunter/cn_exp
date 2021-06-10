//
// Created by Shiroha on 21/06/10.
//
#include "utils.h"

using namespace std;
using namespace defs;

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
        os << cover_to_hex(part);
        if (-- cnt) os << ':';
    }
    return os;
}

ostream &utils::operator<<(ostream &os, const mac_addr &ip) {
    int cnt = 6;
    for (auto byte : ip.bt) {
        os << cover_to_hex(byte);
        if (-- cnt) os << ':';
    }
    return os;
}

