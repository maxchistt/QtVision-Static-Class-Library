#include <vsn_graphicssceneview.h>
#include <vsn_prcameramanipulate.h>
#include <vsn_openglfunctionlist.h>
#include <assert.h> 

//-----------------------------------------------------------------------------
// конструктор по умолчанию
// ----
GraphicsSceneView::GraphicsSceneView(void* pWindowHandle, SceneContent* pSceneContent)
    : m_pWindowHandle(pWindowHandle)
    , m_bInitialized(false)
    , m_bInPaintOpenGL(false)
    , m_bLButtonDown(false)
    , m_bMouseMove(false)
    , m_pLight(new Light)
    , m_pViewPort(new Viewport)
    , m_pSceneContent(pSceneContent)
    , m_pPrManipulate(NULL)
{
    m_pPrManipulate = new PrCameraManipulate(this);
    m_pViewPort->GetCamera()->SetDefaultUpVector(Y_AXIS);
    m_pViewPort->GetCamera()->SetViewOrientation(IsoXYZ);

    m_pLight->SetType(Light::DirectionLight);
    m_pLight->SetPosition(1.0, 1.0, 1.0);
    m_pSceneContent->SetViewport(m_pViewPort);
}

//-----------------------------------------------------------------------------
// деструктор
// ----
GraphicsSceneView::~GraphicsSceneView()
{
    delete m_pPrManipulate;
    delete m_pLight;
    delete m_pViewPort;
}

//-----------------------------------------------------------------------------
// Этот метод инициализирует вид
// ----
void GraphicsSceneView::Initialize()
{
    if (m_bInitialized)
        return;

    m_requestedFormat.SetSamples(4);
    m_requestedFormat.SetDepthBufferSize(24);
    m_requestedFormat.SetStencilBufferSize(8);

    m_pWindow = new Window();
    m_pContext = new OpenGLContext();
    m_pContext->SetFormat(m_requestedFormat);
    m_pWindow->Create(m_pWindowHandle, m_pContext);
    m_bInitialized = true;

    MakeCurrent();
    m_pViewPort->InitOpenGl();
    GlobalFlags::Initialize();

    glEnable(GL_NORMALIZE);
    m_pViewPort->SetBackgroundColour(Color(255, 255, 255));

    SetVertexBufferObject(true);

    if (!m_pSceneContent->GetBoundingBox().IsEmpty())
        m_pViewPort->ZoomToFit(m_pSceneContent->GetBoundingBox());
}

//-------------------------------------------------------------------------------
// Этот метод разрушает вид и его контекст.
// ---
void GraphicsSceneView::Destroy()
{
    if (m_bInitialized)
        MakeCurrent();

    if (m_bInitialized)
        DoneCurrent();

    delete m_pContext;
    m_pContext = V_NULL;

//    delete m_pWindow;
    m_pWindow = 0;

    m_bInitialized = false;
}

//-------------------------------------------------------------------------------
//
// ---
void GraphicsSceneView::MakeCurrent()
{
    if (!m_bInitialized)
        return;

    assert(m_pContext != V_NULL);
    m_pContext->MakeCurrent(m_pWindow);
}

//-------------------------------------------------------------------------------
//
// ---
void GraphicsSceneView::DoneCurrent()
{
    if (!m_bInitialized)
        return;

    assert(m_pContext != V_NULL);
    m_pContext->DoneCurrent();
}

//-------------------------------------------------------------------------------
//
// ---
void GraphicsSceneView::PaintOpenGL()
{
    OpenGLContext* pContext = OpenGLContext::GetCurrentContext();
    assert(pContext != V_NULL);
    OpenGLFunctionList* pFuncs = pContext->GetFunctionList();

    m_pViewPort->SetDistanceClippingPlane(m_pSceneContent->GetBoundingBox());
    m_pSceneContent->GetContainer()->UpdateObjectVisibleState();

    pFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // загрузить ед. матрицу
    OpenGLContext::GetCurrentContext()->oglLoadIdentity();

    m_pLight->oglPerform();
    m_pViewPort->SetClipPlane(true);
    m_pSceneContent->Render(0, ShadedWithEdgesMode);
    m_pViewPort->SetClipPlane(false);
}

