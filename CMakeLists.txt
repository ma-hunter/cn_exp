cmake_minimum_required(VERSION 3.19)
project(cn_exp)

set(CMAKE_CXX_STANDARD 17)
set(NPCAP_INCLUDE ./module/include)
set(NPCAP_LIB ./module/lib/x64)

include_directories(${NPCAP_INCLUDE} ./include)
link_directories(${NPCAP_LIB})

add_executable(cn_exp main.cpp src/utils.cpp)
target_link_libraries(cn_exp Packet.lib wpcap.lib)