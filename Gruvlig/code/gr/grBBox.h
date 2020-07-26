#ifndef		_GRBBOX_H_
#define		_GRBBOX_H_

#include	"grV2.h"


// grBBox
//////////////////////////////////////////////////
class grBBox
{
public:
	struct Corners
	{
		grV2f TopLeft, TopRight, BotRight, BotLeft;
	};

	inline grBBox( const grV2f& rDimension, const grV2f& rPos )
		: m_Dimensions( rDimension )
		, m_MidPos( rPos )
	{
		UpdateCorners();
	}

	inline const grV2f& GetPosistion() const
	{
		return m_MidPos;
	}

	inline void SetPosition( const grV2f& rPosition )
	{
		m_MidPos = rPosition;
		UpdateCorners( );
	}

	inline const grV2f& GetDimensions() const
	{
		return m_Dimensions;
	}

	inline void SetDimensions( const grV2f& rDimensions )
	{
		m_Dimensions = rDimensions;
		UpdateCorners();
	}

	inline const Corners& GetCorners() const
	{
		return m_Corners;
	}

	const bool Intersects( const grBBox& rBBox )
	{
		grBBox otherBox{ rBBox };
		grV2f otherPos{ otherBox.GetPosistion( ) };
		grV2f otherDimensions{ otherBox.GetDimensions( ) };
		grV2f otherTopLeft{ otherBox.GetCorners( ).TopLeft };

		return	m_Corners.TopLeft.x	< otherTopLeft.x		+ otherDimensions.x &&
				otherTopLeft.x		< m_Corners.TopLeft.x	+ m_Dimensions.x &&
				m_Corners.TopLeft.y	< otherTopLeft.y		+ otherDimensions.y &&
				otherTopLeft.y		< m_Corners.TopLeft.y	+ m_Dimensions.y;
	}

	inline const bool IsPointInside( const grV2f& rPoint )
	{
		return	( rPoint.x > m_MidPos.x - m_Dimensions.x ) &&
				( rPoint.x < m_MidPos.x + m_Dimensions.x ) &&
				( rPoint.y > m_MidPos.y - m_Dimensions.y ) &&
				( rPoint.y < m_MidPos.y + m_Dimensions.y );
	}
	
private:
	inline void UpdateCorners()
	{
		float halfDimensionX{ m_Dimensions.x * 0.5f };
		float halfDimensionY{ m_Dimensions.y * 0.5f };

		m_Corners.TopLeft.x = m_MidPos.x - halfDimensionX;
		m_Corners.TopLeft.y = m_MidPos.y - halfDimensionY;

		m_Corners.TopRight.x = m_MidPos.x + halfDimensionX;
		m_Corners.TopRight.y = m_Corners.TopLeft.y;

		m_Corners.BotRight.x = m_Corners.TopRight.x;
		m_Corners.BotRight.y = m_MidPos.y + halfDimensionY;

		m_Corners.BotLeft.x = m_Corners.TopLeft.x;
		m_Corners.BotLeft.y = m_Corners.BotRight.y;
	}
	
	Corners	m_Corners;
	grV2f m_Dimensions, m_MidPos;	// m_MidPos = Origo
};

#endif		//_GRBBOX_H_