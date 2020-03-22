#ifndef		_GRNAVNODE_H_
#define		_GRNAVNODE_H_

#define		ADJACENT_NODES	8

#include	"grCommon.h"
#include	"grDefine.h"
#include	"grV2.h"


// grNavNode
//////////////////////////////////////////////////
class grNavNode
{
public:

	grNavNode	( const grV2f pos, const uInt id )
		: m_pArrNeighbour		{}
		, m_ArrNeighbourCostG	{}
		, m_MidPos				( pos )
		, m_CostG				( 0.0f )
		, m_CostF				( 0.0f )
		, m_Id					( id )
		, m_NumOfNeighbours		( 0 )
		, m_pParent				( nullptr )
	{
		for( uInt i = 0; i < ADJACENT_NODES; ++i )
		{
			m_pArrNeighbour[ i ] = nullptr;
		}

		for( uInt i = 0; i < ADJACENT_NODES; ++i )
		{
			m_ArrNeighbourCostG[ i ] = 0;
		}
	}

	~grNavNode	( void )
	{
		for ( uInt i = 0; i < ADJACENT_NODES; ++i )
		{
			m_pArrNeighbour[ i ] = nullptr;
		}

		m_pParent = nullptr;
	}

	//////////////////////////////////////////////////

	grNavNode*		m_pArrNeighbour			[ ADJACENT_NODES ];
	float			m_ArrNeighbourCostG		[ ADJACENT_NODES ];	// Distance from this node to neighbour node

	grV2f			m_MidPos;									// Origo
	
	float			m_CostG;									// Previous nodes G + the distance from previous node to this node
	float			m_CostF;									// G + H (heuristic calc in NavMesh.hpp)
	
	uInt			m_Id;
	uInt			m_NumOfNeighbours;
	
	grNavNode*		m_pParent;

};

#endif		//_GRNAVNODE_H_

