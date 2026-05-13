cmake_minimum_required(VERSION 3.14)
project(AppleMusicStatus2Discord CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch discord-rpc
FetchContent_Declare(
    discord-rpc
    GIT_REPOSITORY https://github.com/discordapp/discord-rpc.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(discord-rpc)

add_executable(AppleMusicStatus2Discord
    src/main.cpp
    src/discord_manager.cpp
)

if (APPLE)
    target_sources(AppleMusicStatus2Discord PRIVATE src/media_mac.cpp)
elseif (WIN32)
    target_sources(AppleMusicStatus2Discord PRIVATE src/media_win.cpp)
    # Required for WinRT
    target_compile_options(AppleMusicStatus2Discord PRIVATE /await)
    target_link_libraries(AppleMusicStatus2Discord PRIVATE windowsapp)
endif()

target_include_directories(AppleMusicStatus2Discord PRIVATE src ${discord-rpc_SOURCE_DIR}/include)
target_link_libraries(AppleMusicStatus2Discord PRIVATE discord-rpc)
