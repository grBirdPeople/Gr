#ifndef		_GRBBOX_H_
#define		_GRBBOX_H_

#include	"grCommon.h"
#include	"grV2.h"


// grBBox
//////////////////////////////////////////////////
class grBBox
{
public:

	grBBox( const grV2f& rDimension, const grV2f& rPos ) : m_Dimension( rDimension ), m_Pos( rPos ), m_TopLeftCorner( grV2f() )
	{
		SetTopLeftCorner( m_Dimension, m_Pos );
	}
	
	//////////////////////////////////////////////////
	
	inline	grV2f&	GetDimension		( void )									{ return m_Dimension; }
	inline	void	SetDimension		( const grV2f& rDmension )					{ m_Dimension = rDmension; }
	inline	grV2f&	GetPos				( void )									{ return m_Pos; }
	inline	void	SetPos				( const grV2f& rPos )						{ m_Pos = rPos;	SetTopLeftCorner(m_Dimension, m_Pos); }
	inline	grV2f&	GetTopLeftCorner	( void )									{ return m_TopLeftCorner; }
	
	//////////////////////////////////////////////////
	
			bool	Intersect			( const grBBox& rBBox );
			bool	PointInside			( const grV2f& rPoint );
	
	//////////////////////////////////////////////////
	
private:

	inline	void	SetTopLeftCorner( const grV2f& rDimension, const grV2f& rPos )
	{
		m_TopLeftCorner.x = rPos.x - ( rDimension.x * 0.5f );
		m_TopLeftCorner.y = rPos.y - ( rDimension.y * 0.5f );
	}

	//////////////////////////////////////////////////
	
	grV2f	m_Dimension,
			m_Pos,				// Origo
			m_TopLeftCorner;

};

#endif		//_GRBBOX_H_
