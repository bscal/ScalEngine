#include "Audio.h"

#include "Core/Logger.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

namespace Scal
{

global_var ma_engine Engine;

void InitializeAudio()
{
    ma_result result = ma_engine_init(0, &Engine);
    if (result != MA_SUCCESS)
    {
        SERROR("MiniAudio result was unsuccessful");
        return;
    }
	SINFO("Audio successfully initialized!");

    // TODO figure out file paths
    // TODO maybe wrap in functions?
    // NOTE this is test code
    const char* soundPath = "F:/dev/ScalEngine/bin/Debug-windows-x86_64/ScalGame/Assets/Audio/VSM.mp3";
    ma_engine_set_volume(&Engine, 0.025f);
    result = ma_engine_play_sound(&Engine, soundPath, 0);
    if (result != MA_SUCCESS)
    {
        SERROR("Audio could not be played");
    }
}

void ShutdownAudio()
{
    ma_engine_uninit(&Engine);
}

}
