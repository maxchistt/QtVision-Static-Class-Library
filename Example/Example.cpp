#include "Example.h"

static void createShapeSegment(GeometryRep* pShapeRep, const MbVector3D& vecMove, const Color& color, SceneSegment* pParent)
{
	SceneSegment* pSegment = new SceneSegment(pShapeRep, pParent);
	pSegment->SetColorPresentationMaterial(color);
	MbMatrix3D relativeMatrix; relativeMatrix.Move(vecMove);
	pSegment->CreateRelativeMatrix(relativeMatrix);
}

Example::Example(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setupWindowSize();
	QtVision::setSurfaceFormat();

	glWidget = new QtVision::QtOpenGLSceneWidget(this);
	setCentralWidget(glWidget);

	prepareTestScene();

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
	::createShapeSegment(SceneGenerator::Instance()->CreateSphere(1.5), MbVector3D(-5.0, -4.0, 0.0), Color(100, 100, 180), pTopSegment);
	// 5 Rectangle
	::createShapeSegment(SceneGenerator::Instance()->CreateRectangle(2.5, 2.5), MbVector3D(5.0, 4.0, 0.0), Color(200, 100, 180), pTopSegment);

	auto pLabel = new LabelGeometry();
	pLabel->Init(WString("text"), MbPlacement3D(MbVector3D::xAxis, MbVector3D::yAxis, {}));
	pLabel->SetFontSize(22);
	new SceneSegment(new GeometryRep(pLabel), pTopSegment);

	glWidget->sceneContent()->GetContainer()->SetUseVertexBufferObjects(true);
	QtVision::createProcessesCameraControls(glWidget->graphicsEngine()->GetTopEssence());
	glWidget->viewport()->ZoomToFit(glWidget->sceneContent()->GetBoundingBox());
}

void Example::setupWindowSize()
{
	QRect geom = QApplication::desktop()->availableGeometry();
	this->resize(2 * geom.width() / 3, 2 * geom.height() / 3);
}

void Example::prepareTestScene()
{
	glWidget->mainLight()->SetDoubleSided(true);
	glWidget->viewport()->SetBackgroundColour(Color(74, 74, 74));
}
