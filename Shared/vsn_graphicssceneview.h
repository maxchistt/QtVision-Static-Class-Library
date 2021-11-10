////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru.
         \en. \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_GraphicsSceneViewVIEW_H
#define __VSN_GraphicsSceneViewVIEW_H

// en
// W A R N I N G 
//
// This file is not part of the C3D Vision API. It exists only as the implementation of certain tasks. 
// This main file may be changed from version to version without notice, or even be removed.
//

// ru
// В Н И М А Н И Е
//
// Этот файл не является частью C3D Vision API. Он существует только в виде реализации некоторых задач.
// Этот главный файл может изменяться с каждой версией без замечания или даже может быть удален.



#include <vsn_vision.h>
#include <vsn_window.h>

class PrCameraManipulate;
//------------------------------------------------------------------------------
/** \brief   \ru .
             \en . \~
    \ingroup NO GROUP
*/
//---
class GraphicsSceneView
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  GraphicsSceneView(void* pWindowHandle, SceneContent* pSceneContent);
  /// \ru Деструктор. \en Destructor.
  virtual ~GraphicsSceneView();
public:
  /// \ru Эта функция инициализирует вид. \en The function initializes view.
  virtual void Initialize();
  /// \ru Эта функция разрушает вид и его контекст. \en The function destroys view and its context.
  virtual void Destroy();

public:
  virtual void MakeCurrent();
  virtual void DoneCurrent();
  virtual void PaintOpenGL();
  virtual void Render();

  virtual void Update();
  virtual void UpdateResize(int cx, int cy);


public:
  /// \ru Вернуть признак использования VBO. \en Returns flag of Vertex Buffer Object (VBO) usage.
  static bool IsVertexBufferObject();
  /// \ru Установить признак использования VBO. \en Sets flag of VBO usage.
  static void SetVertexBufferObject( bool set );

  /** \brief \ru Показать все относительно указанного центра, если он задан.
             \en If specified, shows everything about the given center. \~
      \param[in] center - \ru Если указатель равен null, то центр будет выбран как центр масс мира.
                          \en If pointer is null, then center will be selected as the world mass center. \~
  */
  virtual void FitWorld(MbCartPoint3D* center = NULL);

  /** \brief \ru Эта функция устанавливает ориентацию вида. По умолчанию ориентация является плоскостью XY.
             \en The function sets view orientation. The default orientation is XY plane. \~
      \param[in] orientation - \ru Новая ориентация вида.
                               \en New view orientation. \~
      \param[in] fitworld - \ru Если true, то показать все.
                            \en If true, then fits to all. \~
  */
  virtual void  SetViewOrientation(Orientation orientation, bool fitWorld = true);

  /** \brief \ru Установить камеру для сцены.
             \en Sets the new camera for the scene. \~
      \param[in] camera - \ru Новая камера.
                          \en New camera. \~
  */
  virtual void SetCamera(Camera camera);

  /** \brief \ru Вернуть указатель на камеру.
             \en Returns pointer to the camera. \~
      \return \ru Вернуть указатель на камеру с ее текущими параметрами. \en Returns pointer to the camera with its current parameters.
  */
  virtual Camera* GetCamera() const;

  /** \brief \ru Это функция возвращает указатель на Viewport.
             \en The function returns pointer to viewport. \~
     \return \ru Вернуть указатель на Viewport. \en Returns pointer to viewport.
  */
  Viewport* GetViewport() const;

public:
  /** \brief \ru Эта функция вызывается при нажатии левой кнопки мыши с последующим ее движением.
             \en The function is called when pressing the left mouse button with following mouse move. \~
      \param[in] event- \ru MouseEvent содержит информацию о текущем событии.
                        \en MouseEvent contains information about the current event. \~
      \return \ru Возвращает статус события. \en Returns event state.
  */
  virtual int OnLButtonDownPostMove(MouseEvent& event);

  /** \brief \ru Эта функция вызывается при нажатии левой кнопки мыши.
             \en The function is called when pressing the left mouse button. \~
      \param[in] event- \ru MouseEvent содержит информацию о текущем событии.
                        \en MouseEvent contains information about the current event. \~
      \return \ru Возвращает статус события. \en Returns event state.
  */
  virtual int OnLButtonDown(MouseEvent& event);

  /** \brief \ru Эта функция вызывается при движении мыши.
             \en The function is called while moving mouse. \~
      \param[in] event- \ru MouseEvent содержит информацию о текущем событии.
                        \en MouseEvent contains information about the current event. \~
      \return \ru Возвращает статус события. \en Returns event state.
  */
  virtual int OnMouseMove(MouseEvent& event);

  /** \brief \ru Эта функция вызывается при отпускании левой кнопки мыши.
             \en The function is called when releasing the left mouse button. \~
      \param[in] event- \ru MouseEvent содержит информацию о текущем событии.
                        \en MouseEvent contains information about the current event. \~
      \return \ru Возвращает статус события. \en Returns event state.
  */
  virtual int OnLButtonUp(MouseEvent& event);

  /** \brief \ru Эта функция вызывается при работе с колесиком мыши.
             \en The function is called while working with the mouse wheel. \~
      \param[in] event- \ru MouseEvent содержит информацию о текущем событии.
                        \en MouseEvent contains information about the current event. \~
      \return \ru Возвращает статус события. \en Returns event state.
  */
  virtual int OnMouseWheel(WheelEvent& event);

protected:
  void* m_pWindowHandle;
  bool  m_bInitialized;
  bool  m_bInPaintOpenGL;
  bool  m_bLButtonDown;
  bool  m_bMouseMove;

  Light*           m_pLight;
  Viewport*        m_pViewPort;
  SceneContent*    m_pSceneContent;
  PrCameraManipulate* m_pPrManipulate;
  Window*          m_pWindow;
  OpenGLContext*   m_pContext;
  RenderingAreaFormat      m_requestedFormat;
};

#endif /* __VSN_GraphicsSceneViewVIEW_H */
