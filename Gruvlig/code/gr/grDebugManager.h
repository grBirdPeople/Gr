#ifndef		_GRDEBUGMANAGER_H_
#define		_GRDEBUGMANAGER_H_


#include	<SFML/Graphics/VertexArray.hpp>
#include	<SFML/Graphics/RenderWindow.hpp>

#include	"grCommon.h"
#include	"grSingleton.h"

using		vecVertexArray = std::vector<sf::VertexArray>;

class		grBBox;


// grDebugManager
//////////////////////////////////////////////////
class grDebugManager : public grSingleton<grDebugManager>
{
public:
	
	grDebugManager( void )
	{
		m_VecBBoxDebug.reserve( 10000 );
	}

	//////////////////////////////////////////////////

	void AddBBox( grBBox& rBBox, const sf::Color color );
	inline void DebugRender( sf::RenderWindow& rRenderWin )
	{
		if ( m_VecBBoxDebug.size() > 0 )
		{
			for ( auto& i : m_VecBBoxDebug )
				rRenderWin.draw( i );

			m_VecBBoxDebug.clear();
		}
	}
	
	//////////////////////////////////////////////////

private:

	vecVertexArray	m_VecBBoxDebug;

};

#endif		//_GRDEBUGMANAGER_H_
