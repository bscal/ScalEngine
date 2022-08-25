#pragma once

#include "Core.h"

namespace Scal
{

struct ApplicationState;

void InitializeAudio(const ApplicationState* AppGame);
void ShutdownAudio();

SAPI void SoundPlayFromFile(const char* soundPath);

}
