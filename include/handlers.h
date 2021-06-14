//
// Created by shiroha on 21/06/10.
//

#ifndef CN_EXP_HANDLERS_H
#define CN_EXP_HANDLERS_H

#include "definitions.h"
#include "pcap.h"

#define DEFINE_PKG_HANDLER(name) \
    void name##_pkg_handler(uchar *param, const pkt_header *header, const uchar *data)
#define CALL_PKG_HANDLER(name, param, header, data) \
    name##_pkg_handler(param, header, data)
#define RECURSIVE_CALL_PKG_HANDLER(name) \
    name##_pkg_handler(param, header, data)
#define GET_PKG_HANDLER(name) name##_pkg_handler

namespace handler {

    using pkt_header = struct pcap_pkthdr;
    using pkt_handler = void(*)(uchar *, const pkt_header *, const uchar *);
}

using namespace handler;

DEFINE_PKG_HANDLER(packet);

DEFINE_PKG_HANDLER(ethernet);

DEFINE_PKG_HANDLER(ipv4);

DEFINE_PKG_HANDLER(ipv6);

DEFINE_PKG_HANDLER(arp);

DEFINE_PKG_HANDLER(icmp);

DEFINE_PKG_HANDLER(tcp);

DEFINE_PKG_HANDLER(udp);

#endif //CN_EXP_HANDLERS_H
