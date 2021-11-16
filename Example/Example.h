#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Example.h"

#include <qt_openglwidget.h>
#include <vsn_geomsamples.h>
#include <mesh_triangle.h>
#include <mesh.h>
#include <vsn_labelgeometry.h>

VSN_USE_NAMESPACE

class Example : public QMainWindow
{
	Q_OBJECT

public:
	Example(QWidget* parent = Q_NULLPTR);

private:
	QtVision::QtOpenGLSceneWidget* glWidget = Q_NULLPTR;
	Ui::ExampleClass ui;
	void setupWindowSize();
	void prepareTestScene();
private slots:
	void makeTestSceneSlot();
};
