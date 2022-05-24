#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Example.h"

#include <qt_openglwidget.h>
#include <vsn_geomsamples.h>
#include <mesh_triangle.h>
#include <mesh.h>
#include <vsn_labelgeometry.h>

#include <vsn_colorarray.h>
#include <vsn_polygongeometry.h>

#include "colorDef.h"

#include <vsn_vision.h>

#include <cur_polyline.h>
#include <alg_curve_fillet.h>
#include <cur_contour.h>
#include <op_swept_parameter.h>
#include <action_solid.h>
#include <assembly.h>


VSN_USE_NAMESPACE

class Example : public QMainWindow
{
	Q_OBJECT

public:
	Example(QWidget* parent = Q_NULLPTR);

private:
	QtVision::QtOpenGLSceneWidget* glWidget = Q_NULLPTR;
	Ui::ExampleClass ui;
	void prepareTestSceneBackground();
	
private slots:
	void drawBaseGeomsSceneSlot();
	void drawMathGeomsSceneSlot();
};
