#include "discord_manager.h"
#include "media_info.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <array>
#include <memory>
#include <cstdio>

// ANSI Color Codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// Using a common public Client ID for Apple Music Rich Presence
const std::string DEFAULT_CLIENT_ID = "773825528921849856";

std::string UrlEncode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;
    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else if (c == ' ') {
            escaped << '+';
        } else {
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int((unsigned char)c);
            escaped << std::nouppercase;
        }
    }
    return escaped.str();
}

std::string GetAlbumCoverUrl(const std::string& artist, const std::string& title) {
    std::string query = UrlEncode(artist + " " + title);
    std::string cmd = "curl -s \"https://itunes.apple.com/search?term=" + query + "&entity=song&limit=1\"";
    
    std::array<char, 1024> buffer;
    std::string result;
#ifdef _WIN32
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
#else
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
#endif
    if (!pipe) {
        return "apple";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    std::string searchKey = "\"artworkUrl100\":\"";
    size_t pos = result.find(searchKey);
    if (pos != std::string::npos) {
        size_t start = pos + searchKey.length();
        size_t end = result.find("\"", start);
        if (end != std::string::npos) {
            std::string url = result.substr(start, end - start);
            size_t replacePos = url.find("100x100bb");
            if (replacePos != std::string::npos) {
                url.replace(replacePos, 9, "512x512bb");
            }
            return url;
        }
    }
    return "apple";
}

std::string FormatTime(double total_seconds) {
    int minutes = static_cast<int>(total_seconds) / 60;
    int seconds = static_cast<int>(total_seconds) % 60;
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d", minutes, seconds);
    return std::string(buf);
}

std::string CreateProgressBar(double position, double duration, int width = 20) {
    if (duration <= 0) return "[" + std::string(width, ' ') + "]";
    double progress = position / duration;
    if (progress > 1.0) progress = 1.0;
    if (progress < 0.0) progress = 0.0;
    
    int pos = static_cast<int>(width * progress);
    std::string bar = "[";
    for (int i = 0; i < width; ++i) {
        if (i < pos) bar += "=";
        else if (i == pos) bar += ">";
        else bar += " ";
    }
    bar += "]";
    return bar;
}

int main(int argc, char** argv) {
    std::string client_id = DEFAULT_CLIENT_ID;
    if (argc > 1) {
        client_id = argv[1];
    }

    DiscordManager discord(client_id);
    
    bool last_playing = false;
    std::string last_song_id = "";
    std::string current_cover_url = "apple";
    int64_t last_start_time = 0;

    std::cout << BOLD << MAGENTA << "======================================" << RESET << std::endl;
    std::cout << BOLD << CYAN << "  🎵 Apple Music to Discord RPC 🎵" << RESET << std::endl;
    std::cout << BOLD << MAGENTA << "======================================" << RESET << std::endl;
    std::cout << YELLOW << "Press Ctrl+C to exit." << RESET << "\n" << std::endl;

    while (true) {
        MediaInfo info;
        if (GetCurrentMediaInfo(info)) {
            if (!info.is_playing) {
                if (last_playing) {
                    discord.ClearPresence();
                    last_playing = false;
                    last_song_id = ""; // Reset so it updates immediately when playing resumes
                    std::cout << "\r" << std::string(100, ' ') << "\r";
                    std::cout << YELLOW << "[Status] " << RESET << "Paused/Stopped (Cleared Discord RPC)" << std::endl;
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
                
                std::string current_song_id = info.title + info.artist;
                
                // Only update Discord RPC if song changed or time scrubbed by more than 2 seconds
                bool should_update_rpc = false;
                
                if (current_song_id != last_song_id) {
                    current_cover_url = GetAlbumCoverUrl(info.artist, info.title);
                    last_song_id = current_song_id;
                    should_update_rpc = true;
                    std::cout << "\n";
                } else if (!last_playing) {
                    should_update_rpc = true;
                } else if (std::abs(start_time - last_start_time) > 2) {
                    should_update_rpc = true;
                }
                
                if (should_update_rpc) {
                    std::string details = info.title;
                    std::string state = "by " + info.artist;
                    std::string large_text = "Album: " + info.album;
                    
                    discord.UpdatePresence(state, details, start_time, end_time, current_cover_url, large_text);
                    last_start_time = start_time;
                }
                
                last_playing = true;
                
                // Terminal progress bar
                std::string progress_bar = CreateProgressBar(info.position_sec, info.duration_sec);
                std::string time_str = FormatTime(info.position_sec) + " / " + FormatTime(info.duration_sec);
                
                std::cout << "\r" << std::string(100, ' ') << "\r"; // Clear line
                std::cout << GREEN << "[Playing] " << RESET << BOLD << info.title << RESET << " by " << info.artist 
                          << "  " << CYAN << progress_bar << " " << time_str << RESET << std::flush;
            }
        } else {
            if (last_playing) {
                discord.ClearPresence();
                last_playing = false;
                last_song_id = "";
                std::cout << "\r" << std::string(100, ' ') << "\r"; // Clear line
                std::cout << YELLOW << "[Status] " << RESET << "Apple Music Not Running (Cleared Discord RPC)" << std::endl;
            }
        }
        
#ifdef DISCORD_DISABLE_IO_THREAD
        Discord_UpdateConnection();
#endif
        Discord_RunCallbacks();
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
