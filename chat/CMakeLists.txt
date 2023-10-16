cmake_minimum_required(VERSION 3.16)
project(chat C)

set(CMAKE_C_STANDARD 99)

add_executable(chat chatServer.c chatServer.h)
