#include	"grDebugManager.h"

#include	<SFML/System/Vector2.hpp>

#include	"grDefine.h"
#include	"grNavMeshManager.h"


// AddBox
//////////////////////////////////////////////////
void
grDebugManager::AddBBox( const grBBox& rBBox, const sf::Color color )
{
	grBBox box = rBBox;
	
	sf::Vector2f topLeft( box.GetTopLeftCorner().x, box.GetTopLeftCorner().y );
	sf::Vector2f topRight		= topLeft;
	sf::Vector2f bottomRight	= topLeft;
	sf::Vector2f bottomLeft		= topLeft;
	
	topRight.x		+= box.GetDimension().x;
	bottomRight.x	+= box.GetDimension().x;
	bottomRight.y	+= box.GetDimension().y;
	bottomLeft.y	+= box.GetDimension().y;
	
	sf::VertexArray vertexBox( sf::LinesStrip, 5 );
	
	vertexBox[ 0 ].position	= topLeft;
	vertexBox[ 1 ].position	= topRight;
	vertexBox[ 2 ].position	= bottomRight;
	vertexBox[ 3 ].position	= bottomLeft;
	vertexBox[ 4 ].position	= topLeft;
	
	vertexBox[ 0 ].color	= color;
	vertexBox[ 1 ].color	= color;
	vertexBox[ 2 ].color	= color;
	vertexBox[ 3 ].color	= color;
	vertexBox[ 4 ].color	= color;
		
	m_VecBBoxDebug.push_back( vertexBox );
}

