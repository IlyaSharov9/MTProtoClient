cmake_minimum_required(VERSION 3.0)
project(mtproto_client)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${BOOST_INCLUDE_DIR})
include_directories(Library)

file(GLOB mtproto_root_src "./Sources/*.cpp" "./Sources/*.h")
file(GLOB mtproto_details_src "./Sources/details/*.cpp" "./Sources/details/*.h")

add_executable(mtproto_client ${mtproto_root_src} ${mtproto_details_src})
target_link_libraries(mtproto_client OpenSSL::Crypto)
target_link_libraries(mtproto_client ZLIB::ZLIB)
target_link_libraries(mtproto_client ${Boost_Libraries})