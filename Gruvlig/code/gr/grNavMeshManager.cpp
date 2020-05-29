#include	"grNavMeshManager.h"

#include	<iostream>

#include	"grDefine.h"
#include	"grMap.h"


// SetNavMeshRenderIdx // Set to minus anything int to avoid running any navmesh debug
//////////////////////////////////////////////////
void grNavMeshManager::SetNavMeshToDebug( const intS idx )	// TODO: Is first in now. Would be nice to have some simple generic id system to use in classes whenever
{
	m_NavMeshRenderIdx = idx;
}


// GetNavMesh
//////////////////////////////////////////////////
grNavMesh*
grNavMeshManager::GetNavMesh( const str& name )
{
	for( auto& mesh : m_VecNavMesh )
	{
		if( mesh->GetOwnerMap()->GetMapData()->Name == name ) { return mesh; }
	}
	
#ifdef DEBUG
	std::cerr << "grNavMeshManager::GetNavMesh(): No map exists with name '" << name << "'" << std::endl;
#endif	// DEBUG

	return nullptr;
}


// CreateNavMesh
//////////////////////////////////////////////////
grNavMesh*
grNavMeshManager::CreateNavMesh( grMap* pMap )
{
	m_VecNavMesh.push_back( new grNavMesh( pMap ) );
	return m_VecNavMesh[ m_VecNavMesh.size() - 1 ];
}


void grNavMeshManager::Debug( void )
{
	if ( m_NavMeshRenderIdx > -1 )
	{
		m_VecNavMesh[ m_NavMeshRenderIdx ]->Debug();
	}
}
