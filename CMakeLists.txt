cmake_minimum_required(VERSION 3.18)
project(bot-collection)

set(CXX_STANDARD 17)


add_subdirectory(3rdparty/tgbot-cpp)

option(HAS_ALL_BOTS ON)

macro(add_bot name sources)
    option(HAS_${name} OFF)
    if(HAS_ALL_BOTS OR HAS_${name})
        add_executable(${name} ${sources})
        target_compile_definitions(${name} PRIVATE BOT_NAME="${name}")
        target_link_libraries(${name} TgBot)
    endif()
endmacro()

add_bot(shuibiao_bot    src/shuibiao-bot.cpp)
add_bot(systemd_bot     src/systemd-bot.cpp)
add_bot(biaodian_bot    src/biaodian-bot.cpp)
add_bot(tasks_bot       src/task-bot.cpp)
