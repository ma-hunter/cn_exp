//
// Created by Shiroha on 21/06/15.
//

#include "helpers.h"
#include "utils.h"

#include <map>
#include <iostream>
#include <iomanip>

using namespace std;

map<string, int> ip_count;

void ip_counter(const string &ip_str) { ++ ip_count[ip_str];}

void ip_counter(string &&ip_str) { ip_counter(ip_str); }

void helpers::protocol_helper_callback(const defs::ipv4_header *header) {
    ip_counter(converter::ipv4_to_string(header->src));
}

void helpers::protocol_helper_callback(const defs::ipv6_header *header) {
    ip_counter(converter::ipv6_to_string(header->src));
}

void helpers::protocol_helper_callback(const defs::arp_header *header) {
    ip_counter(converter::ipv4_to_string(header->src_ip));
}

void helpers::output_ip_counts(int width) {
    cout << "IP" << setfill(' ') << setw(width) << "Flow" << endl;
    for (const auto &[ip, flow] : ip_count)
        cout << ip << setw(int(width - ip.length())) << flow << endl;
}


