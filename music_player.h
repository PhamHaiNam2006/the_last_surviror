#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <SDL_mixer.h>
#include <string>
#include <vector>

class MusicPlayer {
public:
    bool loadMusicFiles(const std::vector<std::string>& paths);
    bool loadSingleMusic(const std::string& path);
    void LoadSoundEffect(const std::string& path);

    void playSingleMusic();
    void startPlaylist();
    void playSfx();
    void playNext();

    void setGameState(bool inGame);
    void setVolume(int percent);
    void update();
    void cleanup();

    Mix_Chunk* getSFX () { return soundEffect; }
private:
    std::vector<Mix_Music*> musicTracks;
    int currentTrackIndex = 0;
    bool isPlaying = false;

    Mix_Music* singleMusic = nullptr;
    bool playingSingle = false;

    Mix_Chunk* soundEffect = nullptr;

    bool isInGame = false;
};

#endif
