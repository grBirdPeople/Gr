#include	"grBBox.h"

#include	"grDefine.h"
			

// Intersect
//////////////////////////////////////////////////
bool
grBBox::Intersect( const grBBox& rBBox )
{
	grBBox otherBox			= rBBox;
	grV2f otherPos			= otherBox.GetPos();
	grV2f otherDimension	= otherBox.GetDimension();
	grV2f otherCorner		= otherBox.GetTopLeftCorner();

	return	m_TopLeftCorner.x	< otherCorner.x		+ otherDimension.x	&&
			otherCorner.x		< m_TopLeftCorner.x	+ m_Dimension.x		&&
			m_TopLeftCorner.y	< otherCorner.y		+ otherDimension.y	&&
			otherCorner.y		< m_TopLeftCorner.y	+ m_Dimension.y;
}


// PointInside
//////////////////////////////////////////////////
bool
grBBox::IsPointInside(const grV2f& rPoint)
{
	if( ( rPoint.x > ( m_MidPos.x - m_Dimension.x ) ) && ( rPoint.x < (m_MidPos.x + m_Dimension.x ) ) &&
		( rPoint.y > ( m_MidPos.y - m_Dimension.y ) ) && ( rPoint.y < (m_MidPos.y + m_Dimension.y ) ) )
	{
		return true;
	}
	
	return false;
}
