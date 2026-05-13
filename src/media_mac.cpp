#include "media_info.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>
#include <vector>

bool GetCurrentMediaInfo(MediaInfo& info) {
    const char* script = 
        "if application \"Music\" is running then\n"
        "  tell application \"Music\"\n"
        "    set track_name to name of current track\n"
        "    set track_artist to artist of current track\n"
        "    set track_album to album of current track\n"
        "    set track_duration to duration of current track\n"
        "    set track_position to player position\n"
        "    set player_state to player state as string\n"
        "    return player_state & \"|||\" & track_name & \"|||\" & track_artist & \"|||\" & track_album & \"|||\" & track_duration & \"|||\" & track_position\n"
        "  end tell\n"
        "else\n"
        "  return \"not_running\"\n"
        "end if";

    std::string cmd = std::string("osascript -e '") + script + "'";
    
    std::array<char, 2048> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        return false;
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    if (result.empty() || result.find("not_running") != std::string::npos) {
        return false;
    }

    std::vector<std::string> parts;
    size_t pos = 0;
    std::string delimiter = "|||";
    while ((pos = result.find(delimiter)) != std::string::npos) {
        parts.push_back(result.substr(0, pos));
        result.erase(0, pos + delimiter.length());
    }
    if (!result.empty()) {
        if (result.back() == '\n') result.pop_back();
        parts.push_back(result);
    }

    if (parts.size() >= 6) {
        info.is_playing = (parts[0] == "playing");
        info.title = parts[1];
        info.artist = parts[2];
        info.album = parts[3];
        try {
            info.duration_sec = std::stod(parts[4]);
            info.position_sec = std::stod(parts[5]);
        } catch (...) {
            info.duration_sec = 0;
            info.position_sec = 0;
        }
        return true;
    }

    return false;
}
