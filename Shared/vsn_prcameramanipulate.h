////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Класс PrCameraManipulate является оберткой для процессов манипуляции с камерой.
         \en PrCameraManipulate class is a wrapper for camera manipulation processes. \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_PRCAMERAMANIPULATE_H
#define __VSN_PRCAMERAMANIPULATE_H

// en
// W A R N I N G 
//
// This file is not part of the C3D Vision API. It exists only as the implementation of certain tasks. 
// This main file may change from version to version without notice, or even be removed.
//

// ru
// В Н И М А Н И Е
//
// Этот файл не является частью C3D Vision API. Он существует только в виде реализации некоторых задач.
// Этот главный файл может изменяться с каждой версией без замечания или даже может быть удален.
//


#include <vsn_vision.h>
#include <vsn_window.h>

class GraphicsSceneView;
//------------------------------------------------------------------------------
/** \brief   \ru Класс PrCameraManipulate является оберткой для процессов манипуляции с камерой.
             \en PrCameraManipulate class is a wrapper for camera manipulation processes. \~
    \ingroup NO GROUP
*/
//---
class PrCameraManipulate
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  explicit PrCameraManipulate(GraphicsSceneView* pGraphicsSceneView);
  /// \ru Деструктор. \en Destructor.
  virtual ~PrCameraManipulate();

protected:
  virtual void Update();

public:
  /** \brief \ru Эта функция управляет нажатием левой кнопки мыши и перемещением мыши. Она проверяет, нажаты ли клавиши Shift и Ctrl,
                 а затем передает управление таким процессам как панорамирование, вращение по орбите или масштабирование.
             \en The function controls pressing the left mouse button and mouse move. It checks if Shift or
                 Ctrl key is pressed and then passes control over such processes as panning, orbiting or zooming. \~
    \param[in] event- \ru MouseEvent содержит информацию о текущем событии.
                      \en MouseEvent contains information about the current event. \~
    \return \ru Возвращает статус события. \en Returns event state.
  */
  virtual int OnLButtonDownPostMove(MouseEvent& event);

  /** \brief \ru Эта функция управляет нажатием левой кнопки мыши для манипуляций с камерой.
             \en The function controls pressing the left mouse button for camera manipulation. \~
      \param[in] event- \ru MouseEvent содержит информацию о текущем событии.
                        \en MouseEvent contains information about the current event. \~
      \return \ru Возвращает статус события. \en Returns event state.
  */
  virtual int OnLButtonDown(MouseEvent& event);

  /** \brief \ru Эта функция управляет отпусканием левой кнопки мыши, завершающим любую активность выбора объекта и манипуляции с камерой.
             \en The function controls releasing the left mouse button for finalizing any object selection activity and camera manipulation. \~
      \param[in] event- \ru MouseEvent содержит информацию о текущем событии.
                        \en MouseEvent contains information about the current event. \~
      \return \ru Возвращает статус события. \en Returns event state.
  */
  virtual int OnLButtonUp(MouseEvent& event);

  /** \brief \ru Эта функция управляет действиями с колесиком мыши для манипуляций с камерой.
             \en The function controls mouse wheel actions for camera manipulation. \~
      \param[in] event- \ru MouseEvent содержит информацию о текущем событии.
                        \en MouseEvent contains information about the current event. \~
	    \return \ru Возвращает статус события. \en Returns event state.
  */
	virtual int OnMouseWheel(WheelEvent& event);

protected:
  GraphicsSceneView*  m_pGraphicsSceneView;
  ProcessManager* m_pProcessManager;
  bool m_bLButtonDown;
};

#endif // __VSN_PRCAMERAMANIPULATE_H
