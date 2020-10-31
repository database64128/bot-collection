#include <iostream>
#include <list>
#include <map>
#include <string>
#include <tgbot/tgbot.h>
#include <thread>

using namespace TgBot;

enum SYSTEMD_MESSAGE_STATUS
{
    ONGOING_A, // [***   ]
    ONGOING_B, // [ ***  ]
    ONGOING_C, // [  *** ]
    ONGOING_D, // [   ***]
    ONGOING_E, // [*   **]
    ONGOING_F, // [**   *]
    NB_COUNT
};

std::map<SYSTEMD_MESSAGE_STATUS, std::string> SYSTEMD_MESSAGE_STATUS_STR{
    { ONGOING_A, "[***   ]" }, //
    { ONGOING_B, "[ ***  ]" }, //
    { ONGOING_C, "[  *** ]" }, //
    { ONGOING_D, "[   ***]" }, //
    { ONGOING_E, "[*   **]" }, //
    { ONGOING_F, "[**   *]" }  //
};

typedef int32_t chatid_t;
typedef int32_t messageid_t;

struct SystemdMessage
{
    int64_t chatId;
    int32_t messageId;
    SYSTEMD_MESSAGE_STATUS status;
    std::string message;
    int count;
};

SystemdMessage *message;

std::string get_message(SystemdMessage &msg)
{
    msg.status = SYSTEMD_MESSAGE_STATUS((msg.status + 1) % NB_COUNT);
    return "`" + SYSTEMD_MESSAGE_STATUS_STR[msg.status] + " " + msg.message + "`";
}

std::string remove_from_string(std::string &text)
{
    size_t pos = text.find(" ");
    return text.substr(pos + 1);
}

int main(int arc, char *argv[])
{
    TgBot::Bot bot(getenv("TELEGRAM_API_KEY"));
    const auto worker_do_job = [&]() {
        do
        {
            usleep(1.6 * 1000000);
            static long lastMsgId = 0, lastChatId = 0, lastCount = 0;
            if (!message)
                continue;
            const auto currentMsgId = message->messageId;
            const auto currentChatId = message->chatId;
            //
            const auto hasLastMessage = lastMsgId != 0 && lastChatId != 0;
            const auto differ = currentMsgId != lastMsgId || currentChatId != lastChatId;
            //
            try
            {
                const auto reached = "`[  OK  ] Reached target ";
                if (message->count < 0)
                    continue;
                else if (message->count == 0)
                    bot.getApi().editMessageText(reached + message->message + "`", currentChatId, currentMsgId, {}, "MarkdownV2");
                else if (!differ)
                    bot.getApi().editMessageText(get_message(*message), currentChatId, currentMsgId, {}, "MarkdownV2");
                else if (hasLastMessage && differ)
                    bot.getApi().editMessageText(reached + message->message + "`", lastChatId, lastMsgId, {}, "MarkdownV2");
                message->count--;
            }
            catch (TgBot::TgException &e)
            {
                std::cout << "error: " << e.what() << std::endl;
            }
            //
            lastChatId = currentChatId;
            lastMsgId = currentMsgId;
        } while (true);
    };

    std::thread worker(worker_do_job);

    const auto onSystemd = [&](const TgBot::Message::Ptr ptr) {
        auto text = remove_from_string(ptr->text);
        const auto msg = bot.getApi().sendMessage(ptr->chat->id, "`[ WAIT ] Starting " + text + "`", false, 0, {}, "MarkdownV2");
        if (!message)
            message = new SystemdMessage;
        message->chatId = msg->chat->id;
        message->messageId = msg->messageId;
        message->status = ONGOING_A;
        message->message = text;
        message->count = 15;
    };

    const auto onStart = [&](const TgBot::Message::Ptr ptr) {
        bot.getApi().sendMessage(ptr->chat->id, "`S Y S T E M D`", true, 0, {}, "MarkdownV2");
    };

    bot.getEvents().onCommand("start", onStart);
    bot.getEvents().onCommand("systemd", onSystemd);

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
