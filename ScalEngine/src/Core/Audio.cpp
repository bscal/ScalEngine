#include "Audio.h"

#include "Core/Application.h"
#include "Core/Logger.h"
#include "Core/SMemory.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#define USING_LOW_LEVEL_AUDIO 1

namespace Scal
{

#if USING_LOW_LEVEL_AUDIO
global_var ma_context Context;
global_var ma_device Device;
#else
global_var ma_engine Engine;
#endif


void DataCallback(ma_device* Device, void* Output, const void* Input, ma_uint32 FrameCount)
{
	ApplicationState* GameInstance = (ApplicationState*)Device->pUserData;

	int Hertz = GameInstance->AudioHertz;
	if (Hertz == 0) Hertz = 1;
	int SamplesPerSecond = 48000;
	int BytesPerSample = sizeof(float) * 2;
	int WaveCounter = 0;
	int CounterLength = SamplesPerSecond / Hertz;
	int halfSquareWavePeriod = CounterLength / 2;
	int channel = 0;
	for (int i = 0; i < FrameCount; ++i)
	{
		if (WaveCounter < 1)
			WaveCounter = CounterLength;
		
		float value = (WaveCounter-- > CounterLength / 2) ? 0.01f : -0.01f;
		((float*)Output)[channel++] = value;
		((float*)Output)[channel++] = value;
	}

	// In playback mode copy data to pOutput. In capture mode read data from pInput. In full-duplex mode, both
	// pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
	// frameCount frames.
}

#if USING_LOW_LEVEL_AUDIO
void InitializeAudioDevice(const ApplicationState* gameInstance)
{
	if (ma_context_init(NULL, 0, NULL, &Context) != MA_SUCCESS)
	{
		SERROR("Audio context failed to init");
		return;
	}

	ma_device_info* pPlaybackInfos;
	ma_uint32 playbackCount;
	ma_device_info* pCaptureInfos;
	ma_uint32 captureCount;
	if (ma_context_get_devices(
		&Context, &pPlaybackInfos, &playbackCount,
		&pCaptureInfos, &captureCount) != MA_SUCCESS)
	{
		SERROR("Audio failed to get devices");
		return;
	}

	for (ma_uint32 iDevice = 0; iDevice < playbackCount; ++iDevice)
	{
		SINFO("%d - %s", iDevice, pPlaybackInfos[iDevice].name);
	}

	ma_device_config config = ma_device_config_init(ma_device_type_playback);
	config.deviceType = ma_device_type_playback;
	config.playback.format = ma_format_f32;
	config.playback.channels = 2;
	config.sampleRate = 48000;
	config.dataCallback = DataCallback;
	config.pUserData = (void*)gameInstance;

	if (ma_device_init(&Context, &config, &Device) != MA_SUCCESS)
	{
		SERROR("MiniAudio result was unsuccessful");
		return;
	}

	if (ma_device_start(&Device) != MA_SUCCESS)
	{
		SERROR("MiniAudio device start was unsuccessful");
		return;
	}
}
#endif

void InitializeAudio(const ApplicationState* gameInstance)
{
	#if USING_LOW_LEVEL_AUDIO
	InitializeAudioDevice(gameInstance);
	#else
	if (ma_engine_init(0, &Engine) != MA_SUCCESS)
	{
		SERROR("MiniAudio result was unsuccessful");
		return;
	}
	SINFO("Audio successfully initialized!");

	ma_engine_set_volume(&Engine, 0.02f);
	#endif
}

SAPI void SoundPlayFromFile(const char* soundPath)
{
	#if !USING_LOW_LEVEL_AUDIO
	ma_engine_play_sound(&Engine, soundPath, 0);
	#endif
}

void ShutdownAudio()
{

	#if USING_LOW_LEVEL_AUDIO
	ma_device_uninit(&Device);
	ma_context_uninit(&Context);
	#else
	ma_engine_uninit(&Engine);
	#endif
}

}
