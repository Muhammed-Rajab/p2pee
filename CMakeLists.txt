# basics
cmake_minimum_required(VERSION 3.40)
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
  GIT_TAG 3.12.0
)

FetchContent_MakeAvailable(json)

#----------------------------------------
# executable setups
#----------------------------------------

# 1. p2pee
add_executable(p2pee
  main.cpp
)

target link_libraries(p2pee PRIVATE
  nlohmann_json::nlohmann_json
)
