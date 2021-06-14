//
// Created by Shiroha on 21/06/15.
//

#ifndef CN_EXP_HELPERS_H
#define CN_EXP_HELPERS_H

#include "definitions.h"

namespace helpers {

    constexpr auto default_output_width = 45;

    void protocol_helper_callback(const defs::ipv4_header *header);

    void protocol_helper_callback(const defs::ipv6_header *header);

    void protocol_helper_callback(const defs::arp_header *header);

    void output_ip_counts(int width = default_output_width);
}

#endif //CN_EXP_HELPERS_H
