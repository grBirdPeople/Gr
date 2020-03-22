#ifndef		_GRNAVMESH_H_
#define		_GRNAVMESH_H_

#include	"grCommon.h"
#include	"grV2.h"

class		grNavNode;
class		grMap;

typedef		std::vector<grNavNode*>	vecNavNode;
typedef		std::vector<grV2f>		vecPathPos;

#include	"grMap.h"


// grAiNavMesh
//////////////////////////////////////////////////
class grNavMesh
{
public:

	friend class grNavMeshManager;
	
	//////////////////////////////////////////////////

					grNavMesh			( grMap* pOwnerMap ) : m_pOwnerMap( *pOwnerMap )	{ CreateNavMesh(); }
					~grNavMesh			( void );

	//////////////////////////////////////////////////

	grMap*			GetOwnerMap			( void )											{ return &m_pOwnerMap; }
	vecNavNode&		GetNavNodes			( void )											{ return m_VecNode; }

	//////////////////////////////////////////////////
	
	void			CreateNavMesh		( void );
	vecNavNode*		FindPath			( const uInt startPos, const uInt endPos );
	void			DebugRender			( void );
	
	//////////////////////////////////////////////////
	
private:

	bool			AStar				( grNavNode* pStartNode, grNavNode* pEndNode );
	float			Heuristic			( grNavNode* pNode, grNavNode* pEndNode );
	void			InsertSortByCost	( vecNavNode& rVec );
	void			BuildNodePath		( grNavNode* pEndNode );
	bool			FindInOpenOrClose	( const uInt nodeId, const vecNavNode& vec );
	
	////////////////////////////////////////////////

	vecNavNode	m_VecNode;
	vecNavNode	m_VecPath;
	
	grMap&		m_pOwnerMap;

};

#endif		//_GRNAVMESH_H_
