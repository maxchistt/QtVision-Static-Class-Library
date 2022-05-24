#include "Example.h"

static NodeKey createShapeSegment(GeometryRep* pShapeRep, const MbVector3D& vecMove, const Color& color, SceneSegment* rootSceneSegment)
{
	SceneSegment* pSegment = new SceneSegment(pShapeRep, rootSceneSegment);
	pSegment->SetColorPresentationMaterial(color);
	pSegment->GetTransform().Move(vecMove);
	return pSegment->GetUniqueKey();
}

static NodeKeyVector createMathGeomSegments(MbItem* item, VSN::SceneSegment* rootSceneSegment)
{
	NodeKeyVector keys;

	if (item->Type() == MbeSpaceType::st_Assembly) {
		//���������� ������ �� ��������� ��������, �� MbAssembly ������������ ����������
		RPArray<MbItem> subitems;
		SArray<MbMatrix3D> matrs;
		MbMatrix3D matrFrom;
		item->GetMatrixFrom(matrFrom);
		item->GetItems(MbeSpaceType::st_Item, matrFrom, subitems, matrs);

		for (auto subitem : subitems) {
			NodeKeyVector subkeys = createMathGeomSegments(subitem, rootSceneSegment);
			keys.insert(keys.cend(), subkeys.cbegin(), subkeys.cend());
		}
	}
	else {
		SceneSegment* segSinSurface = new SceneSegment(SceneGenerator::Instance()->CreateMathRep(item, CommandType::Synchronous), rootSceneSegment);
		//segSinSurface->AddFeature(new Features::DoubleSidedLighting());
		keys.push_back(segSinSurface->GetUniqueKey());
	}

	return keys;
}

static void createTestMathGeomSketch(RPArray<MbContour>& _arrContours)
{
	// �������� ������� ����� ��������, � �������� � ���������� ��������� ����������.
	// ������ ������� - 8 ����� ��� ����� ����� ������� ��������� ����������.
	SArray<MbCartPoint> arrPnts(8);
	arrPnts.Add(MbCartPoint(0, 0));
	arrPnts.Add(MbCartPoint(50, 0));
	arrPnts.Add(MbCartPoint(50, 50));
	arrPnts.Add(MbCartPoint(0, 50));
	// ���������� ������ ������� �������� ������� �� ������
	MbPolyline* pPolyline = new MbPolyline(arrPnts, true);
	MbContour* pContourPolyline = NULL;
	// ������� ���������� � �������������� ������� FilletPolyContour
	::FilletPolyContour(pPolyline, 5, false, arrPnts[4], pContourPolyline);
	// ������� �������� �����, � ������� ����� ���������� ���������� � ������
	// ���������� ����� ����� ��� ���������� � �������������� ������� FilletTwoSegments
	ptrdiff_t idxSideRight1 = 0;
	ptrdiff_t idxSideRight2 = 2;
	ptrdiff_t idxSideRight3 = 4;
	// ���������� ����������
	pContourPolyline->FilletTwoSegments(idxSideRight1, 5);
	pContourPolyline->FilletTwoSegments(idxSideRight2, 5);
	pContourPolyline->FilletTwoSegments(idxSideRight3, 5);
	_arrContours.push_back(pContourPolyline);
}

static MbAssembly* createTestMathGeomAssemblyModel()
{
	// ��������� �� (�� ��������� ��������� � ������� ��)
	MbPlacement3D pl;
	// �������� ���������� ��� ���� ������������
	RPArray<MbContour> arrContours;
	createTestMathGeomSketch(arrContours);
	// ����������� ���������� (� ��������� XY ���������� ��)

	// ���������� ���� ������������
	// ���������� ����������� �� ��������� XY ���������� ��.
	// ������ ���������: ������-��������� ������ ����������� �����������,
	// ��������� �� ���������� �������������� � �������-������� ���� �����
	// ������ �� ������ �������.
	MbPlane* pPlaneXY = new MbPlane(MbCartPoint3D(0, 0, 0),
		MbCartPoint3D(1, 0, 0),
		MbCartPoint3D(0, 1, 0));
	// ������, �������� ��������� ����������
	MbSweptData sweptData(*pPlaneXY, arrContours);
	// ������������ ������ ��� �������� ������������
	MbVector3D dir(0, 0, -1);
	// ��������� �������� ������������, �������� �������� ���� ��� ���������� � ������
	// � � �������� ����������� ����� (������� ������������ � �����).
	const double HEIGHT_FORWARD = 60.0, HEIGHT_BACKWARD = 30.0;
	const double ANGLE_FORWARD_DEGREE = 15.0;
	ExtrusionValues extrusionParams(HEIGHT_FORWARD, HEIGHT_BACKWARD);
	// �������� ������ ��� ������������ � ������ �����������
	extrusionParams.side1.rake = ANGLE_FORWARD_DEGREE * M_PI / 180.0;
	// ����������� ��������� ������ �������� ���� � �������� ����������
	MbSNameMaker operNames(1, MbSNameMaker::i_SideNone, 0);
	PArray<MbSNameMaker> cNames(0, 1, false);
	// ����� �������-������� ��� ���������� �������� ���� ������������
	MbSolid* pSolid = NULL;
	MbResultType res = ::ExtrusionSolid(sweptData, dir, NULL, NULL, false,
		extrusionParams, operNames, cNames, pSolid);

	MbSolid* pSolid2 = NULL;
	MbResultType res2 = ::ExtrusionSolid(sweptData, dir, NULL, NULL, false,
		extrusionParams, operNames, cNames, pSolid2);


	pSolid->Move(MbVector3D(50, 100, 20));
	pSolid->SetColor(LIGHTBLUE);

	pSolid2->Move(MbVector3D(0, 0, 0));
	pSolid2->SetColor(LIGHTGREEN);

	MbAssembly* pAsm = new MbAssembly();
	pAsm->AddItem(*pSolid);
	pAsm->AddItem(*pSolid2);

	// ���������� ��������� ������ ����������� ��������� �������� ����
	::DeleteItem(pPlaneXY);
	::DeleteItem(pSolid);
	::DeleteItem(pSolid2);

	return pAsm;
}


