#ifndef		_GRMAPMANAGER_H_
#define		_GRMAPMANAGER_H_

#define		MAX_MAP	16

#include	"grCommon.h"
#include	"grSingleton.h"

class		grMap;

typedef		std::vector<grMap*>	vecMap;


// grMapManager
//////////////////////////////////////////////////
class grMapManager : public grSingleton<grMapManager>
{
public:
			grMapManager	( void )						{ m_pVecMap.reserve( MAX_MAP );	CreateMaps(); }
			~grMapManager	( void )						{ for ( auto& i : m_pVecMap ) { if ( i != nullptr ) { DELANDNULL( i ); } } }
	
	//////////////////////////////////////////////////
	
	grMap*	GetMap			( const str& uniqueName );

	//////////////////////////////////////////////////

private:
	
	void	CreateMaps		( void );
	
	//////////////////////////////////////////////////
	
	vecMap	m_pVecMap;

};

#endif		//_GRMAPMANAGER_H_
