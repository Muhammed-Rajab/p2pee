# basics
cmake_minimum_required(VERSION 3.28)
project(p2pee)

# set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# enable fetch content
include(FetchContent)

# fetch nlohmann/json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json/
  GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

# check for openssl
find_package(OpenSSL REQUIRED)

# check for boost
find_package(Boost REQUIRED COMPONENTS system)

#----------------------------------------
# executable setups
#----------------------------------------

include_directories(include)

# 1. p2pee
add_executable(p2pee
  main.cpp
  src/node.cpp
  src/protocol.cpp
)

target_include_directories(p2pee PRIVATE
  ${json_SOURCE_DIR}/include
)

target_link_libraries(p2pee PRIVATE
  nlohmann_json::nlohmann_json
  OpenSSL::SSL
  OpenSSL::Crypto
  Boost::system
)

# 2. asio-client test
add_executable(client_test
  ./asio-client.cpp
)

target_include_directories(client_test PRIVATE
  ${json_SOURCE_DIR}/include
  include
)

target_link_libraries(client_test PRIVATE
  nlohmann_json::nlohmann_json
  OpenSSL::SSL
  OpenSSL::Crypto
  Boost::system
)
