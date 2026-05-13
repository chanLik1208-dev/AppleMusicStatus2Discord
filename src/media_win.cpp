#include "media_info.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Media.Control.h>
#include <iostream>

using namespace winrt;
using namespace Windows::Media::Control;

bool GetCurrentMediaInfo(MediaInfo& info) {
    try {
        init_apartment();

        auto manager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
        if (!manager) return false;

        auto session = manager.GetCurrentSession();
        if (!session) return false;

        auto media_props = session.TryGetMediaPropertiesAsync().get();
        if (!media_props) return false;

        auto timeline = session.GetTimelineProperties();
        auto playback = session.GetPlaybackInfo();

        info.title = to_string(media_props.Title());
        info.artist = to_string(media_props.Artist());
        info.album = to_string(media_props.AlbumTitle());
        
        info.is_playing = (playback && playback.PlaybackStatus() == GlobalSystemMediaTransportControlsSessionPlaybackStatus::Playing);
        
        if (timeline) {
            info.duration_sec = timeline.EndTime().count() / 10000000.0;
            info.position_sec = timeline.Position().count() / 10000000.0;
        } else {
            info.duration_sec = 0;
            info.position_sec = 0;
        }

        return true;
    } catch (const winrt::hresult_error& e) {
        return false;
    }
}
