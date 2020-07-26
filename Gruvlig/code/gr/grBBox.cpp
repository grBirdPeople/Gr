#include	"grBBox.h"

#include	"grDefine.h"
#include	"grMath.h"
			

// cTor
//////////////////////////////////////////////////
grBBox::grBBox( const grV2f& rDimension, const grV2f& rPos )
	: m_Dimensions( rDimension )
	, m_MidPos( rPos )
{
	UpdateCorners();
}


// UpdateCorners
//////////////////////////////////////////////////
inline void
grBBox::UpdateCorners()
{
	float halfDimensionX = m_Dimensions.x * 0.5f;
	float halfDimensionY = m_Dimensions.y * 0.5f;

	m_Corners.TopLeft.x = m_MidPos.x - halfDimensionX;
	m_Corners.TopLeft.y = m_MidPos.y - halfDimensionY;

	m_Corners.TopRight.x = m_MidPos.x + halfDimensionX;
	m_Corners.TopRight.y = m_Corners.TopLeft.y;

	m_Corners.BotRight.x = m_Corners.TopRight.x;
	m_Corners.BotRight.y = m_MidPos.y + halfDimensionY;

	m_Corners.BotLeft.x = m_Corners.TopLeft.x;
	m_Corners.BotLeft.y = m_Corners.BotRight.y;
}