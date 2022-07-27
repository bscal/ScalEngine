#include <Scal.h>
#include <Core/Logger.h>

int main()
{
	Scal::Application* app = new Scal::Application();
	app->Print(5);

	SINFO("Logging %s values, %d", "a", 123);
	SFATAL("FATAL %f ERROR", 0.00525f);
	return 0;
}