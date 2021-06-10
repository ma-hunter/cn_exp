#include <iostream>

#include "definitions.h"
#include "utils.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    defs::ipv6_addr addr{{12, 45, 219, 78, 87, 0, 43, 119}};

    std::cout << addr << ' ' << to_hex_string(100) << std::endl;

    return 0;
}
