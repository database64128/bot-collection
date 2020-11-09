#include <iostream>
#include <list>
#include <map>
#include <string>
#include <tgbot/tgbot.h>
#include <thread>

using namespace TgBot;

int main(int arc, char *argv[])
{
    const auto api_key_env = BOT_NAME "_API_KEY";
    std::cout << "Reading env: " << api_key_env << std::endl;
    TgBot::Bot bot(getenv(api_key_env));

    const std::vector<std::string> questions = { "你发这些什么目的？",        //
                                                 "谁指使你的？",              //
                                                 "你的动机是什么？",          //
                                                 "你取得有关部门许可了吗 ？", //
                                                 "法律法规容许你发了吗？",    //
                                                 "你背后是谁？",              //
                                                 "发这些想干什么？",          //
                                                 "你想颠覆什么？",            //
                                                 "你要破坏什么？" };
    const auto count = questions.size();
    const auto onQuestion = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            const auto message = questions[rand() % count];
            const auto replyMsgId = ptr->replyToMessage ? ptr->replyToMessage->messageId : 0;
            bot.getApi().sendMessage(ptr->chat->id, message, false, replyMsgId);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    const auto onStart = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            bot.getApi().sendMessage(ptr->chat->id, "跟我们走一趟", false, ptr->messageId);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    bot.getEvents().onCommand("start", onStart);
    bot.getEvents().onCommand("question", onQuestion);
    // bot.getEvents().onCommand("go", onGo);

    try
    {
        std::cout << "Bot username: " << bot.getApi().getMe()->username << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true)
        {
            longPoll.start();
        }
    }
    catch (TgBot::TgException &e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }
    return 0;
}
