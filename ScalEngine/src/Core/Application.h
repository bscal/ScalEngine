#pragma once

#include "Core.h"

namespace Scal
{
	struct Application
	{
		SCAL_API void Print(int i);
	};

	void Run(Application* app);

}
