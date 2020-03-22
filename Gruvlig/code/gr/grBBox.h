#ifndef		_GRBBOX_H_
#define		_GRBBOX_H_

#include	"grCommon.h"
#include	"grV2.h"


// grBBox
//////////////////////////////////////////////////
class grBBox
{
public:

							grBBox				( const grV2f& rDimension, const grV2f& rPos );
	
	//////////////////////////////////////////////////
	
	inline	const	grV2f&	GetDimension		( void )					{ return m_Dimension; }
	inline			void	SetDimension		( const grV2f& rDmension )	{ m_Dimension = rDmension; }
	inline	const	grV2f&	GetPos				( void )					{ return m_MidPos; }
	inline			void	SetPos				( const grV2f& rPos );
	inline	const	grV2f&	GetTopLeftCorner	( void )					{ return m_TopLeftCorner; }
	
	//////////////////////////////////////////////////
	
			const	bool	Intersect			( const grBBox& rBBox );
	inline	const	bool	IsPointInside		( const grV2f& rPoint );
	
	//////////////////////////////////////////////////
	
private:

	inline			void	SetTopLeftCorner	( const grV2f& rDimension, const grV2f& rPos );

	//////////////////////////////////////////////////
	
	grV2f	m_Dimension,
			m_MidPos,			// Origo
			m_TopLeftCorner;

};

#endif		//_GRBBOX_H_
