#include "Example.h"

static NodeKey createShapeSegment(GeometryRep* pShapeRep, const MbVector3D& vecMove, const Color& color, SceneSegment* pParent)
{
	SceneSegment* pSegment = new SceneSegment(pShapeRep, pParent);
	pSegment->SetColorPresentationMaterial(color);
	pSegment->GetTransform().Move(vecMove);
	return pSegment->GetUniqueKey();
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

	connect(ui.draw_Action, &QAction::triggered, this, &Example::makeTestSceneSlot);
}

void Example::makeTestSceneSlot()
{
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

void Example::prepareTestSceneBackground()
{
	glWidget->mainLight()->SetDoubleSided(true);
	glWidget->viewport()->SetBackgroundColour(Color(74, 74, 74));
}
