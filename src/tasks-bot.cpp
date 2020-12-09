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

    const std::vector<std::string> OKText = { "好的", "行", "可以", "完全同意", "OK", "我觉得好", "好吧", "嗯！" };
    const std::vector<std::string> StudyText = { "快去学习", "今天学了吗?", "今天学习了吗?", "啥时候学习?", "忘记学习了吗?", "快去学习啊" };
    const auto onOK = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            const auto count = OKText.size();
            const auto message = OKText[rand() % count];
            const auto replyMsgId = ptr->replyToMessage ? ptr->replyToMessage->messageId : 0;
            bot.getApi().sendMessage(ptr->chat->id, message, false, replyMsgId);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };
    const auto onStudy = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            const auto count = StudyText.size();
            const auto message = StudyText[rand() % count];
            const auto replyMsgId = ptr->replyToMessage ? ptr->replyToMessage->messageId : 0;
            bot.getApi().sendMessage(ptr->chat->id, message, false, replyMsgId);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };
    const auto onAssign = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            const auto replyMsgId = ptr->replyToMessage ? ptr->replyToMessage->messageId : 0;
            bot.getApi().sendMessage(ptr->chat->id, "交 给 你 了", false, replyMsgId);
            onOK(ptr);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    const auto onUnAssign = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            const auto replyMsgId = ptr->replyToMessage ? ptr->replyToMessage->messageId : 0;
            bot.getApi().sendMessage(ptr->chat->id, "不 干 了", false, replyMsgId);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    const auto onStart = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            bot.getApi().sendMessage(ptr->chat->id, R"(`好！`)", true, 0, {}, "MarkdownV2");
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    bot.getEvents().onCommand("start", onStart);
    bot.getEvents().onCommand("ok", onOK);
    bot.getEvents().onCommand("study", onStudy);
    bot.getEvents().onCommand("assign", onAssign);
    bot.getEvents().onCommand("unassign", onUnAssign);

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
