#ifndef		_GRDEBUGMANAGER_H_
#define		_GRDEBUGMANAGER_H_

#include	<SFML/Graphics/VertexArray.hpp>
#include	<SFML/Graphics/VertexBuffer.hpp>
#include	<SFML/Graphics/RenderWindow.hpp>
#include	<SFML/System/Vector2.hpp>

#include	"grBBox.h"
#include	"grCommon.h"
#include	"grDefine.h"
#include	"grSingleton.h"

class		grBBox;


// grDebugManager
//////////////////////////////////////////////////
class grDebugManager : public grSingleton<grDebugManager>
{
public:
	
	grDebugManager()
		: m_puVertices( std::make_unique<sf::VertexArray[]>( 10000 ) )
		, m_MaxBBox( 10000 )
		, m_Size( 0 )
		, m_bEnable( true )
	{}

	inline const bool IsEnable() const
	{
		return m_bEnable;
	}

	inline void Enable( const bool enable )
	{
		m_bEnable = enable;
	}

	inline void AddBBox( const grBBox& rBBox, const sf::Color color )
	{
#ifdef DEBUG
		if ( m_Size >= m_MaxBBox && m_bEnable == true )
		{
			std::cerr << "grDebugManager::AddBBox: Max BBoxes added or this is not enabled\n" << std::endl;
			return;
		}
#endif // DEBUG

		const grBBox::Corners& corners = rBBox.GetCorners();
		sf::Vector2f topLeft( corners.TopLeft.x, corners.TopLeft.y );
		sf::Vector2f topRight( corners.TopRight.x, corners.TopRight.y );
		sf::Vector2f botRight( corners.BotRight.x, corners.BotRight.y );
		sf::Vector2f botLeft( corners.BotLeft.x, corners.BotLeft.y );

		sf::VertexArray vertexBox( sf::LinesStrip, 5 );

		vertexBox[ 0 ].position = topLeft;
		vertexBox[ 1 ].position = topRight;
		vertexBox[ 2 ].position = botRight;
		vertexBox[ 3 ].position = botLeft;
		vertexBox[ 4 ].position = topLeft;

		vertexBox[ 0 ].color = color;
		vertexBox[ 1 ].color = color;
		vertexBox[ 2 ].color = color;
		vertexBox[ 3 ].color = color;
		vertexBox[ 4 ].color = color;

		m_puVertices[ m_Size++ ] = vertexBox;
	}

	inline void Render( sf::RenderWindow& rRenderWin )
	{
		if ( m_Size > 0 && m_bEnable == true )
		{
			for( sizeT i = 0; i < m_Size; ++i )
				rRenderWin.draw( m_puVertices[ i ] );
		}

		m_Size = 0;
	}

private:
	pU<sf::VertexArray[]> m_puVertices;
	sizeT m_MaxBBox, m_Size;
	bool m_bEnable;
};

#endif		//_GRDEBUGMANAGER_H_
