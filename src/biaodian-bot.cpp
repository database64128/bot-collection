#include <iconv.h>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <tgbot/tgbot.h>
#include <thread>
#include <uniconv.h>

using namespace TgBot;

std::string remove_from_string(std::string &text)
{
    size_t pos = text.find(" ");
    const auto subs = text.substr(pos + 1);
    return subs == text ? "" : subs;
}

int main(int arc, char *argv[])
{
    const auto api_key_env = BOT_NAME "_API_KEY";
    std::cout << "Reading env: " << api_key_env << std::endl;
    TgBot::Bot bot(getenv(api_key_env));

    const auto onPunctuation = [&](const TgBot::Message::Ptr ptr) {
        const auto msg = remove_from_string(ptr->text);
        const auto repeatMsg = msg.empty() ? "?" : msg;
        const auto replyMsgId = ptr->replyToMessage ? ptr->replyToMessage->messageId : 0;
        const auto realMsg = [=] {
            const auto t = (rand() % 50) + 1;
            std::string msg;
            for (auto i = 0; i < t; i++)
                msg += repeatMsg;
            return msg;
        }();

        try
        {
            bot.getApi().sendMessage(ptr->chat->id, realMsg, false, replyMsgId);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    const auto onStart = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            bot.getApi().sendMessage(ptr->chat->id, R"(`!@#$%^&*()_+-={}|[]\:";'<>?,./`)", true, 0, {}, "MarkdownV2");
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    bot.getEvents().onCommand("start", onStart);
    bot.getEvents().onCommand("write", onPunctuation);

    try
    {
        std::cout << "Bot username: " << bot.getApi().getMe()->username << std::endl;
        TgBot::TgLongPoll longPoll(bot);
        while (true)
        {
            longPoll.start();
        }
    }
    catch (std::exception &e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }
    return 0;
}
