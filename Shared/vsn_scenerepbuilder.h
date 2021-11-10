#ifndef __VSN_SCENEREPBUILDER_H
#define __VSN_SCENEREPBUILDER_H

#include <unordered_map>
#include <vsn_vision.h>

// en
// W A R N I N G 
//
// This file is not part of the C3D Vision API. It exists only as the implementation of certain tasks. 
// This header file may change from version to version without notice, or even be removed.
//

// ru
// NO TRANSLATION.

class MbItem;
class MbModel;
class MbSolid;
typedef std::unordered_map<MbSolid*, SceneSegmentRef*> SolidHash;

/* BuilderRepresentation */
class SceneRepresentationBuilder
{
public:
  explicit SceneRepresentationBuilder(MbModel* pMathModel, SceneSegment* pTopSegment);
  virtual ~SceneRepresentationBuilder();
public:
  void BuildScene();

protected:
  void Build( const MbItem* pItem, SceneSegment* pParent, const MbMatrix3D& mx, SolidHash& mapSolids );
  MbItem* CreateMesh( const MbSolid* pSolid );

private:
  MbModel* m_pModel;
  SceneSegment* m_pTopSegment;
};

#endif /* __VSN_SCENEREPBUILDER_H */
                                                                                                                                            