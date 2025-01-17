#include	"grMap.h"

#include	"grDefine.h"
#include	"grNavMesh.h"
#include	"grNavNode.h"
#include	"grNavMeshManager.h"


// cTor
//////////////////////////////////////////////////
grMap::grMap( const str& uniqueName,
			const str& strMap,
			const grV2f& tileSize,
			const intU numTilesX,
			const intU numTilesY )
	: m_pMapData	( new sMapData{ uniqueName, strMap, tileSize, numTilesX, numTilesY } )
	, m_pNavMesh	( nullptr )
{}


// CreateNavMesh
//////////////////////////////////////////////////
grMap::~grMap( void )
{
	if ( m_pNavMesh != nullptr )
	{
		m_pNavMesh = nullptr;
	}

	if ( m_pMapData != nullptr )
	{
		DELANDNULL( m_pMapData );
	}
}


// CreateNavMesh
//////////////////////////////////////////////////
void
grMap::CreateNavMesh( void )
{
	if( m_pNavMesh != nullptr )
	{ return; }

	m_pNavMesh = grNavMeshManager::Instance().CreateNavMesh( this );
	m_pNavMesh->FindPath( 0, 0 );	// Init pos
}


// IsValidPos
//////////////////////////////////////////////////
grNavNode*
grMap::FindValidPos( const grV2f& pos )
{
	if ( m_pNavMesh != nullptr )
	{
		float halfTileX = m_pMapData->TileSize.x * 0.5f;
		float halfTileY = m_pMapData->TileSize.y * 0.5f;

		for ( intU node = 0; node < m_pNavMesh->GetNavNodes().size(); ++node )
		{
			float minX = m_pNavMesh->GetNavNodes()[ node ]->m_MidPos.x - halfTileX;
			float maxX = m_pNavMesh->GetNavNodes()[ node ]->m_MidPos.x + halfTileX;
			float minY = m_pNavMesh->GetNavNodes()[ node ]->m_MidPos.y - halfTileY;
			float maxY = m_pNavMesh->GetNavNodes()[ node ]->m_MidPos.y + halfTileY;
		
			if( ( pos.x > minX ) && ( pos.x < maxX ) &&
				( pos.y > minY ) && ( pos.y < maxY ) )
			{
				return m_pNavMesh->GetNavNodes()[ node ];
			}
		}
	}

	return nullptr;
}
