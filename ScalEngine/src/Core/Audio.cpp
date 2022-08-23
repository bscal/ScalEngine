#include "Audio.h"

#include "Core/Logger.h"
#include "Core/SMemory.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

namespace Scal
{

global_var ma_engine Engine;
global_var ma_context Context;
global_var ma_device Device;

// TEST
global_var ma_waveform SinWave;



void DataCallback(ma_device* Device, void* Output, const void* Input, ma_uint32 FrameCount)
{
	//global_var int Hertz = 256;
	//global_var int SamplesPerSecond = 48000;
	//global_var int BytesPerSample = sizeof(float) * 2;
	//global_var ma_uint32 WaveCounter = 0;
	//int CounterLength = SamplesPerSecond / Hertz;
	//int halfSquareWavePeriod = CounterLength / 2;
	//for (ma_uint32 i = 0; i < FrameCount; ++i)
	//{
	//	if (WaveCounter) WaveCounter = CounterLength;

	//	float sineValue = ;
	//	float value = (--WaveCounter < halfSquareWavePeriod) ? 0.01f : -0.01f;
	//	((float*)Output)[i] = value;
	//}

	// In playback mode copy data to pOutput. In capture mode read data from pInput. In full-duplex mode, both
	// pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
	// frameCount frames.
	//ma_waveform* sinWave = (ma_waveform*)Device->pUserData;
	//ma_waveform_read_pcm_frames(sinWave, Output, FrameCount, 0);
}

void InitializeAudioDevice()
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

	ma_waveform_config sinWaveConfig = ma_waveform_config_init(
		ma_format_f32, 2, 48000, ma_waveform_type_sine, .2, 220);
	ma_waveform_init(&sinWaveConfig, &SinWave);

	ma_device_config config = ma_device_config_init(ma_device_type_playback);
	//config.playback.pDeviceID = 0;
	config.deviceType = ma_device_type_playback;
	config.playback.format = ma_format_f32;
	config.playback.channels = 2;
	config.sampleRate = 48000;
	config.dataCallback = DataCallback;
	config.pUserData = &SinWave;

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

void InitializeAudio()
{
	ma_result result = ma_engine_init(0, &Engine);
	if (result != MA_SUCCESS)
	{
		SERROR("MiniAudio result was unsuccessful");
		return;
	}
	SINFO("Audio successfully initialized!");
	ma_engine_set_volume(&Engine, 0.02f);
}

 SAPI void SoundPlayFromFile(const char* soundPath)
{
	ma_engine_play_sound(&Engine, soundPath, 0);
}

void ShutdownAudio()
{
	ma_engine_uninit(&Engine);
	//ma_device_uninit(&Device);
	//ma_context_uninit(&Context);
}

}
