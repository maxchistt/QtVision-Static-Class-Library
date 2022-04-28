#include <qt_openglwidgetprivate.h>
#include <QApplication>
#include <QScreen>
#include <QWindow>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QtGui/qevent.h>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOffscreenSurface>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QOpenGLFramebufferObject>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
#include <QScreen>
#else
#include <QDesktopWidget>
#endif

#include <vsn_window.h>
#include <vsn_openglfbo.h>
#include <vsn_graphicsview.h>
#include <vsn_renderstatistics.h>
#include <vsn_graphicssceneengine.h>
#include <vsn_graphicsscene.h>
#include <vsn_texture2d.h>
#include <tool_enabler.h>

#include <qt_aboutscenewidget.h>
#include <qt_openglcontext.h>
#include <qt_licensewnd.h>

#include <last.h>

VSN_BEGIN_NAMESPACE

namespace QtVision {

/* QtConverterEventListener */
QtConverterEventListener::QtConverterEventListener(QObject* pParent, GraphicsScene* pListenerEvent)
    : QObject(pParent)
    , m_pListenerEvent(pListenerEvent)
    , m_idleTimerId(-1)
{
    Q_ASSERT(m_pListenerEvent != nullptr);
    parent()->installEventFilter(this);
#ifndef VSN_OS_WIN
    m_idleTimerId = startTimer(QGuiApplication::primaryScreen()->refreshRate());
#endif
}

//-----------------------------------------------------------------------------
//
// ---
QtConverterEventListener::~QtConverterEventListener()
{
    parent()->removeEventFilter(this);
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::keyPressEvent(QKeyEvent* event)
{
    KeyEvent processEvent(ProcessEvent::KeyDown, event->key(), event->nativeScanCode(), event->nativeVirtualKey(), event->nativeModifiers());
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::keyReleaseEvent(QKeyEvent* event)
{
    KeyEvent processEvent(ProcessEvent::KeyUp, event->key(), event->nativeScanCode(), event->nativeVirtualKey(), event->nativeModifiers());
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseHoverEvent(QHoverEvent* event)
{
    Q_ASSERT(m_pListenerEvent != nullptr);
    int flags = 0;
    HoverEvent processEvent(ProcessEvent::HoverMove, PointI(event->posF().x(), event->posF().y()), PointI());
    processEvent.SetFlags(flags);
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
static int mapModifiers(Qt::KeyboardModifiers state)
{
    int flag = 0;
    if (state & Qt::ShiftModifier)
        flag |= mb_Shift;
    if (state & Qt::ControlModifier)
        flag |= mb_Control;
    if (state & Qt::AltModifier)
        flag |= mb_Alt;
    return flag;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mousePressEvent(QMouseEvent* event)
{
    Q_ASSERT(m_pListenerEvent != nullptr);
    ProcessEvent::EventType type = ProcessEvent::None;
    int flags = mapModifiers(event->modifiers());
    if (event->button() == Qt::LeftButton)
    {
        flags |= mb_LButton;
        type = ProcessEvent::LButtonDown;
    }
    else if (event->button() == Qt::MiddleButton)
    {
        flags |= mb_MButton;
        type = ProcessEvent::MButtonDown;
    }
    else if (event->button() == Qt::RightButton)
    {
        flags |= mb_RButton;
        type = ProcessEvent::RButtonDown;
    }

    std::shared_ptr<Viewport> ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(type);
    processEvent.SetFlags(flags);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseMoveEvent(QMouseEvent* event)
{
    int flags = mapModifiers(event->modifiers());
    if (event->buttons() == Qt::LeftButton)
        flags |= mb_LButton;
    if (event->buttons() == Qt::MiddleButton)
        flags |= mb_MButton;
    if (event->buttons() == Qt::RightButton)
        flags |= mb_RButton;

    std::shared_ptr<Viewport> ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(ProcessEvent::MouseMove);
    processEvent.SetFlags(flags);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseReleaseEvent(QMouseEvent* event)
{
    ProcessEvent::EventType type = ProcessEvent::None;
    int flags = mapModifiers(event->modifiers());
    if (event->button() == Qt::LeftButton)
    {
        flags |= mb_LButton;
        type = ProcessEvent::LButtonUp;
    }
    else if (event->button() == Qt::MiddleButton)
    {
        flags |= mb_MButton;
        type = ProcessEvent::MButtonUp;
    }
    else if (event->button() == Qt::RightButton)
    {
        flags |= mb_RButton;
        type = ProcessEvent::RButtonUp;
    }

    std::shared_ptr<Viewport> ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(type);
    processEvent.SetFlags(flags);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseDoubleClickEvent(QMouseEvent* event)
{
    ProcessEvent::EventType type = ProcessEvent::None;
    int flags = mapModifiers(event->modifiers());
    if (event->button() == Qt::LeftButton)
    {
        flags |= mb_LButton;
        type = ProcessEvent::LButtonDblClk;
    }
    else if (event->button() == Qt::MiddleButton)
    {
        flags |= mb_MButton;
        type = ProcessEvent::MButtonDblClk;
    }
    else if (event->button() == Qt::RightButton)
    {
        flags |= mb_RButton;
        type = ProcessEvent::RButtonDblClk;
    }

    std::shared_ptr<Viewport> ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(type);
    processEvent.SetFlags(flags);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseWheelEvent(QWheelEvent* event)
{
    WheelEvent processEvent;
    int flags = mapModifiers(event->modifiers());
    processEvent.SetFlags(flags);
    processEvent.SetWheelDelta(event->delta());
    processEvent.SetWindowPos(PointI(event->pos().x(), event->pos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseEnterEvent(QMouseEvent* event)
{
    std::shared_ptr<Viewport> ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(ProcessEvent::MouseEnter);
    processEvent.SetFlags(0);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
//
// ---
bool QtConverterEventListener::mouseLeaveEvent(QMouseEvent* event)
{
    std::shared_ptr<Viewport> ptrViewport = m_pListenerEvent->GetViewport();
    MouseEvent processEvent(ProcessEvent::MouseLeave);
    processEvent.SetFlags(0);
    processEvent.SetViewport(ptrViewport != nullptr ? ptrViewport->GetId() : 0);
    processEvent.SetScreenPos(PointI(event->screenPos().x(), event->screenPos().y()));
    processEvent.SetWindowPos(PointI(event->windowPos().x(), event->windowPos().y()));
    processEvent.SetClientPos(PointI(event->localPos().x(), event->localPos().y()));
    BaseApplication::GetInstance()->OnProcessSendEvent(m_pListenerEvent, &processEvent);
    return false;
}

//-----------------------------------------------------------------------------
// Перегрузка для внутренних работ.
// ---
bool QtConverterEventListener::eventFilter(QObject* watched, QEvent* event)
{
    if (watched != parent())
        return QObject::eventFilter(watched, event);

    switch (event->type())
    {
        case QEvent::KeyPress:
            return keyPressEvent(static_cast<QKeyEvent*>(event));
        case QEvent::KeyRelease:
            return keyReleaseEvent(static_cast<QKeyEvent*>(event));
        case QEvent::HoverMove:
            return mouseHoverEvent(static_cast<QHoverEvent*>(event));
        case QEvent::MouseMove:
            return mouseMoveEvent(static_cast<QMouseEvent*>(event));
        case QEvent::MouseButtonDblClick:
            return mouseDoubleClickEvent(static_cast<QMouseEvent*>(event));
        case QEvent::MouseButtonPress:
            return mousePressEvent(static_cast<QMouseEvent*>(event));
        case QEvent::MouseButtonRelease:
            return mouseReleaseEvent(static_cast<QMouseEvent*>(event));
        case QEvent::Wheel:
            return mouseWheelEvent(static_cast<QWheelEvent*>(event));
        case QEvent::Enter:
            return mouseEnterEvent(static_cast<QMouseEvent*>(event));
        case QEvent::Leave:
            return mouseLeaveEvent(static_cast<QMouseEvent*>(event));
        default:
            break;
    }
    return QObject::eventFilter(watched, event);
}

//-----------------------------------------------------------------------------
//
// ---
void QtConverterEventListener::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_idleTimerId)
        BaseApplication::OnProcessSendPostedEvents();
    else
        QObject::timerEvent(event);
}

//-----------------------------------------------------------------------------
// 
// ---
void QtOpenGLWidgetPrivate::initialize()
{
    VSN_P(QtOpenGLWidget);
    if (m_bInitialized)
        return;
    qApp->setWindowIcon(QIcon(":/res/crystal.ico"));
    p.makeCurrent();
    m_pQContext = new QtOpenGLContextShell(&p, m_pSharedContainer);
    m_pQContext->MakeCurrent();
    m_bInitialized = true;
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidgetPrivate::reset()
{
    VSN_P(QtOpenGLWidget);
    VSN_DELETE_AND_NULL(m_pQContext);
    m_bInitialized = false;
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidgetPrivate::renderNow()
{
    VSN_P(QtOpenGLWidget);
    QOpenGLContext* openGLContext = p.context();
    if (!openGLContext)
        return;
    p.makeCurrent();
    p.paintGL();
    openGLContext->swapBuffers(openGLContext->surface());
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidgetPrivate::renderLater()
{
    VSN_P(QtOpenGLWidget);
    if (!m_bUpdatePending)
    {
        m_bUpdatePending = true;
        QCoreApplication::postEvent(&p, new QEvent(QEvent::UpdateRequest));
    }
}

//-----------------------------------------------------------------------------
// Конструктор по умолчанию.
// ---
QtOpenGLWidget::QtOpenGLWidget(QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f)
{
    VSN_INIT_PRIVATE(QtOpenGLWidget);
}

//-----------------------------------------------------------------------------
// Конструктор по умолчанию.
// ---
QtOpenGLWidget::QtOpenGLWidget(QtOpenGLWidgetPrivate& dd, QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f)
{
    vsn_d_ptr = &dd;
    vsn_d_ptr->setPublic(this);
}

//-----------------------------------------------------------------------------
// Деструктор OpenGLWidget освобождает созданные ресурсы
// ---
QtOpenGLWidget::~QtOpenGLWidget()
{
    // Вылет при закрытии вкладки документа
    makeCurrent();
    VSN_FINI_PRIVATE();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidget::makeCurrent()
{
    VSN_D(QtOpenGLWidget);
    if (d.m_pQContext)
        d.m_pQContext->MakeCurrent();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidget::doneCurrent()
{
    VSN_D(QtOpenGLWidget);
    if (d.m_pQContext)
        d.m_pQContext->DoneCurrent();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на OpenGLContextInterface.
// ---
OpenGLContextInterface* QtOpenGLWidget::contextInterface()
{
    VSN_D(QtOpenGLWidget);
    return d.m_pQContext;
}

//-----------------------------------------------------------------------------
// Просто вызывает функцию QWidget::update(). Служит для совместимости слотов.
// ---
void QtOpenGLWidget::updateWidget()
{
    update();
}

//-----------------------------------------------------------------------------
// Просто вызывает функцию QWidget::rapaint(). Служит для совместимости слотов.
// ---
void QtOpenGLWidget::repaintWidget()
{
    repaint();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidget::initializeGL()
{
    VSN_D(QtOpenGLWidget);
    if (!d.m_bInitialized && window() && window()->windowHandle())
        d.initialize();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLWidget::paintEvent(QPaintEvent* event)
{
    QOpenGLWidget::paintEvent(event);
}

//-----------------------------------------------------------------------------
//
// ---
bool QtOpenGLWidget::event(QEvent* event)
{
    VSN_D(QtOpenGLWidget);
    if (event->type() == QEvent::WindowChangeInternal && d.m_bInitialized && !qGuiApp->testAttribute(Qt::AA_ShareOpenGLContexts))
        d.reset();

    bool ok = QOpenGLWidget::event(event);

    switch (event->type())
    {
        case QEvent::UpdateRequest:
            if (d.m_bUpdatePending)
            {
                update();
                d.m_bUpdatePending = false;
                return true;
            }
            break;
        case QEvent::WindowChangeInternal:
            if (qGuiApp->testAttribute(Qt::AA_ShareOpenGLContexts))
                break;
            if (d.m_bInitialized)
                d.reset();
            // FALLTHROUGH
        case QEvent::Show: // reparenting may not lead to a resize so reinitalize on Show too
            if (!d.m_bInitialized && !size().isEmpty() && window() && window()->windowHandle())
                d.initialize();
            break;
        default:
            break;
    }
    return ok;
}


/* OpenGLWidgetPrivate */
class QtOpenGLSceneWidgetPrivate : public QtOpenGLWidgetPrivate
{
    VSN_DECLARE_EX_PUBLIC(QtOpenGLSceneWidget)
public:
    explicit QtOpenGLSceneWidgetPrivate(GraphicsSceneEnginePtr ptrEngine, OpenGLContextContainer* pSharedContainer)
        : QtOpenGLWidgetPrivate(pSharedContainer)
        , m_ptrGraphicsView(std::make_shared<GraphicsView>(ptrEngine))
        , m_pEventFilter(nullptr)
        , m_pAboutScene(nullptr)
        , m_bVsnInitialized(false)
    {
    }
    virtual ~QtOpenGLSceneWidgetPrivate()
    {
        m_ptrGraphicsView->GetSceneEngine()->GetObjectPickSelection()->ResetOpenGLContext(m_pQContext);
        VSN_DELETE_AND_NULL(m_pQContext);
        VSN_DELETE_AND_NULL(m_pEventFilter);
    }
    virtual void initialize() override;
public:
    void processActivation();
    void processDeactivation();
public:
    GraphicsViewPtr m_ptrGraphicsView;
    QtConverterEventListener* m_pEventFilter;
    QtAboutSceneWidget* m_pAboutScene;
    bool m_bVsnInitialized;
};

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidgetPrivate::initialize()
{
    VSN_P(QtOpenGLSceneWidget);
    if (m_bInitialized)
        return;
    QtOpenGLWidgetPrivate::initialize();
    if (m_bVsnInitialized)
        return;

    p.setFocusPolicy(Qt::StrongFocus);
    p.setMouseTracking(true);
    p.setAttribute(Qt::WA_Hover, true);

    m_ptrGraphicsView->Initialize();
    m_pEventFilter = new QtConverterEventListener(&p, m_ptrGraphicsView->GetGraphicsScene());
    Object::Connect(m_ptrGraphicsView.get(), &GraphicsView::OnViewModified, &p, &QtOpenGLWidget::repaintWidget);

    m_bVsnInitialized = true;
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidgetPrivate::processActivation()
{
    VSN_P(QtOpenGLSceneWidget);
    if (Essence* pEssence = m_ptrGraphicsView->GetSceneEngine()->GetTopEssence())
    {
        std::vector<Process*> vector = pEssence->FindChildren<Process*>();
        for (auto it = vector.begin(); it != vector.end(); ++it)
        {
            Process* pProcess = (*it);
            pProcess->SetViewport(m_ptrGraphicsView->GetViewport().get());
            bool bConnect = Object::Connect(pProcess, &Process::OnModified, &p, &QtOpenGLWidget::repaintWidget);
            Q_ASSERT(bConnect != false);
            if (PrAbstractCamera* pAbsProcess = vobject_cast<PrAbstractCamera*>(pProcess))
            {
                bConnect = Object::Connect(pAbsProcess, &PrAbstractCamera::OnCameraModified, &p, &QtOpenGLWidget::repaintWidget);
                Q_ASSERT(bConnect != false);
            }
        }
    }
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidgetPrivate::processDeactivation()
{
    VSN_P(QtOpenGLSceneWidget);
    if (Essence* pEssence = m_ptrGraphicsView->GetSceneEngine()->GetTopEssence())
    {
        std::vector<Process*> vector = pEssence->FindChildren<Process*>();
        for (auto it = vector.begin(); it != vector.end(); ++it)
        {
            Process* pProcess = (*it);
            pProcess->SetViewport(m_ptrGraphicsView->GetViewport().get());
            Object::Disconnect(pProcess, &Process::OnModified, &p, &QtOpenGLWidget::repaintWidget);

            if (PrAbstractCamera* pAbsProcess = vobject_cast<PrAbstractCamera*>(pProcess))
                Object::Disconnect(pAbsProcess, &PrAbstractCamera::OnCameraModified, &p, &QtOpenGLWidget::repaintWidget);
        }
    }
}


/* QtOpenGLSceneWidget */
QtOpenGLSceneWidget::QtOpenGLSceneWidget(QWidget* parent, Qt::WindowFlags f)
    : QtOpenGLWidget(*new QtOpenGLSceneWidgetPrivate(std::make_shared<GraphicsSceneEngine>(), nullptr), parent, f)
{
}

//-----------------------------------------------------------------------------
// Special constructor.
// ---
QtOpenGLSceneWidget::QtOpenGLSceneWidget(GraphicsSceneEnginePtr engine, OpenGLContextContainer* sharedContainer, QWidget* parent, Qt::WindowFlags f)
    : QtOpenGLWidget(*new QtOpenGLSceneWidgetPrivate(engine, sharedContainer), parent, f)
{
}

//-----------------------------------------------------------------------------
//
// ---
QtOpenGLSceneWidget::~QtOpenGLSceneWidget()
{
}

//-----------------------------------------------------------------------------
//
// ---
GraphicsScene* QtOpenGLSceneWidget::graphicsScene() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_ptrGraphicsView->GetGraphicsScene();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на компонент выбора объектов.
// ---
ObjectPickSelection* QtOpenGLSceneWidget::objectPickSelection() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_ptrGraphicsView->GetSceneEngine()->GetObjectPickSelection();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на GraphicsSceneEngine.
// ---
GraphicsSceneEnginePtr QtOpenGLSceneWidget::graphicsEngine() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_ptrGraphicsView->GetSceneEngine();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на GraphicsView.
// ---
GraphicsViewPtr QtOpenGLSceneWidget::graphicsView() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_ptrGraphicsView;
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::setAboutSceneWidget(QtAboutSceneWidget* pAboutScene)
{
    VSN_D(QtOpenGLSceneWidget);
    d.m_pAboutScene = pAboutScene;
}

//-----------------------------------------------------------------------------
// Вернуть указатель на основной источник света
// ---
Light* QtOpenGLSceneWidget::mainLight() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_ptrGraphicsView->GetGraphicsScene()->GetMainLight();
}

//-----------------------------------------------------------------------------
//
// ---
Camera* QtOpenGLSceneWidget::camera() const
{
    return viewport()->GetCamera();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на Viewport для отображения сцены.
// ---
Viewport* QtOpenGLSceneWidget::viewport() const
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_ptrGraphicsView->GetViewport().get();
}

//-----------------------------------------------------------------------------
// Вернуть указатель на содержимое сцены.
// ---
SceneContentPtr QtOpenGLSceneWidget::sceneContent()
{
    VSN_D(const QtOpenGLSceneWidget);
    return d.m_ptrGraphicsView->GetGraphicsScene()->GetSceneContent();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::resizeGL(int w, int h)
{
    VSN_D(QtOpenGLSceneWidget);
    QtOpenGLWidget::resizeGL(w, h);
    if (d.m_pQContext == nullptr)
        return;
    d.m_pQContext->MakeCurrent();
    if (OpenGLContextInterface::GetCurrentContext() && d.m_pQContext == OpenGLContextInterface::GetCurrentContext())
        d.m_ptrGraphicsView->DoResize(w, h);

    if (d.m_pAboutScene != Q_NULLPTR)
    {
        const int margin = 10;
        QRect rc = rect();
        QSize sz = d.m_pAboutScene->sizeHint();
        rc.setTopLeft(QPoint(rc.right() - sz.width() - margin, margin));
        d.m_pAboutScene->setPosition(rc.topLeft());
    }
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::paintGL()
{
    VSN_D(QtOpenGLSceneWidget);
    if (d.m_pQContext == nullptr)
        return;

    if (RenderStatistics::IsActivated())
        RenderStatistics::ResetCounters();

    d.m_pQContext->MakeCurrent();
    d.m_ptrGraphicsView->DoRender();
    d.m_pQContext->DoneCurrent();

    if (d.m_pAboutScene != Q_NULLPTR)
    {
        QPainter painter(this);
        d.m_pAboutScene->updateStatistics();
        d.m_pAboutScene->paintStatistics(&painter);
    }
}

//-----------------------------------------------------------------------------
//
// ---
bool QtOpenGLSceneWidget::event(QEvent* event)
{
    return QtOpenGLWidget::event(event);
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::enterEvent(QEvent* event)
{
     VSN_D(QtOpenGLSceneWidget);
     QtOpenGLWidget::enterEvent(event);
     d.m_ptrGraphicsView->GetSceneEngine()->GetObjectPickSelection()->SetOpenGLContext(d.m_pQContext);
     d.m_ptrGraphicsView->GetSceneEngine()->GetObjectPickSelection()->SetViewport(d.m_ptrGraphicsView->GetViewport());
     d.processActivation();
}

//-----------------------------------------------------------------------------
//
// ---
 void QtOpenGLSceneWidget::leaveEvent(QEvent* event)
{
     VSN_D(QtOpenGLSceneWidget);
     QtOpenGLWidget::leaveEvent(event);
     d.processDeactivation();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::focusInEvent(QFocusEvent* event)
{
    VSN_D(QtOpenGLSceneWidget);
    QtOpenGLWidget::focusInEvent(event);
    d.processActivation();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::focusOutEvent(QFocusEvent* event)
{
    VSN_D(QtOpenGLSceneWidget);
    QtOpenGLWidget::focusOutEvent(event);
    d.processDeactivation();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::ZoomToFit()
{
    VSN_D(QtOpenGLSceneWidget);
    if ( viewport() && sceneContent() )
        viewport()->ZoomToFit(sceneContent()->GetBoundingBox());
}

//-----------------------------------------------------------------------------
//
// ---
bool QtOpenGLSceneWidget::OnEvent(ProcessEvent* event)
{
    if (event->GetType() == ProcessEvent::Draw)
        repaint();
    return Object::OnEvent(event);
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::showEvent(QShowEvent* event)
{
    VSN_D(QtOpenGLSceneWidget);
    if (!d.m_bVsnInitialized)
        return;
    d.m_ptrGraphicsView->PreparingToDisplay();
    d.renderLater();
}

//-----------------------------------------------------------------------------
//
// ---
void QtOpenGLSceneWidget::closeEvent(QCloseEvent* event)
{
    QtOpenGLWidget::closeEvent(event);
}

//-----------------------------------------------------------------------------
// Creating processes to control the camera.
// ---
QT_FUNC(void) createProcessesCameraControls(QtOpenGLSceneWidget* pGLWidget, ProcessTypes prType)
{
    Essence* pEssence = pGLWidget->graphicsEngine()->GetTopEssence();
    Q_ASSERT(pEssence != nullptr);

    // Create and add process Orbit
    if (prType.CheckFlag(pt_Orbit))
    {
        PrCameraOrbit* pProcess = new PrCameraOrbit(pEssence);
        pProcess->SetGraphicsScene(pGLWidget->graphicsView()->GetGraphicsScene());
        pProcess->SetMouseButtonsActivate({ VSN::mb_RButton });
    }
    // Create and add process Rotate
    if (prType.CheckFlag(pt_Rotate))
    {
        PrCameraRotate* pProcess = new PrCameraRotate(pGLWidget->graphicsView().get(), pEssence);
        pProcess->SetGraphicsScene(pGLWidget->graphicsView()->GetGraphicsScene());
        pProcess->SetMouseButtonsActivate({ VSN::mb_RButton });
    }
    // Create and add process Rotate
    if (prType.CheckFlag(pt_ComplexRotation))
    {
        PrCameraComplexRotation* pProcess = new PrCameraComplexRotation(pGLWidget->graphicsView().get(), pEssence);
        pProcess->SetGraphicsScene(pGLWidget->graphicsView()->GetGraphicsScene());
        pProcess->SetMouseButtonsActivate({ VSN::mb_RButton });
    }
    // Create and add process Pan
    if (prType.CheckFlag(pt_Pan))
    {
        PrCameraPan* pProcess = new PrCameraPan(pEssence);
        pProcess->SetGraphicsScene(pGLWidget->graphicsView()->GetGraphicsScene());
        pProcess->SetMouseButtonsActivate({ VSN::mb_MButton });
    }
    // Create and add process Zoom
    if (prType.CheckFlag(pt_Zoom))
    {
        PrCameraZoom* pProcess = new PrCameraZoom(pGLWidget->graphicsView()->GetGraphicsScene(), pEssence);
        VSN_UNUSED(pProcess);
    }
}

//-----------------------------------------------------------------------------
// Setting the format for QSurface.
// ---
QT_FUNC(void) setSurfaceFormat()
{
    QSurfaceFormat format;
#ifdef QT_OPENGL_ES_2
    format.setRenderableType(QSurfaceFormat::OpenGLES);
#else
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) 
    {
//        format.setVersion(4, 3);
//        format.setProfile(QSurfaceFormat::CoreProfile);
    }
#endif
    format.setDepthBufferSize(24);
    format.setSamples(4);
    format.setStencilBufferSize(8);
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);
}

//-----------------------------------------------------------------------------
// License check
// ---
QT_FUNC(bool) activateLicense()
{
    VSN_ASSERT_X(QApplication::instance() != nullptr, "QtVision::activateLicense()", "QtVision::activateLicense() should be called after QApplication created!");

    QProcessEnvironment procEnv = QProcessEnvironment::systemEnvironment();
    QString envKey = procEnv.value("C3DKey");
    QString envSignature = procEnv.value("C3Dsignature");
    if (!envKey.isEmpty() && !envSignature.isEmpty())
    {
        const std::string key = envKey.toStdString();
        const std::string signature = envSignature.toStdString();

        ::EnableMathModules(key.c_str(), static_cast<int>(key.length()), signature.c_str(), static_cast<int>(signature.length()));
    }

    if (!IsMathVisionEnable())
    {
        QtVision::LicenseWnd licenseWnd;
        return licenseWnd.Activate();
    }

    return true;
}

/// \ru Позиционирование главного окна. \en Positioning the main window.
QT_FUNC(void) setWindowPosition(QWidget& widget)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    const QRect availableGeometry = widget.screen()->availableGeometry();
#else
    const QRect availableGeometry = QApplication::desktop()->availableGeometry();
#endif
    widget.resize(QSize(2 * availableGeometry.width() / 3, 2 * availableGeometry.height() / 3));
    widget.move(QPoint((availableGeometry.width() - widget.width()) / 2, (availableGeometry.height() - widget.height()) / 2));
}

} // namespace QtVision
VSN_END_NAMESPACE
