#include "discord_manager.h"
#include "media_info.h"
#include <iostream>
#include <thread>
#include <chrono>

// Using a common public Client ID for Apple Music Rich Presence
const std::string DEFAULT_CLIENT_ID = "773825528921849856";

int main(int argc, char** argv) {
    std::string client_id = DEFAULT_CLIENT_ID;
    if (argc > 1) {
        client_id = argv[1];
    }

    DiscordManager discord(client_id);
    
    bool last_playing = false;

    std::cout << "Apple Music to Discord RPC started. (Press Ctrl+C to exit)" << std::endl;

    while (true) {
        MediaInfo info;
        if (GetCurrentMediaInfo(info)) {
            if (!info.is_playing) {
                if (last_playing) {
                    discord.ClearPresence();
                    last_playing = false;
                    std::cout << "Status: Paused (Cleared RPC)" << std::endl;
                }
            } else {
                int64_t start_time = 0;
                int64_t end_time = 0;
                
                if (info.duration_sec > 0) {
                    auto now = std::chrono::system_clock::now();
                    auto now_sec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
                    
                    start_time = now_sec - static_cast<int64_t>(info.position_sec);
                    end_time = start_time + static_cast<int64_t>(info.duration_sec);
                }
                
                std::string details = info.title;
                std::string state = "by " + info.artist;
                std::string large_text = "Album: " + info.album;
                
                // Using "apple" as the image key which is common for this client ID
                discord.UpdatePresence(state, details, start_time, end_time, "apple", large_text);
                last_playing = true;
                std::cout << "Status: Playing - " << details << " " << state << std::endl;
            }
        } else {
            if (last_playing) {
                discord.ClearPresence();
                last_playing = false;
                std::cout << "Status: Stopped (Cleared RPC)" << std::endl;
            }
        }
        
#ifdef DISCORD_DISABLE_IO_THREAD
        Discord_UpdateConnection();
#endif
        Discord_RunCallbacks();
        
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
