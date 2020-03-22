#include	"grBBox.h"

#include	"grDefine.h"
#include	"grMath.h"
			

// cTor
//////////////////////////////////////////////////
grBBox::grBBox( const grV2f& rDimension, const grV2f& rPos )
	: m_Dimension		( rDimension )
	, m_MidPos			( rPos )
	, m_TopLeftCorner	( grV2f() )
	, m_RotationInDeg	( 15.0f )
{
	SetCorners( m_Dimension, m_MidPos );
}


grBBox::grBBox( const grV2f& rDimension, const grV2f& rPos, const float rotationInDeg )
	: m_Dimension		( rDimension )
	, m_MidPos			( rPos )
	, m_TopLeftCorner	( grV2f( ) )
	, m_RotationInDeg	( rotationInDeg )
{
	SetCorners( m_Dimension, m_MidPos );

	if ( m_RotationInDeg != 0.0f )
	{
		CheckRotationBounds( &m_RotationInDeg );
		float rotInRad = m_RotationInDeg * grMath::DegToRad;

		RotatePoint( &m_Corners.TopLeft, rotInRad );
		RotatePoint( &m_Corners.TopRight, rotInRad );
		RotatePoint( &m_Corners.BotRight, rotInRad );
		RotatePoint( &m_Corners.BotLeft, rotInRad );

		// Would like to use this but circular hell
		//grMath::RotatePoint( &m_Corners.TopLeft, rotInRad );
		//grMath::RotatePoint( &m_Corners.TopRight, rotInRad );
		//grMath::RotatePoint( &m_Corners.BotRight, rotInRad );
		//grMath::RotatePoint( &m_Corners.BotLeft, rotInRad );
	}
}


// SetPos
//////////////////////////////////////////////////
inline void
grBBox::SetPos( const grV2f& rPos )
{
	m_MidPos = rPos;
	SetCorners( m_Dimension, m_MidPos );
}


// Intersect
//////////////////////////////////////////////////
const bool
grBBox::Intersects( const grBBox& rBBox )
{
	// TODO: Use corners instead of m_TopLeftCorner

	grBBox box			= rBBox;
	grV2f pos			= box.GetPos();
	grV2f dimension		= box.GetDimension();
	grV2f topLeftCorner	= box.GetTopLeftCorner();

	return	m_TopLeftCorner.x	< topLeftCorner.x	+ dimension.x	&&
			topLeftCorner.x		< m_TopLeftCorner.x	+ m_Dimension.x	&&
			m_TopLeftCorner.y	< topLeftCorner.y	+ dimension.y	&&
			topLeftCorner.y		< m_TopLeftCorner.y	+ m_Dimension.y;
}


// PointInside
//////////////////////////////////////////////////
inline const bool
grBBox::IsPointInside(const grV2f& rPoint)
{
	if( ( rPoint.x > ( m_MidPos.x - m_Dimension.x ) ) && ( rPoint.x < (m_MidPos.x + m_Dimension.x ) ) &&
		( rPoint.y > ( m_MidPos.y - m_Dimension.y ) ) && ( rPoint.y < (m_MidPos.y + m_Dimension.y ) ) )
	{
		return true;
	}
	
	return false;
}


// SetTopLeftCorner
//////////////////////////////////////////////////
inline void
grBBox::CheckRotationBounds( float* rotInDeg )
{
	if ( *rotInDeg > 359.9f )
	{
		*rotInDeg = *rotInDeg - 360.0f;
	}
	else if ( *rotInDeg < -359.9f )
	{
		*rotInDeg = *rotInDeg + 360.0f;
	}
}


// RotatePoint
//////////////////////////////////////////////////
inline void
grBBox::RotatePoint( grV2f* pPoint, const float rotInRad )
{
	float x = pPoint->x - m_MidPos.x;
	float y = pPoint->y - m_MidPos.y;
	float sin = std::sin( rotInRad );
	float cos = std::cos( rotInRad );
	float newX = x * cos - y * sin;
	float newY = x * sin + y * cos;
	pPoint->x = newX + m_MidPos.x;
	pPoint->y = newY + m_MidPos.y;
}


// SetCorners
//////////////////////////////////////////////////
inline void
grBBox::SetCorners( const grV2f& rDimension, const grV2f& rPos )
{
	m_Corners.TopLeft.x = rPos.x - ( rDimension.x * 0.5f );
	m_Corners.TopLeft.y = rPos.y - ( rDimension.y * 0.5f );

	m_Corners.TopRight.x = rPos.x + ( rDimension.x * 0.5f );
	m_Corners.TopRight.y = m_Corners.TopLeft.y;

	m_Corners.BotRight.x = m_Corners.TopRight.x;
	m_Corners.BotRight.y = rPos.y + ( rDimension.y * 0.5f );

	m_Corners.BotLeft.x = m_Corners.TopLeft.x;
	m_Corners.BotLeft.y = m_Corners.BotRight.y;






	//m_TopLeftCorner.x = rPos.x - ( rDimension.x * 0.5f );
	//m_TopLeftCorner.y = rPos.y - ( rDimension.y * 0.5f );

	//m_TopRightCorner.x = rPos.x + ( rDimension.x * 0.5f );
	//m_TopRightCorner.y = m_TopLeftCorner.y;

	//m_BotLeftCorner.x = m_TopLeftCorner.x;
	//m_BotLeftCorner.y = rPos.y + ( rDimension.y * 0.5f );

	//m_BotRightCorner.x = m_TopRightCorner.x;
	//m_BotRightCorner.y = m_BotLeftCorner.y;
}
