#include "Example.h"

#include <QApplication>
#include <QtWidgets/QApplication>

#include <qlogging.h>
#include <last.h>

VSN_USE_NAMESPACE

int main(int argc, char* argv[])
{
#if defined(WIN32) /*&& defined(HUNT_FOR_MEM_LEAKS)*/
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	QApplication app(argc, argv);
	app.setApplicationName("Basics geometries");
	app.setOrganizationName("C3DLabs");

	if (!QtVision::activateLicense())
		return 0;

	Application vapp;

	Example mainWindow;
	mainWindow.show();

	return app.exec();
}
