#include "discord_manager.h"
#include <cstring>
#include <iostream>

static void handleDiscordReady(const DiscordUser* request) {
    std::cout << "Discord RPC Connected" << std::endl;
}

static void handleDiscordDisconnected(int errcode, const char* message) {
    std::cout << "Discord RPC Disconnected (" << errcode << ": " << message << ")" << std::endl;
}

static void handleDiscordError(int errcode, const char* message) {
    std::cout << "Discord RPC Error (" << errcode << ": " << message << ")" << std::endl;
}

DiscordManager::DiscordManager(const std::string& client_id) : m_clientId(client_id) {
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;

    Discord_Initialize(m_clientId.c_str(), &handlers, 1, nullptr);
}

DiscordManager::~DiscordManager() {
    Discord_Shutdown();
}

void DiscordManager::UpdatePresence(const std::string& state, const std::string& details, 
                                    int64_t startTimestamp, int64_t endTimestamp,
                                    const std::string& largeImageKey, const std::string& largeImageText) {
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = state.c_str();
    discordPresence.details = details.c_str();
    if (startTimestamp > 0) {
        discordPresence.startTimestamp = startTimestamp;
        discordPresence.endTimestamp = endTimestamp;
    }
    discordPresence.largeImageKey = largeImageKey.c_str();
    discordPresence.largeImageText = largeImageText.c_str();
    
    Discord_UpdatePresence(&discordPresence);
}

void DiscordManager::ClearPresence() {
    Discord_ClearPresence();
}
