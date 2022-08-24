#pragma once

#include "Core.h"

namespace Scal
{

struct ApplicationGame;

void InitializeAudio(const ApplicationGame* AppGame);
void ShutdownAudio();

SAPI void SoundPlayFromFile(const char* soundPath);

}
