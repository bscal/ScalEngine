#pragma once

#include "Core.h"

namespace Scal
{
	struct Application
	{
		SAPI void Print(int i);
	};

	void Run(Application* app);

}
