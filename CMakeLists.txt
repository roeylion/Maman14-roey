cmake_minimum_required(VERSION 3.24)
project(Maman14 C)

set(CMAKE_C_STANDARD 90)

add_executable(Maman14 assembler.c pre_processor.c globals.c globals.h helpers.c helpers.h pre_processor.h parser.c parser.h base64.c base64.h binary.c binary.h errors.c errors.h first_pass.c first_pass.h table.c table.h)
