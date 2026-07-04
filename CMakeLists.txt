cmake_minimum_required(VERSION 4.2)
project(clanmo C)

set(CMAKE_C_STANDARD 17)

add_executable(clanmo lm.c
        utils/parser.c
        utils/vm.c
        utils/parser.h
        utils/vm.h
        utils/native_functions.c
        utils/native_functions.h)
