#pragma once

#include "Core.h"

namespace Scal
{

void InitializeAudio();
void ShutdownAudio();

SAPI void SoundPlayFromFile(const char* soundPath);

}
