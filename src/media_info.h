#pragma once
#include <string>

struct MediaInfo {
    std::string title;
    std::string artist;
    std::string album;
    bool is_playing;
    double duration_sec;
    double position_sec;
};

// Returns true if successfully fetched, false otherwise
bool GetCurrentMediaInfo(MediaInfo& info);
