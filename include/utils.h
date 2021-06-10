//
// Created by Shiroha on 21/06/10.
//

#ifndef CN_EXP_UTILS_H
#define CN_EXP_UTILS_H

#include <type_traits>
#include <numeric>

#include "definitions.h"

#define NUMERIC_INTEGRAL_CHECK \
    static_assert(numeric_limits<integral>::is_integer, "Integer is required.")
#define TYPE_TRAITS_INTEGRAL_CHECK \
    static_assert(is_integral<integral>::value, "Integral is required.")

namespace utils {
    using namespace std;
    using namespace defs;

    template <typename integral> string to_hex_string(integral x) {
        NUMERIC_INTEGRAL_CHECK;
        TYPE_TRAITS_INTEGRAL_CHECK;
        static char buf[20];
        itoa(x, buf, 16);
        return move(string(buf));
    }
    string to_hex_string(short x);

    ostream &operator <<(ostream &os, const ipv4_addr &ip);

    ostream &operator <<(ostream &os, const ipv6_addr &ip);
}
using namespace utils;

#endif //CN_EXP_UTILS_H
