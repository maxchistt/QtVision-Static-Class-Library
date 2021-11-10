#include <vsn_prcameramanipulate.h>
#include <vsn_graphicssceneview.h>
#include <assert.h> 

//-----------------------------------------------------------------------------
// конструктор по умолчанию
// ----
PrCameraManipulate::PrCameraManipulate(GraphicsSceneView* pGraphicsSceneView)
  : m_pGraphicsSceneView(pGraphicsSceneView)
  , m_pProcessManager(new ProcessManager)
  , m_bLButtonDown(false)
{
  m_pProcessManager->AddProcess(new PrCameraPan(m_pGraphicsSceneView->GetViewport()), ProcessManager::PR_Panoramic);
  m_pProcessManager->AddProcess(new PrCameraZoom(m_pGraphicsSceneView->GetViewport()), ProcessManager::PR_Zoom);
  m_pProcessManager->AddProcess(new PrCameraRotate(m_pGraphicsSceneView->GetViewport()), ProcessManager::PR_Rotate);
  m_pProcessManager->AddProcess(new PrCameraRotate(m_pGraphicsSceneView->GetViewport()), ProcessManager::PR_Orbit);
}

//-----------------------------------------------------------------------------
// деструктор
// ----
PrCameraManipulate::~PrCameraManipulate()
{
  delete m_pProcessManager;
}

//-----------------------------------------------------------------------------
//
// ----
void PrCameraManipulate::Update()
{
  m_pGraphicsSceneView->Update();
}

//-----------------------------------------------------------------------------
//
// ----
int PrCameraManipulate::OnLButtonDownPostMove(MouseEvent& event)
{
  CameraEvent cameraEvent(static_cast<double>(event.GetScreenPos().GetX()), static_cast<double>(event.GetScreenPos().GetY()));
  cameraEvent.SetZoomFactor(1.28);

  if (m_bLButtonDown)
  {
    m_bLButtonDown = false;
    if (event.Shift())
    {
      m_pProcessManager->ActivateProcess(ProcessManager::PR_Panoramic, &cameraEvent);
      return 0;
    }
    else if (event.Control())
    {
      m_pProcessManager->ActivateProcess(ProcessManager::PR_Zoom, &cameraEvent);
      return 0;
    }
    else
    {
      m_pProcessManager->ActivateProcess(ProcessManager::PR_Rotate, &cameraEvent);
      return 0;
    }
  }

  if (m_pProcessManager->IsExistActiveProcess())
  {
    m_pProcessManager->MoveActivateProcess(&cameraEvent);
    Update();
    return 0;
  }
  return -1;
}

//-----------------------------------------------------------------------------
//
// ----
int PrCameraManipulate::OnLButtonDown(MouseEvent& event)
{
  m_bLButtonDown = true;
  return -1;
}

//-----------------------------------------------------------------------------
//
// ----
int PrCameraManipulate::OnLButtonUp(MouseEvent& event)
{
  if (m_pProcessManager->IsExistActiveProcess())
    m_pProcessManager->DeactivateProcess();
  m_bLButtonDown = false;
  return -1;
}

//-----------------------------------------------------------------------------
//
// ----
int PrCameraManipulate::OnMouseWheel(WheelEvent& event)
{
  if (event.GetWheelDelta() > 0)
  {
    double factor = 1.28;
    m_pGraphicsSceneView->GetCamera()->SetZoom(factor);
  }
  else
  {
    double factor = 1.0 / 1.28;
    m_pGraphicsSceneView->GetCamera()->SetZoom(factor);
  }
  Update();

  return -1;
}
