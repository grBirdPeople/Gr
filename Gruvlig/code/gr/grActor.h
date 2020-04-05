#ifndef		_GRACTOR_H_
#define		_GRACTOR_H_

#include	<SFML/Window/Mouse.hpp>

#include	"grDefine.h"
#include	"grMap.h"
#include	"grNavMesh.h"
#include	"grNavNode.h"


class grActor
{
public:

	grActor( grMap* pMap )
		: m_pStart	( pMap->GetNavMesh()->GetNavNodes()[ 0 ] )
		, m_pEnd	( pMap->GetNavMesh()->GetNavNodes()[ 0 ] )
	{}
	
	inline void SetStart( grMap* pMap, const grV2f& rPos ) { if ( pMap->FindValidPos( rPos ) != nullptr ) { m_pStart = pMap->FindValidPos( rPos ); } }
	inline void SetEnd( grMap* pMap, const grV2f& rPos ) { if ( pMap->FindValidPos( rPos ) != nullptr ) { m_pEnd = pMap->FindValidPos( rPos ); } }
	inline void FindPath( grMap* pMap ) { pMap->GetNavMesh()->FindPath( m_pStart->m_Id, m_pEnd->m_Id ); }

private:

	grNavNode*	m_pStart;
	grNavNode*	m_pEnd;

};

#endif		//_GRACTOR_H_

