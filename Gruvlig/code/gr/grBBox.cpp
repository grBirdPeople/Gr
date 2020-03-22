#include	"grBBox.h"

#include	"grDefine.h"
			

// cTor
//////////////////////////////////////////////////
grBBox::grBBox( const grV2f& rDimension, const grV2f& rPos )
	: m_Dimension		( rDimension )
	, m_MidPos			( rPos )
	, m_TopLeftCorner	( grV2f() )
{
	SetTopLeftCorner( m_Dimension, m_MidPos );
}


// SetPos
//////////////////////////////////////////////////
inline void
grBBox::SetPos( const grV2f& rPos )
{
	m_MidPos = rPos;
	SetTopLeftCorner( m_Dimension, m_MidPos );
}


// Intersect
//////////////////////////////////////////////////
const bool
grBBox::Intersect( const grBBox& rBBox )
{
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
grBBox::SetTopLeftCorner( const grV2f& rDimension, const grV2f& rPos )
{
	m_TopLeftCorner.x = rPos.x - ( rDimension.x * 0.5f );
	m_TopLeftCorner.y = rPos.y - ( rDimension.y * 0.5f );
}
