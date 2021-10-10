# CMakeList.txt : CMake project for Dequeue, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Dequeue")

# Add source to this project's executable.
add_executable (Dequeue "Dequeue.cpp" "Dequeue.h" "defaultmemalloc.h")

# TODO: Add tests and install targets if needed.
