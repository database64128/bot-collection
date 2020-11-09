cmake_minimum_required(VERSION 3.18)
project(bot-collection)

set(CXX_STANDARD 17)


add_subdirectory(3rdparty/tgbot-cpp)

option(HAS_ALL_BOTS OFF)
option(HAS_SYSTEMD_BOT OFF)
option(HAS_SHUIBIAO_BOT OFF)
option(HAS_BIAODIAN_BOT OFF)

if(HAS_ALL_BOTS OR HAS_SYSTEMD_BOT)
    add_executable(shuibiao_bot src/shuibiao_bot.cpp)
    target_compile_definitions(shuibiao_bot PRIVATE BOT_NAME="shuibiao_bot")
    list(APPEND BOT_TARGETS shuibiao_bot)
endif()

if(HAS_ALL_BOTS OR HAS_SHUIBIAO_BOT)
    add_executable(systemd_bot src/systemd-bot.cpp)
    target_compile_definitions(systemd_bot PRIVATE BOT_NAME="systemd_bot")
    list(APPEND BOT_TARGETS systemd_bot)
endif()

if(HAS_ALL_BOTS OR HAS_BIAODIAN_BOT)
    add_executable(biaodian_bot src/biaodian-bot.cpp)
    target_compile_definitions(biaodian_bot PRIVATE BOT_NAME="biaodian_bot")
    list(APPEND BOT_TARGETS biaodian_bot)
endif()

foreach(BOT ${BOT_TARGETS})
    target_link_libraries(${BOT} TgBot)
endforeach()

