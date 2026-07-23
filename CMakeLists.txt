cmake_minimum_required(VERSION 4.2)
project(clanmo C)

set(CMAKE_C_STANDARD 17)

add_executable(clanmo main.c
        src/native_functions.c
        src/native_functions.h
        include/error.h
        include/stack.h
        include/parser.h
        include/stack.c
        include/value.h
        include/program.h
        include/vm.h
        src/lanmo.c
        src/lanmo.h
        include/vm.c
        src/parser.c
        include/program.c
        include/memory.h
        include/memory.c
)