Example::Example(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QtVision::setWindowPosition(*this);

	GraphicsSceneEnginePtr ptrEngine = std::make_shared<GraphicsSceneEngine>();
	OpenGLContextContainer* pContextContainer = new OpenGLContextContainer;
	QtVision::setSurfaceFormat();
	glWidget = new QtVision::QtOpenGLSceneWidget(ptrEngine, pContextContainer, this);
	setCentralWidget(glWidget);

	QtVision::createProcessesCameraControls(glWidget);

	prepareTestSceneBackground();

	connect(ui.drawBase_Action, &QAction::triggered, this, &Example::drawBaseGeomsSceneSlot);
	connect(ui.drawMath_Action, &QAction::triggered, this, &Example::drawMathGeomsSceneSlot);
}

void Example::drawBaseGeomsSceneSlot()
{
	glWidget->sceneContent()->Clear();

	SceneSegment* pTopSegment = glWidget->sceneContent()->GetRootSegment();
	Q_ASSERT(pTopSegment != nullptr);

	// 1 Cone
	::createShapeSegment(SceneGenerator::Instance()->CreateCone(1.5, 1.5), MbVector3D(0.0, 4.0, -1.5), Color(255, 0, 0), pTopSegment);
	// 2 Cylinder
	::createShapeSegment(SceneGenerator::Instance()->CreateCylinder(1.5, 1.5), MbVector3D(-5.0, 4.0, -1.5), Color(0, 190, 0), pTopSegment);
	// 3 Box
	::createShapeSegment(SceneGenerator::Instance()->CreateBox(2.5, 2.5, 2.5), MbVector3D(5.0, -4.0, 0.0), Color(0, 0, 180), pTopSegment);
	// 4 Rectangle
	::createShapeSegment(SceneGenerator::Instance()->CreateRectangle(2.5, 2.5), MbVector3D(0.0, -4.0, 0.0), Color(0, 100, 180), pTopSegment);
	// 5 Sphere
	NodeKey key = ::createShapeSegment(SceneGenerator::Instance()->CreateSphere(1.5f, 15.0f), MbVector3D(-5.0, -4.0, 0.0), Color(100, 100, 180), pTopSegment);
#if 0
	RenderObject* obj = glWidget.sceneContent()->GetContainer()->GetObjectById(key);
	obj->SetPolygonMode(bm_Front_and_Back, pm_Point);
#endif
	// 6 Rectangle
	::createShapeSegment(SceneGenerator::Instance()->CreateRectangle(2.5, 2.5), MbVector3D(5.0, 4.0, 0.0), Color(200, 100, 180), pTopSegment);

	// fit scene
	glWidget->ZoomToFit();
}


void Example::drawMathGeomsSceneSlot()
{
	glWidget->sceneContent()->Clear();

	SceneSegment* pTopSegment = glWidget->sceneContent()->GetRootSegment();
	Q_ASSERT(pTopSegment != nullptr);

	createMathGeomSegments(createTestMathGeomAssemblyModel(), pTopSegment);

	// fit scene
	glWidget->ZoomToFit();
}

void Example::prepareTestSceneBackground()
{
	glWidget->mainLight()->SetDoubleSided(true);
	glWidget->viewport()->SetGradientBackgroundColour(Color(30, 164, 250), Color(250, 250, 250));
}

