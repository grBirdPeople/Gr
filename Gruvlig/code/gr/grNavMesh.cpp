#include	"grNavMesh.h"

#include	"grBBox.h"
#include	"grDebugManager.h"
#include	"grDefine.h"
#include	"grMap.h"
#include	"grNavNode.h"


// dTor
//////////////////////////////////////////////////
grNavMesh::~grNavMesh(void)
{
	for ( auto& node : m_VecPath )
	{
		node = nullptr;
	}

	for ( uInt i = 0; i < m_VecNode.size(); ++i )
	{
		DELANDNULL( m_VecNode[ i ] );
	}
}


// CreateNavMesh
//////////////////////////////////////////////////
void
grNavMesh::CreateNavMesh()
{
	str strMap			= m_pOwnerMap.GetMapData()->StrMap;
	grV2f tileSize		= m_pOwnerMap.GetMapData()->TileSize;
	grV2f nodeMidPos	= tileSize * 0.5f;
	uInt nodesX			= m_pOwnerMap.GetMapData()->TilesX;
	uInt nodesY			= m_pOwnerMap.GetMapData()->TilesY;

	uInt mapIndex	= 0;
	uInt nodeIndex	= 0;
	
	uInt reserveSize = nodesX * nodesY;
	m_VecNode.reserve( reserveSize );
	m_VecPath.reserve( reserveSize );
	
	for( uInt y = 0; y < nodesY; ++y )
	{
		for( uInt x = 0; x < nodesX; ++x )
		{
			if( strMap[ mapIndex ] == '#' )
			{
				grV2f gridPos	= tileSize;
				gridPos.x		*= x;
				gridPos.y		*= y;
				gridPos			-= nodeMidPos;
				
				m_VecNode.push_back( new grNavNode( gridPos, nodeIndex ) );
				++nodeIndex;
			}
			
			++mapIndex;
		}
	}
	
	//////////////////////////////////////////////////
	
	// Find neighbouring node(s) & their respective weight
	float maxPossibleNeighbourDist = m_pOwnerMap.GetMapData()->TileSize.Magnitude() + 0.1f;	// + 0.1f because <= fux
	grNavNode* pNeighbourNode;
	
	// Not very optimized but it runs once on init so eh
	for( uInt i = 0; i < m_VecNode.size(); ++i )
	{
		grNavNode* pCurrentNode = m_VecNode[ i ];
		
		for( uInt j = 0; j < m_VecNode.size(); ++j )
		{
			if ( j == i ) { continue; }
			
			pNeighbourNode			= m_VecNode[ j ];
			float dist				= ( pNeighbourNode->m_MidPos - pCurrentNode->m_MidPos ).Magnitude();
			uInt numOfNeighbours	= pCurrentNode->m_NumOfNeighbours;
			
			if( dist < maxPossibleNeighbourDist )
			{
				pCurrentNode->m_ArrNeighbourCostG[ numOfNeighbours ]	= dist;
				pCurrentNode->m_pArrNeighbour[ numOfNeighbours ]		= pNeighbourNode;
				
				pCurrentNode->m_NumOfNeighbours += 1;
				if ( pCurrentNode->m_NumOfNeighbours == 8 ) { break; }
			}
		}
	}
}


// FindPath
//////////////////////////////////////////////////
vecNavNode*
grNavMesh::FindPath( const uInt startPos, const uInt endPos )
{
	m_VecPath.clear();

#ifdef DEBUG
	if( AStar( m_VecNode[ startPos ], m_VecNode[ endPos ] ) == false )
	{
		std::cerr << "grNavMesh::FindPath: Path could not be found\n";
		return nullptr;	
	}
#endif
		
	return &m_VecPath;
}


// DebugRender
//////////////////////////////////////////////////
void
grNavMesh::Debug( void )
{
	if( m_VecNode.size() > 0 )
	{
		grV2f tileSize = m_pOwnerMap.GetMapData()->TileSize;

		// Map
		for( auto& node : m_VecNode )
		{
			grBBox box( tileSize, node->m_MidPos );
			grDebugManager::Instance().AddBBox( box, sf::Color::Red );
		}

		// A*
		grBBox startBox( tileSize, m_VecPath[ 0 ]->m_MidPos );
		grBBox endBox(tileSize, m_VecPath[ m_VecPath.size() - 1 ]->m_MidPos );
		grDebugManager::Instance().AddBBox( startBox, sf::Color::Yellow );
		grDebugManager::Instance().AddBBox( endBox, sf::Color::Yellow );

		for ( int i = 1; i < m_VecPath.size() - 1; ++i )
		{
			grBBox box(tileSize, m_VecPath[ i ]->m_MidPos );
			grDebugManager::Instance().AddBBox( box, sf::Color::Green );
		}
	}
}
	

