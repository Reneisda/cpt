cmake_minimum_required(VERSION 3.26)
project(DataTransfer C)

set(CMAKE_C_STANDARD 17)

add_executable(DataTransfer main.c
        server.c
        server.h
        client.c
        client.h
        settings.h
        win/client.c)
