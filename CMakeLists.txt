cmake_minimum_required(VERSION 3.16)

project(Gity)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gity

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
    src/StatusCommand.cpp
    src/FileUtils.cpp
    src/IgnoreManager.cpp
)

target_include_directories(gity PRIVATE include)

target_link_libraries(gity cryptopp)

install(TARGETS gity
    RUNTIME DESTINATION bin
)