// AStar
//////////////////////////////////////////////////
bool
grNavMesh::AStar( grNavNode* pStartNode, grNavNode* pEndNode )
{
	vecNavNode vecOpen;
	vecNavNode vecClose;
	
	pStartNode->m_CostG		= 0.0f;
	pStartNode->m_CostF		= Heuristic( pStartNode, pEndNode );
	pStartNode->m_pParent	= nullptr;
	vecOpen.push_back( pStartNode );

	while( vecOpen.size() > 0 )
	{
		InsertSortByCost( vecOpen );
		grNavNode* pCurrentNode = vecOpen[ vecOpen.size() - 1 ];
		vecOpen.resize( vecOpen.size() - 1 );
		vecClose.push_back( pCurrentNode );
		
		if( pCurrentNode->m_Id == pEndNode->m_Id )
		{
			BuildNodePath( pCurrentNode );
			return true;
		}
		
		uInt numOfNeighbours = pCurrentNode->m_NumOfNeighbours;
		for( uInt i = 0; i < numOfNeighbours; ++i )
		{
			grNavNode* neighbourNode = pCurrentNode->m_pArrNeighbour[ i ];
		
			if ( FindInOpenOrClose( neighbourNode->m_Id, vecClose ) == true )
			{ continue; }
			
			if( FindInOpenOrClose( neighbourNode->m_Id, vecOpen ) == true )
			{
				float newNeighourCostG 	= pCurrentNode->m_CostG + pCurrentNode->m_ArrNeighbourCostG[ i ];
				float oldNeighourCostF 	= neighbourNode->m_CostF;
				float newNeighourCostF	= newNeighourCostG + Heuristic( neighbourNode, pEndNode );
				
				if( newNeighourCostF < oldNeighourCostF )
				{
					neighbourNode->m_CostG		= newNeighourCostG;
					neighbourNode->m_CostF		= newNeighourCostF;
					neighbourNode->m_pParent	= pCurrentNode;
				}
			}
			else
			{
				neighbourNode->m_CostG		= pCurrentNode->m_CostG + pCurrentNode->m_ArrNeighbourCostG[ i ];
				neighbourNode->m_CostF		= neighbourNode->m_CostG + Heuristic( neighbourNode, pEndNode );
				neighbourNode->m_pParent	= pCurrentNode;
				vecOpen.push_back( neighbourNode );
			}
		}
	}

	return false;
}


// Heuristic
//////////////////////////////////////////////////
float
grNavMesh::Heuristic( grNavNode* pNode, grNavNode* pEndNode )
{
	// TODO: Make better math
	//return grMath::Abs( pNode->m_Pos.x - pEndNode->m_Pos.x ) + grMath::Abs( pNode->m_Pos.y - pEndNode->m_Pos.y );
	return ( pEndNode->m_MidPos - pNode->m_MidPos ).Magnitude();
}


// InsertSortCost
//////////////////////////////////////////////////
void
grNavMesh::InsertSortByCost( vecNavNode& rVec )
{
	if ( rVec.size() > 1 )
	{
		uInt vecSize			= ( uInt )rVec.size();
		sInt idxBefore			= 0;
		grNavNode* currentNode	= nullptr;

		for ( uInt idxNow = 1; idxNow < vecSize; ++idxNow )
		{
			currentNode	= rVec[ idxNow ];
			idxBefore	= idxNow - 1;

			float currentCost	= currentNode->m_CostF;
			float beforeCost	= rVec[ idxBefore ]->m_CostF;

			while ( ( currentCost > beforeCost ) && ( idxBefore > -1 ) )
			{
				rVec[ idxNow ]	= rVec[ idxBefore ];
				idxBefore		-= 1;

				if ( idxBefore > -1 )
				{ beforeCost = rVec[ idxBefore ]->m_CostF; }
			}

			rVec[ ++idxBefore ] = currentNode;
		}
	}
}


// BuildPath
//////////////////////////////////////////////////
inline void
grNavMesh::BuildNodePath( grNavNode* pEndNode )
{
	grNavNode* pNode = pEndNode;
	m_VecPath.push_back( pNode );
	
	while( pNode->m_pParent != nullptr )
	{
		pNode = pNode->m_pParent;
		m_VecPath.push_back( pNode );
	}
}


// FindInOpenClose
//////////////////////////////////////////////////
inline bool
grNavMesh::FindInOpenOrClose( const uInt nodeId, const vecNavNode& vec )
{
	for( uInt i = 0; i < vec.size(); ++i )
	{
		if ( nodeId == vec[ i ]->m_Id ) { return true; }
	}
	return false;
}

