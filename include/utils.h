//
// Created by Shiroha on 21/06/10.
//

#ifndef CN_EXP_UTILS_H
#define CN_EXP_UTILS_H

#include <type_traits>
#include <numeric>

#include "definitions.h"

#define NUMERIC_INTEGRAL_CHECK(integral) \
    static_assert(numeric_limits<integral>::is_integer, "Integer is required.")
#define TYPE_TRAITS_INTEGRAL_CHECK(integral) \
    static_assert(is_integral<integral>::value, "Integral is required.")
#define OUTPUT_ALLOW_RADIX_CHECK(radix) \
    static_assert(radix == 10 || radix == 16, "Radix is not allowed.")

namespace utils {
    using namespace std;
    using namespace defs;

    template <typename integral> string to_hex_string(integral x) {
        NUMERIC_INTEGRAL_CHECK(integral);
        TYPE_TRAITS_INTEGRAL_CHECK(integral);
        static char buf[20];
        itoa(x, buf, 16);
        return move(string(buf));
    }

    template <typename integral>
    inline const char *cover_to_hex(integral x) {
        TYPE_TRAITS_INTEGRAL_CHECK(integral);
        static char buf[24];
        fill(buf, buf + 20, '0');
        itoa(x, buf + sizeof(integral), 16);
        return buf + strlen(buf + sizeof(integral));
    }

    string to_hex_string(short x);

    template <typename integral, unsigned radix, bool cover>
    ostream &write_addr(ostream &os, const integral *st, const integral *ed, char split) {
        NUMERIC_INTEGRAL_CHECK(integral);
        TYPE_TRAITS_INTEGRAL_CHECK(integral);
        OUTPUT_ALLOW_RADIX_CHECK(radix);
        for (auto cur = st; cur != ed; ++ cur) {
            if (cur != st) os << split;
            if constexpr (!cover) {
                if constexpr (radix == 16)
                    os << to_hex_string(*cur);
                else os << to_string(*cur);
            } else if constexpr (radix == 16)
                os << cover_to_hex(*cur);
            else os << *cur;
        }
        return os;
    }

    ostream &operator <<(ostream &os, const ipv4_addr &ip);

    ostream &operator <<(ostream &os, const ipv6_addr &ip);

    ostream &operator <<(ostream &os, const mac_addr &mac);

    ostream &operator <<(ostream &os, const eth_type &pro);

    ostream &operator <<(ostream &os, const ipv4_protocol &pro);

    ostream &operator <<(ostream &os, const ipv6_protocol &pro);

    ostream &operator <<(ostream &os, const arp_oc &pro);

    ostream &operator <<(ostream &os, const icmp_type &echo);
}
using namespace utils;

namespace converter {
    using namespace std;
    using namespace defs;

    tuple<uint, uint, uint> ipv6_vtf_extract(uint vtf);

    string tcp_flag_to_string(tcp_flag tfl);

    string ipv4_to_string(ipv4_addr ad);

    string ipv6_to_string(ipv6_addr ad);
}

#endif //CN_EXP_UTILS_H
