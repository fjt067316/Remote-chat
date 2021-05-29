cmake_minimum_required(VERSION 3.17)
project(TcpIpListener)

set(CMAKE_CXX_STANDARD 14)

add_executable(TcpIpListener main.cpp Tcplistener.h TcpListener.cpp)