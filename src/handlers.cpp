//
// Created by shiroha on 21/06/10.
//

#include "handlers.h"
#include "utils.h"
#include "helpers.h"

#include <ctime>
#include <iostream>
#include <winsock2.h>
#include <tuple>

using namespace std;
using namespace defs;

DEFINE_PKG_HANDLER(packet) {
    tm *lt;
    char time_str[16];
    time_t local_tv_sec = header->ts.tv_sec;
    lt = localtime(&local_tv_sec);
    strftime(time_str, sizeof time_str, "%H:%M:%S", lt);
    cout << endl << "Time: " << time_str << ", " << header->ts.tv_usec
         << "\t\tLength: " << header->len << endl;
    CALL_PKG_HANDLER(ethernet, param, header, data);
}

DEFINE_PKG_HANDLER(ethernet) {
    auto eh = (eth_header *)data;
    auto type = ntohs(eh->type);
    cout << "Type: 0x" << to_hex_string(type) << " (" << (eth_type)type << ")\n";
    cout << "Destination address: " << eh->des << endl;
    cout << "Source address: " << eh->src << endl;
    switch ((eth_type)type) {
        case IPv4:
            RECURSIVE_CALL_PKG_HANDLER(ipv4);
            break;
        case IPv6:
            RECURSIVE_CALL_PKG_HANDLER(ipv6);
            break;
        case ARP:
            RECURSIVE_CALL_PKG_HANDLER(arp);
            break;
        case RARP:
            cerr << "We don\'t handle RARP packets. Process exited." << endl;
            break;
        default: cerr << "Unknown eth_type type recognized." << endl;
    }
}

DEFINE_PKG_HANDLER(ipv4) {
    auto ih = (ipv4_header *)(data + eth_header_length);
    cout << "Version: " << (ih->v_ihl & 0xf0 >> 4) << endl;
    const auto ihl = ih->v_ihl & 0x0f;
    cout << "Header length: " << ihl << " (" << (ihl * 4) << "B)" << endl;
    cout << "Type of service: " << int(ih->tos) << endl;
    cout << "Total length: " << ntohs(ih->len) << endl;
    cout << "Identification: " << ntohs(ih->id) << endl;
    const auto flags = ih->fl_fof & 0xe000 >> 12;
    const auto offset = ih->fl_fof & 0x1fff;
    cout << "Flags: " << flags << endl;
    cout << "Fragment offset: " << offset << " (" << offset * 8 << " B)" << endl;
    cout << "Protocol: " << (ipv4_protocol)ih->proto << endl;
    cout << "Checksum: " << ntohs(ih->checksum) << endl;
    cout << "Source IP address: " << ih->src << endl;
    cout << "Destination IP address: " << ih->des << endl;
    switch ((ipv4_protocol)ih->proto) {
        case TCP:
            RECURSIVE_CALL_PKG_HANDLER(tcp);
            break;
        case UDP:
            RECURSIVE_CALL_PKG_HANDLER(udp);
            break;
        case ICMP:
            RECURSIVE_CALL_PKG_HANDLER(icmp);
            break;
        default: cerr << "Unknown ipv4_protocol type recognized." << endl;
    }
    helpers::protocol_helper_callback(ih);
}

DEFINE_PKG_HANDLER(ipv6) {
    auto ih = (ipv6_header *)(data + eth_header_length);
    auto [ver, tc, fl] = converter::ipv6_vtf_extract(ih->vtf);
    cout << "Version: " << ver << endl;
    cout << "Traffic class: " << tc << endl;
    cout << "Flow label: " << fl << endl;
    cout << "Payload length: " << ntohs(ih->len) << endl;
    cout << "Next head: " << ntohs(ih->next) << endl;
    cout << "Hop Limit: " << int(ih->ttl) << endl;
    cout << "Source IP address: " << ih->src << endl;
    cout << "Destination IP address: " << ih->des << endl;
    switch ((ipv6_protocol) ih->next) {
        case TCPv6:
            RECURSIVE_CALL_PKG_HANDLER(tcp);
            break;
        case UDPv6:
            RECURSIVE_CALL_PKG_HANDLER(udp);
            break;
        case ICMPv6:
            RECURSIVE_CALL_PKG_HANDLER(icmp);
            break;
        default: cerr << "Unknown ipv6_protocol type recognized." << endl;
    }
    helpers::protocol_helper_callback(ih);
}

DEFINE_PKG_HANDLER(arp) {
    auto ah = (arp_header *)(data + eth_header_length);
    cout << "Hardware type: " << ntohs(ah->hwt) << endl;
    cout << "Protocol type: " << cover_to_hex(ntohs(ah->prt)) << endl;
    cout << "Hardware address length: " << int(ah->hwl) << endl;
    cout << "Protocol address length: " << int(ah->prl) << endl;
    cout << "Operation code: " << (arp_oc)ah->op << endl;
    cout << "Source IP address: " << ah->src_ip << endl;
    cout << "Destination IP address: " << ah->des_ip << endl;
    cout << "Source MAC address: " << ah->src_mac << endl;
    cout << "Destination MAC address: " << ah->des_mac << endl;
    helpers::protocol_helper_callback(ah);
}

DEFINE_PKG_HANDLER(icmp) {
    auto ih = (icmp_header *)(data + pro_header_length);
    cout << "ICMP type: " << (icmp_type)ih->type << endl;
    cout << "ICMP code: " << ih->code << endl;
    cout << "Identification: " << ih->id << endl;
    cout << "Sequence: " << ih->seq << endl;
    cout << "ICMP checksum: " << ih->checksum << endl;
}

DEFINE_PKG_HANDLER(tcp) {
    auto th = (tcp_header *)(data + pro_header_length);
    cout << "Source port: " << ntohs(th->sp) << endl;
    cout << "Destination port: " << ntohs(th->dp) << endl;
    cout << "Sequence: " << ntohl(th->sq) << endl;
    cout << "Acknowledgement: " << ntohl(th->ac) << endl;
    const auto offset = th->off & 0xf0 >> 4;
    cout << "Data offset: " << offset << " (" << (offset >> 2) << " B)" << endl;
    cout << "Flags: " << converter::tcp_flag_to_string(th->fl) << endl;
    cout << "Window: " << th->windows << endl;
    cout << "Checksum: " << th->checksum << endl;
    cout << "Urgent pointer: " << th->up << endl;
}

DEFINE_PKG_HANDLER(udp) {
    auto uh = (udp_header *)(data + pro_header_length);
    cout << "Source port: " << ntohs(uh->sp) << endl;
    cout << "Destination port: " << ntohs(uh->dp) << endl;
    cout << "Length: " << ntohs(uh->len) << endl;
    cout << "Checksum: " << ntohs(uh->checksum) << endl;
}