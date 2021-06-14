cmake_minimum_required(VERSION 3.19)
project(cn_exp)

set(CMAKE_CXX_STANDARD 17)
set(NPCAP_INCLUDE ./module/include)
set(NPCAP_LIB ./module/lib/x64)
aux_source_directory(./src SRC)
aux_source_directory(./test TEST)

include_directories(${NPCAP_INCLUDE} ./include)
link_directories(${NPCAP_LIB})

add_executable(cn_exp main.cpp src/utils.cpp src/handlers.cpp include/helpers.h src/helpers.cpp)
target_link_libraries(cn_exp Packet.lib wpcap.lib ws2_32.lib libws2_32.a)

add_executable(cn_exp_test test/main.cpp ${SRC} src/handlers.cpp include/helpers.h src/helpers.cpp)
target_link_libraries(cn_exp_test Packet.lib wpcap.lib ws2_32.lib libws2_32.a)