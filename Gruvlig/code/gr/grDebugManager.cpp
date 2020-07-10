#include	"grDebugManager.h"

#include	<SFML/System/Vector2.hpp>

#include	"grBBox.h"
#include	"grDefine.h"


// AddBox
//////////////////////////////////////////////////
void
grDebugManager::AddBBox( grBBox& rBBox, const sf::Color color )
{
	if ( m_VecBBoxDebug.size() >= m_MaxBBox )
	{
		std::cerr << "grDebugManager::AddBBox: Max BBoxes added\n" << std::endl;
	}

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

	m_VecBBoxDebug.push_back( vertexBox );
}
