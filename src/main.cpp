#include <iostream>
#include <list>
#include <map>
#include <tgbot/tgbot.h>
#include <thread>

using namespace TgBot;

enum SYSTEMD_MESSAGE_STATUS
{
    ONGOING_A, // [***  ]
    ONGOING_B, // [ *** ]
    ONGOING_C, // [  ***]
    ONGOING_D, // [*  **]
    ONGOING_E, // [**  *]
    NB_COUNT
};

std::map<SYSTEMD_MESSAGE_STATUS, std::string> SYSTEMD_MESSAGE_STATUS_STR{
    { ONGOING_A, "[***  ]" }, //
    { ONGOING_B, "[ *** ]" }, //
    { ONGOING_C, "[  ***]" }, //
    { ONGOING_D, "[*  **]" }, //
    { ONGOING_E, "[**  *]" }  //
};

typedef int32_t chatid_t;
typedef int32_t messageid_t;

struct SystemdMessage
{
    int64_t chatId;
    int32_t messageId;
    SYSTEMD_MESSAGE_STATUS status;
    std::string message;
};

std::string get_message(SystemdMessage &msg)
{
    msg.status = SYSTEMD_MESSAGE_STATUS((msg.status + 1) % NB_COUNT);
    return "`" + SYSTEMD_MESSAGE_STATUS_STR[msg.status] + " " + msg.message + "`";
}

int main(int arc, char *argv[])
{
    std::list<std::thread *> workers;
    TgBot::Bot bot("............................");
    const auto worker_do_job = [&](SystemdMessage msg) {
        int n = 0;
        do
        {
            bot.getApi().editMessageText(get_message(msg), msg.chatId, msg.messageId, {}, "MarkdownV2");
            sleep(1);
            n++;
        } while (n <= 100);
    };

    const auto onSystemd = [&](const TgBot::Message::Ptr ptr) {
        const auto msg = bot.getApi().sendMessage(ptr->chat->id, "`[ WAIT] Starting: " + ptr->text + "`", false, 0, {}, "MarkdownV2");
        const auto t = new std::thread(worker_do_job, SystemdMessage{ msg->chat->id, msg->messageId, ONGOING_A, ptr->text });
        workers.push_back(t);
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
