#ifndef		_GRNAVMESHMANAGER_H_
#define		_GRNAVMESHMANAGER_H_

#define		MAX_NAVMESH	128

#include	"grCommon.h"
#include	"grNavMesh.h"
#include	"grSingleton.h"

class		grMap;

typedef		std::vector<grNavMesh*>	vecNavMesh;


// grNavMeshManager
//////////////////////////////////////////////////
class grNavMeshManager : public grSingleton<grNavMeshManager>
{
public:

	grNavMeshManager( void )
		: m_NavMeshRenderIdx	( -1 )
	{ m_VecNavMesh.reserve( MAX_NAVMESH ); }

	~grNavMeshManager( void )
	{
		for ( uInt i = 0; i < m_VecNavMesh.size(); ++ i )
		{ DELANDNULL( m_VecNavMesh[ i ] ); }
	}

	//////////////////////////////////////////////////

	void SetNavMeshToDebug( const sInt idx );
	
	//////////////////////////////////////////////////
	
	grNavMesh* GetNavMesh( const str& name );
	
	//////////////////////////////////////////////////
	
	grNavMesh* CreateNavMesh( grMap* pMap );
	void Debug( void );

	//////////////////////////////////////////////////

private:

	vecNavMesh	m_VecNavMesh;

	sInt m_NavMeshRenderIdx;
};

#endif		//_GRNAVMESHMANAGER_H_

