#include "music_player.h"
#include <SDL.h>

bool MusicPlayer::loadMusicFiles(const std::vector<std::string>& paths) {
    for (const std::string& path : paths) {
        Mix_Music* music = Mix_LoadMUS(path.c_str());
        if (!music) {
            SDL_Log("Failed to load %s: %s", path.c_str(), Mix_GetError());
            return false;
        }
        musicTracks.push_back(music);
    }
    return true;
}

void MusicPlayer::startPlaylist() {
    if (musicTracks.empty()) return;
    if (Mix_PlayingMusic()) Mix_HaltMusic();
    currentTrackIndex = 0;
    isPlaying = true;
    playingSingle = false;
    Mix_PlayMusic(musicTracks[currentTrackIndex], 1);
    currentTrackIndex = (currentTrackIndex + 1) % musicTracks.size();
}

void MusicPlayer::playNext() {
    if (musicTracks.empty()) return;

    if (Mix_PlayingMusic()) Mix_HaltMusic();

    Mix_PlayMusic(musicTracks[currentTrackIndex], 1);
    isPlaying = true;
    playingSingle = false;

    currentTrackIndex = (currentTrackIndex + 1) % musicTracks.size();
}


bool MusicPlayer::loadSingleMusic(const std::string& path) {
    if (singleMusic) {
        Mix_FreeMusic(singleMusic);
        singleMusic = nullptr;
    }

    singleMusic = Mix_LoadMUS(path.c_str());
    if (!singleMusic) {
        SDL_Log("Failed to load single music %s: %s", path.c_str(), Mix_GetError());
        return false;
    }
    return true;
}

void MusicPlayer::playSingleMusic() {
    if (!singleMusic) return;

    if (Mix_PlayingMusic()) Mix_HaltMusic();

    Mix_PlayMusic(singleMusic, -1);
    playingSingle = true;
}

void MusicPlayer::LoadSoundEffect(const std::string& path) {
    soundEffect =Mix_LoadWAV(path.c_str());
}

void MusicPlayer::playSfx() {
    Mix_PlayChannel(-1, soundEffect, 0);
}

void MusicPlayer::setVolume(int percent) {
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    int volume = (MIX_MAX_VOLUME * percent) / 100;
    Mix_VolumeMusic(volume);
}

void MusicPlayer::update() {
    if (!Mix_PlayingMusic() && isPlaying && !playingSingle) {
        playNext();
    }
}

void MusicPlayer::cleanup() {
    for (Mix_Music* music : musicTracks) {
        Mix_FreeMusic(music);
    }
    musicTracks.clear();

    if (singleMusic) {
        Mix_FreeMusic(singleMusic);
        singleMusic = nullptr;
    }
    Mix_FreeChunk(soundEffect);
    soundEffect = nullptr;

    isPlaying = false;
    playingSingle = false;
}
