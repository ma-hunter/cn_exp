//
// Created by Shiroha on 21/06/10.
//

#ifndef CN_EXP_DEFINITIONS_H
#define CN_EXP_DEFINITIONS_H

#include <iostream>

#include "../module/include/pcap.h"

using uchar = unsigned char;
using llong = long long;
using ullong = unsigned long long;
using ulong = unsigned long;
using ushort = unsigned short;
using uint = unsigned int;

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128_t;
using uch = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

namespace defs {

    struct ipv4_addr {
        uchar bt[4];
    };

    struct ipv6_addr {
        ushort pt[8];
    };

    struct mac_addr {
        uchar bt[6];
    };

    struct eth_header {
        mac_addr des;
        mac_addr src;
        ushort type;
    };

    struct ipv4_header {
        uchar v_ihl;        // Version (4bits) + Internet header length (4bits)
        uchar tos;          // Type of service
        ushort len;         // Total length
        ushort id;          // Identification
        ushort fl_fof;      // Flags (3bits) + Fragment offset (13bits)
        uchar ttl;          // Time to live
        uchar proto;        // Protocol
        ushort checksum;    // Header checksum
        ipv4_addr src;      // Source address
        ipv4_addr des;      // Destination address
        uint op_pad;        // Option + Padding
    };

    struct ipv6_header {
        uint vtf;           // Version traffic class flow label
        ushort len;         // Payload length
        uchar next;         // Next header
        uchar ttl;          // Time to live
        ipv6_addr src;      // Source address
        ipv6_addr des;      // Destination address
    };

    struct arp_header {
        ushort hwt;         // Hardware type
        ushort prt;         // Protocol type
        uchar hwl;          // Hardware length
        uchar prl;          // Protocol length
        ushort op;          // Operation code
        mac_addr src_mac;   // Source MAC address
        ipv4_addr src_ip;   // Source IPv4 address
        mac_addr des_mac;   // Destination MAC address
        ipv4_addr des_ip;   // Destination IPv4 address
    };

    struct tcp_header {
        ushort sp;          // Source port
        ushort dp;          // Destination port
        uint sq;            // Sequence
        uint ac;            // Acknowledgement
        uchar off;          // Offset
        uchar fl;           // Flags
        ushort windows;
        ushort checksum;
        ushort up;          // Urgent pointer
    };

    struct udp_header {
        ushort sp;          // Source port
        ushort dp;          // Destination port
        ushort len;         // Datagram length
        ushort checksum;
    };

    struct icmp_header {
        uchar type;
        uchar code;
        ushort checksum;
        ushort id;
        ushort seq;
    };

    constexpr auto eth_header_length = 14;

    enum eth_type {
        IPv4 = 0x0800,
        IPv6 = 0x86DD,
        ARP = 0x0806,
        RARP = 0x0835
    };

    enum ipv4_protocol {
        TCP = 6,
        UDP = 17,
        ICMP = 1
    };

    enum ipv6_protocol {
        TCPv6 = 6,
        UDPv6 = 17,
        ICMPv6 = 58
    };
}

#endif //CN_EXP_DEFINITIONS_H
