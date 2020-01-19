cmake_minimum_required(VERSION 3.0)
project(plaid-cc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include_directories(include)

find_package(CURL REQUIRED)
find_package(Protobuf REQUIRED)
