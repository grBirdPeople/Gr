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
	
	//////////////////////////////////////////////////

	inline	const	grV2f&		GetPosistion			( void )						{ return m_MidPos; }
	inline			void		SetPosition				( const grV2f& rPosition )
																						{
																							m_MidPos = rPosition;
																							UpdateCorners( );
																						}
	inline	const	grV2f&		GetDimensions			( void )						{ return m_Dimensions; }
	inline			void		SetDimensions			( const grV2f& rDimensions )
																						{
																							m_Dimensions = rDimensions;
																							UpdateCorners( );
																						}
	inline	const	Corners&	GetCorners				( void ) 						{ return m_Corners; }
	
	//////////////////////////////////////////////////
	
			const	bool		Intersects				( const grBBox& rBBox );
	inline	const	bool		IsPointInside			( const grV2f& rPoint );
	
	//////////////////////////////////////////////////
	
private:

	inline			void		UpdateCorners			( void );

	//////////////////////////////////////////////////
	
	Corners	m_Corners;

	grV2f	m_Dimensions,
			m_MidPos;			// Origo

};

#endif		//_GRBBOX_H_
