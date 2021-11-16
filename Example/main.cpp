#include "Example.h"
#include <QtWidgets/QApplication>

#include <qt_openglwidget.h>
#include <qlogging.h>
#include <last.h>

VSN_USE_NAMESPACE

int main(int argc, char* argv[])
{
#if defined(WIN32) /*&& defined(HUNT_FOR_MEM_LEAKS)*/
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	QApplication app(argc, argv);

	if (!QtVision::checkLicenseWithInput())return 0;

	Application vapp;
	app.setApplicationName("Basics geometries");
	app.setOrganizationName("C3DLabs");

	Example mainWindow;
	mainWindow.show();

	return app.exec();
}