//-------------------------------------------------------------------------------
//
// ---
void GraphicsSceneView::Render()
{
    if (!m_bInitialized)
        return;

    MakeCurrent();

    OpenGLContext* pContext = OpenGLContext::GetCurrentContext();
    assert(pContext != V_NULL);

    m_bInPaintOpenGL = true;
    PaintOpenGL();
    pContext->SwapBuffers(m_pWindow);
    m_bInPaintOpenGL = false;
}

//-------------------------------------------------------------------------------
//
// ---
void GraphicsSceneView::Update()
{
    if (!m_bInitialized)
        return;
//    Invalidate(GetHWND());
    ::InvalidateRect((HWND)m_pWindowHandle, NULL, TRUE);
//    m_pWindow->Update();
}

//-------------------------------------------------------------------------------
//
// ---
void GraphicsSceneView::UpdateResize(int cx, int cy)
{
    Initialize();
    if (!m_bInitialized)
        return;
    m_pViewPort->SetSizeViewport(cx, cy);
}

//-------------------------------------------------------------------------------
// Вернуть признак использования VBO.
// ---
bool GraphicsSceneView::IsVertexBufferObject()
{
    return GlobalFlags::IsVertexBufferObject();
}

//-------------------------------------------------------------------------------
// Установить признак использования VBO
// ---
void GraphicsSceneView::SetVertexBufferObject(bool set)
{
    GlobalFlags::SetVertexBufferObject(set);
}

//-------------------------------------------------------------------------------
//
// ---
void GraphicsSceneView::FitWorld(MbCartPoint3D* center)
{
    center = center;
    MbCube cube = m_pSceneContent->GetBoundingBox();
    if (!cube.IsEmpty())
        m_pViewPort->ZoomToFit(m_pSceneContent->GetBoundingBox());
}

//-------------------------------------------------------------------------------
//
// ---
void GraphicsSceneView::SetViewOrientation(Orientation orientation, bool fitWorld)
{
    GetCamera()->SetViewOrientation(orientation);
    if (fitWorld)
        m_pViewPort->ZoomToFit(m_pSceneContent->GetBoundingBox());
}

//-------------------------------------------------------------------------------
//
// ---
void GraphicsSceneView::SetCamera(Camera camera)
{
    m_pViewPort->GetCamera()->Init(camera);
}

//-------------------------------------------------------------------------------
//
// ---
Camera* GraphicsSceneView::GetCamera() const
{
    return m_pViewPort->GetCamera();
}

//-------------------------------------------------------------------------------
//
// ---
Viewport* GraphicsSceneView::GetViewport() const
{
    return m_pViewPort;
}

//-------------------------------------------------------------------------------
//
// ---
int GraphicsSceneView::OnLButtonDownPostMove(MouseEvent& event)
{
    if (m_bLButtonDown)
    {
        if (m_pPrManipulate)
        {
            if (!m_bMouseMove)
                m_pPrManipulate->OnLButtonDown(event);
            m_pPrManipulate->OnLButtonDownPostMove(event);

            m_bMouseMove = true;
        }
        return 0;
    }
    return -1;
}

//-------------------------------------------------------------------------------
//
// ---
int GraphicsSceneView::OnLButtonDown(MouseEvent& event)
{
    m_bLButtonDown = true;
    return -1;
}

//-------------------------------------------------------------------------------
//
// ---
int GraphicsSceneView::OnMouseMove(MouseEvent& event)
{
    int ret = -1;
    if (event.LButton() && !event.RButton())
        ret = OnLButtonDownPostMove(event);
    return ret;
}

//-------------------------------------------------------------------------------
//
// ---
int GraphicsSceneView::OnLButtonUp(MouseEvent& event)
{
    if (m_pPrManipulate)
        m_pPrManipulate->OnLButtonUp(event);

    m_bLButtonDown = false;
    m_bMouseMove = false;
    return -1;
}

//-------------------------------------------------------------------------------
//
// ---
int GraphicsSceneView::OnMouseWheel(WheelEvent& event)
{
    return m_pPrManipulate->OnMouseWheel(event);
}
