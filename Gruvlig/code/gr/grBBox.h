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
		grV2f	TopLeft,
				TopRight,
				BotRight,
				BotLeft;
	};

	//////////////////////////////////////////////////

public:
								grBBox					( const grV2f& rDimension, const grV2f& rPos );
								grBBox					( const grV2f& rDimension, const grV2f& rPos, const float rotationInDeg );
	
	//////////////////////////////////////////////////
	
	inline	const	grV2f&		GetDimension			( void )					{ return m_Dimension; }
	inline			void		SetDimension			( const grV2f& rDmension )	{ m_Dimension = rDmension; }
	inline	const	grV2f&		GetPos					( void )					{ return m_MidPos; }
	inline			void		SetPos					( const grV2f& rPos );
	inline	const	grV2f&		GetTopLeftCorner		( void )					{ return m_TopLeftCorner; }
	inline	const	Corners&	GetCorners				( void ) 					{ return m_Corners; }
	
	//////////////////////////////////////////////////
	
			const	bool		Intersects				( const grBBox& rBBox );
	inline	const	bool		IsPointInside			( const grV2f& rPoint );
	
	//////////////////////////////////////////////////
	
private:

	inline			void		CheckRotationBounds		( float* rotInDeg );
	inline			void		RotatePoint				( grV2f* pPoint, const float rotInRad );
	inline			void		SetCorners				( const grV2f& rDimension, const grV2f& rPos );

	//////////////////////////////////////////////////
	
	Corners	m_Corners;

	grV2f	m_Dimension,
			m_MidPos,			// Origo
			m_TopLeftCorner;

	float	m_RotationInDeg;

};

#endif		//_GRBBOX_H_
