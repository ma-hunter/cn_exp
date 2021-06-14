//
// Created by Shiroha on 21/06/10.
//
#include <tuple>
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

ostream &utils::operator<<(ostream &os, const ipv4_protocol &pro) {
    switch (pro) {
        case TCP:
            os << "TCP";
            break;
        case UDP:
            os << "UDP";
            break;
        case ICMP:
            os << "RARP";
            break;
        default: os << "undefined";
    }
    return os;
}

ostream &utils::operator<<(ostream &os, const ipv6_protocol &pro) {
    switch (pro) {
        case TCPv6:
            os << "TCP";
            break;
        case UDPv6:
            os << "UDP";
            break;
        case ICMPv6:
            os << "ICMP";
            break;
        default: os << "undefined";
    }
    return os;
}

ostream &utils::operator<<(ostream &os, const arp_oc &pro) {
    switch (pro) {
        case ARP_req:
            os << "ARP request protocol";
            break;
        case ARP_rep:
            os << "ARP reply protocol";
            break;
        case RARP_req:
            os << "RARP request protocol";
            break;
        case RARP_rep:
            os << "RARP reply protocol";
            break;
        default: os << "undefined";
    }
    return os;
}

ostream &utils::operator<<(ostream &os, const icmp_type &echo) {
    switch (echo) {
        case ICMP_req:
            os << "ICMP echo request protocol";
            break;
        case ICMP_rep:
            os << "ICMP echo reply protocol";
            break;
        default: os << "undefined";
    }
    return os;
}

tuple<uint, uint, uint> converter::ipv6_vtf_extract(uint vtf) {
    uint version = (vtf & 0xf0000000 >> 28);
    uint traffic_class = (vtf & 0x0ff00000 >> 20);
    uint flow_label = (vtf & 0x000fffff);
    return make_tuple(version, traffic_class, flow_label);
}

string converter::tcp_flag_to_string(tcp_flag tfl) {
    string ret;
    ret.reserve(32);
    if (tfl & 0x01) ret.append("FIN ");
    if (tfl & 0x02) ret.append("SYN ");
    if (tfl & 0x04) ret.append("RST ");
    if (tfl & 0x08) ret.append("PSH ");
    if (tfl & 0x10) ret.append("ACK ");
    if (tfl & 0x20) ret.append("URG ");
    ret.pop_back();
    return move(ret);
}

string converter::ipv4_to_string(ipv4_addr ad) {
    string ret;
    ret.reserve(24);
    for (auto ii : ad.bt) ret.append(to_string(ii)).append(".");
    ret.pop_back();
    return ret;
}

string converter::ipv6_to_string(ipv6_addr ad) {
    string ret;
    ret.reserve(64);
    for (auto ii : ad.pt) ret.append(cover_to_hex(ii)).append(":");
    ret.pop_back();
    return ret;
}
