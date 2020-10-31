cmake_minimum_required(VERSION 3.18)
project(systemd-bot)

set(CXX_STANDARD 17)


add_subdirectory(3rdparty/tgbot-cpp)

set(SOURCES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} TgBot)

