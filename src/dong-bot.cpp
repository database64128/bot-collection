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

    const std::vector<std::string> questions = { "这种事情见得多了",
                                                 "我只想说懂得都懂，不懂的我也不多解释",
                                                 "毕竟自己知道就好",
                                                 "你们也别来问我怎么了，利益牵扯太大，说了对你我都没好处，当不知道就行了",
                                                 "这里面水很深，牵扯到很多东西",
                                                 "详细情况你们自己是很难找的，网上大部分已经删除干净了，所以我只能说懂得都懂",
                                                 "懂的人已经基本都获利上岸什么的了",
                                                 "你也不知道谁是懂的人也没法请教，大家都藏着掖着生怕别人知道自己懂事",
                                                 "有些时候，某些人对某些事情不懂装懂，还以为别人不懂",
                                                 "你这种不懂的人让这个懂的人完全教不懂",
                                                 "其实你懂的我也懂谁让我们都懂呢不懂的话也没必要装懂，毕竟里面牵扣扯到很多懂不了的事",
                                                 "这种事懂的人也没必要发出来不懂的人看见又来问七问八最后跟你说了你也不一定能懂",
                                                 "就算懂了以后也对你不好",
                                                 "所以你最好是不懂就不要去了解懂太多不好",
                                                 "你甚至都不知道自己不懂",
                                                 "不懂的人永远不懂",
                                                 "懂得都懂",
                                                 "懂的太多了不是好事",
                                                 "当不知道就行了",
                                                 "细细品吧",
                                                 "其实你自己才是不懂的",
                                                 "所以我只能说懂得都懂",
                                                 "只要点点头就行了",
                                                 "懂太多不好",
                                                 "不懂的人永远不懂，关键懂的人都是自己悟的",
                                                 "毕竟里面牵扣扯到很多懂不了的事",
                                                 "不懂的人永远不懂，只能不懂装懂",
                                                 "懂了就能收割不懂的，你甚至都不知道自己不懂",
                                                 "别人懂的够多了，不仅懂，还懂的超越了这个范围" };
    const auto count = questions.size();
    const auto onAnyMessage = [&](const TgBot::Message::Ptr ptr) {
        try
        {
#define 懂了(x) ptr->text.find(x) != std::string::npos
            if (懂了("懂了") || 懂了("不懂") || 懂了("知道了") || 懂了("明白了") || 懂了("不明白") || 懂了("好的") || 懂了("为啥") || 懂了("什么") ||
                懂了("咋回事"))
            {
                const auto message = questions[rand() % count];
                const auto replyMsgId = ptr->messageId;
                bot.getApi().sendMessage(ptr->chat->id, message, false, replyMsgId);
            }
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    const auto onStart = [&](const TgBot::Message::Ptr ptr) {
        try
        {
            bot.getApi().sendMessage(ptr->chat->id, "所以你最好是不懂就不要去了解懂太多不好", false, ptr->messageId);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    };

    bot.getEvents().onCommand("start", onStart);
    bot.getEvents().onCommand("ok", onAnyMessage);
    bot.getEvents().onAnyMessage(onAnyMessage);

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
