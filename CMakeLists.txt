cmake_minimum_required(VERSION 3.18)
project(bot-collection)

set(CXX_STANDARD 17)


add_subdirectory(3rdparty/tgbot-cpp)

option(HAS_ALL_BOTS OFF)
option(HAS_SYSTEMD_BOT OFF)
option(HAS_SHUIBIAO_BOT OFF)

if(HAS_ALL_BOTS OR HAS_SYSTEMD_BOT)
    add_executable(shuibiao-bot src/shuibiao_bot.cpp)
    target_compile_definitions(shuibiao-bot PRIVATE BOT_NAME="shuibiao-bot")
    list(APPEND BOT_TARGETS shuibiao-bot)
endif()

if(HAS_ALL_BOTS OR HAS_SHUIBIAO_BOT)
    add_executable(systemd-bot src/systemd-bot.cpp)
    target_compile_definitions(systemd-bot PRIVATE BOT_NAME="systemd-bot")
    list(APPEND BOT_TARGETS systemd-bot)
endif()

foreach(BOT ${BOT_TARGETS})
    target_link_libraries(${BOT} TgBot)
endforeach()

