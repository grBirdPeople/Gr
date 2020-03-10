#include	"grNavMeshManager.h"

#include	<iostream>

#include	"grDefine.h"
#include	"grNavMesh.h"
#include	"grMap.h"


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