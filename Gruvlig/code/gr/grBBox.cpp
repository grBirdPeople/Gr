#include	"grBBox.h"

#include	"grDefine.h"
#include	"grMath.h"
			

// cTor
//////////////////////////////////////////////////
grBBox::grBBox( const grV2f& rDimension, const grV2f& rPos )
	: m_Dimensions		( rDimension )
	, m_MidPos			( rPos )
{
	UpdateCorners();
}


// Intersect
//////////////////////////////////////////////////
const bool
grBBox::Intersects( const grBBox& rBBox )
{
	grBBox otherBox			= rBBox;
	grV2f otherPos			= otherBox.GetPosistion();
	grV2f otherDimensions	= otherBox.GetDimensions();
	grV2f otherTopLeft		= otherBox.GetCorners().TopLeft;

	return	m_Corners.TopLeft.x	< otherTopLeft.x		+ otherDimensions.x	&&
			otherTopLeft.x		< m_Corners.TopLeft.x	+ m_Dimensions.x	&&
			m_Corners.TopLeft.y	< otherTopLeft.y		+ otherDimensions.y	&&
			otherTopLeft.y		< m_Corners.TopLeft.y	+ m_Dimensions.y;
}


// PointInside
//////////////////////////////////////////////////
inline const bool
grBBox::IsPointInside(const grV2f& rPoint)
{
	if( ( rPoint.x > ( m_MidPos.x - m_Dimensions.x ) ) && ( rPoint.x < ( m_MidPos.x + m_Dimensions.x ) ) &&
		( rPoint.y > ( m_MidPos.y - m_Dimensions.y ) ) && ( rPoint.y < ( m_MidPos.y + m_Dimensions.y ) ) )
	{
		return true;
	}
	
	return false;
}


// UpdateCorners
//////////////////////////////////////////////////
inline void
grBBox::UpdateCorners( void )
{
	m_Corners.TopLeft.x = m_MidPos.x - ( m_Dimensions.x * 0.5f );
	m_Corners.TopLeft.y = m_MidPos.y - ( m_Dimensions.y * 0.5f );

	m_Corners.TopRight.x = m_MidPos.x + ( m_Dimensions.x * 0.5f );
	m_Corners.TopRight.y = m_Corners.TopLeft.y;

	m_Corners.BotRight.x = m_Corners.TopRight.x;
	m_Corners.BotRight.y = m_MidPos.y + ( m_Dimensions.y * 0.5f );

	m_Corners.BotLeft.x = m_Corners.TopLeft.x;
	m_Corners.BotLeft.y = m_Corners.BotRight.y;
}