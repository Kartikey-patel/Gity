cmake_minimum_required(VERSION 3.16)

project(VersionControl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(vcs
    src/main.cpp
    src/Repository.cpp
    src/ObjectStore.cpp
    src/Hasher.cpp
    src/Index.cpp
    src/AddCommand.cpp
    src/Tree.cpp
    src/Commit.cpp
    src/CommitCommand.cpp
    src/LogCommand.cpp
)

target_link_libraries(vcs cryptopp)