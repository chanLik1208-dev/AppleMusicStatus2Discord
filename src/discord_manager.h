#pragma once
#include "discord_rpc.h"
#include <string>

class DiscordManager {
public:
    DiscordManager(const std::string& client_id);
    ~DiscordManager();

    void UpdatePresence(const std::string& state, const std::string& details, 
                        int64_t startTimestamp, int64_t endTimestamp,
                        const std::string& largeImageKey, const std::string& largeImageText);
    void ClearPresence();

private:
    std::string m_clientId;
};
