cmake_minimum_required(VERSION 3.17)
project(FirstMethod)

set(CMAKE_CXX_STANDARD 14)



add_executable(FirstMethod main.cpp NormalMethod.cpp RandomData.cpp original.cpp Test.cpp original_first.cpp GenerateQuery.cpp ReadQuery.cpp small_test.cpp)



file (GLOB_RECURSE files *.cpp)
foreach (file ${files})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
    add_executable (${exe} ${file})
    message (\ \ \ \ --\ src/${exe}.cpp\ will\ be\ compiled\ to\ bin/${exe})
endforeach ()
