#include <vsn_scenerepbuilder.h>

#include <model.h>
#include <instance.h>
#include <assembly.h>
#include <solid.h>
#include <mesh.h>

/* SceneRepresentationBuilder */
SceneRepresentationBuilder::SceneRepresentationBuilder(MbModel* pModel, SceneSegment* pTopSegment)
  : m_pModel( pModel )
  , m_pTopSegment( pTopSegment )
{
}

//------------------------------------------------------------------------------
//
// ---
SceneRepresentationBuilder::~SceneRepresentationBuilder()
{
}

//------------------------------------------------------------------------------
//
// ---
void SceneRepresentationBuilder::BuildScene()
{
  if ( m_pModel == V_NULL || m_pTopSegment == V_NULL )
    return;

  MbMatrix3D mx;
  SolidHash mapSolids;
  MbModel::ItemConstIterator drawIter( m_pModel->CBegin() );
  MbModel::ItemConstIterator drawEnd ( m_pModel->CEnd() );
  for ( ; drawIter != drawEnd; ++drawIter ) 
    Build( *drawIter, m_pTopSegment, mx, mapSolids );
}

//------------------------------------------------------------------------------
//
// ---
void SceneRepresentationBuilder::Build( const MbItem* pItem, SceneSegment* pParent, const MbMatrix3D& mx, SolidHash& mapSolids )
{
  if ( pItem == V_NULL )
    return;

  if ( pItem->IsA() == st_Solid ) 
  {
    MbSolid* pSolid = (MbSolid*)pItem; 
    std::unordered_map<MbSolid*, SceneSegmentRef*>::const_iterator it = mapSolids.find(pSolid);
    if (it != mapSolids.cend())
    {
      SceneSegmentRef* pSegmentRef = it->second;
      SceneSegment* pNewSegment = new SceneSegment( new SceneSegmentData(pSegmentRef) );
      pNewSegment->CreateRelativeMatrix( mx );
      pParent->AddSegment(pNewSegment);
    }
    else
    {
      MbMesh* pMesh = V_NULL;
      if ( MbItem* pItemMesh = CreateMesh(pSolid) )
      {
        ::AddRefItem( pItemMesh );
        if ( pItemMesh->IsA() == st_Mesh )
          pMesh = (MbMesh*)pItemMesh;
        else
        {
          ::ReleaseItem( pItemMesh );
          return;
        }
      }

      if ( pMesh != V_NULL )
      {
        SceneSegmentRef* pSegmentRef = new SceneSegmentRef( SolidObject::CreateGeometryRep3D(pMesh) );
        SceneSegment* pNewSegment = new SceneSegment( new SceneSegmentData(pSegmentRef) );
        pNewSegment->CreateRelativeMatrix( mx );
        pParent->AddSegment(pNewSegment);

        mapSolids.insert(std::pair<MbSolid*, SceneSegmentRef*>(pSolid, pSegmentRef));
        ::ReleaseItem( pMesh );
      }
    }
  }
  else if ( pItem->IsA() == st_Instance ) 
  {
    const MbInstance* pInstance = (const MbInstance*)pItem; 
    Build( pInstance->GetItem(), pParent, pInstance->GetPlacement().GetMatrixFrom() * mx, mapSolids );
  }
  else if ( pItem->IsA() == st_Assembly ) 
  {
    const MbAssembly* pAssembly = (const MbAssembly*)pItem; 
    for ( size_t i = 0, iCount = pAssembly->ItemsCount(); i < iCount; i++ ) 
      Build( pAssembly->GetItem(i), pParent, mx, mapSolids );
  }
}

//------------------------------------------------------------------------------
//
// ---
MbItem* SceneRepresentationBuilder::CreateMesh( const MbSolid* pSolid )
{
  MbRegDuplicate* iReg = V_NULL;
  const MbFormNote note;
  if ( MbItem* pItem = pSolid->CreateMesh( MbStepData(ist_SpaceStep, Math::visualSag), note, iReg) )
    return pItem;
  return V_NULL;
}



