#include <codecvt>
#include <iconv.h>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <tgbot/tgbot.h>
#include <thread>
#include <uniconv.h>

using namespace TgBot;

int main(int arc, char *argv[])
{
    const auto api_key_env = BOT_NAME "_API_KEY";
    std::cout << "Reading env: " << api_key_env << std::endl;
    TgBot::Bot bot(getenv(api_key_env));

    const auto onTypo = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            const auto replyMsgId = ptr->replyToMessage ? ptr->replyToMessage->messageId : 0;
            bot.getApi().sendMessage(ptr->chat->id, "Typo", false, replyMsgId);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    const auto onStart = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            bot.getApi().sendMessage(ptr->chat->id, R"(Hell0 Wolrd.)", true, 0, {}, "MarkdownV2");
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    const auto onInlineQuery = [&](const TgBot::InlineQuery::Ptr ptr) {
        try
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring wide = converter.from_bytes(ptr->query);

            if (wide.empty())
                wide = L"Tpyo";
            else if (wide.length() > 4)
            {
                int start = 1;
                int end = wide.length() - 2;
                const auto randpos = (rand() % (end - start)) + start;
                const auto temp = wide[randpos];
                wide[randpos] = wide[randpos + 1];
                wide[randpos + 1] = temp;
            }

            std::string narrow = converter.to_bytes(wide);
            auto content = std::make_shared<InputTextMessageContent>();
            content->messageText = narrow;
            auto result = std::make_shared<InlineQueryResultArticle>();
            result->id = "1";
            result->inputMessageContent = content;
            result->title = "怎么又打错字了";

            bot.getApi().answerInlineQuery(ptr->id, { result });
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    bot.getEvents().onCommand("start", onStart);
    bot.getEvents().onCommand("typo", onTypo);
    bot.getEvents().onInlineQuery(onInlineQuery);

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
