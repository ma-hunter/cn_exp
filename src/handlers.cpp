//
// Created by shiroha on 21/06/10.
//

#include "handlers.h"
#include "utils.h"

#include <ctime>
#include <iostream>

using namespace std;
using namespace defs;

DEFINE_PKG_HANDLER(packet) {
    tm *lt;
    char time_str[16];
    time_t local_tv_sec = header->ts.tv_sec;
    lt = localtime(&local_tv_sec);
    strftime(time_str, sizeof time_str, "%H:%M:%S", lt);
    cout << "Time: " << time_str << ", " << header->ts.tv_usec << "\t\tLength: "
         << header->len << endl;
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
    cout << "Type of service: " << ih->tos << endl;
    cout << "Total length: " << ntohs(ih->len) << endl;

}