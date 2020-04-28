#ifndef		_GRV2_
#define		_GRV2_

//#include    <math.h>
//#include	<ostream>

#include	"grCommon.h"


// grV2
//////////////////////////////////////////////////
template <typename T>
struct grV2
{
						grV2			( const T& X = 0.0f, const T& Y = 0.0f )
											: x( X )
											, y( Y )
										{}
				inline	grV2			( const grV2<T>& rV2 )				{ *this = rV2; }

	//////////////////////////////////////////////////

	inline	void		Set				( const float X, const float Y )	{ x = X; y = Y; }
	inline	void		Set				( const grV2<T>& rV2 )				{ x = rV2.x; y = rV2.y; }
	inline	T			Magnitude		( void )							{ return  std::sqrt( MagnitudeSqr() ); }
	inline	float		MagnitudeSqr	( void )							{ return ( x * x ) + ( y * y ); }
	inline	void		Normalize		( void )							{ *this = *this / this->Magnitude(); }
	inline	grV2<T>		Normalized		( void )							{ return *this / this->Magnitude(); }
	inline	float		Dot				( const grV2<T>& rV2 )				{ return ( x * rV2.x ) + ( y * rV2.y ); }
	inline  grV2<T>		Cross			( void )							{ return grV2f( y, -x ); }

	//////////////////////////////////////////////////

	inline	grV2<T>		operator+	( const T& num )						{ return grV2<T>( x + num, y + num ); }
	inline	grV2<T>		operator+	( const grV2<T>& rV2 )					{ return grV2<T>( x + rV2.x, y + rV2.y ); }
	inline	grV2<T>		operator-	( const T& num )						{ return grV2<T>( x - num, y - num ); }
	inline	grV2<T>		operator-	( const grV2<T>& rV2 )					{ return grV2<T>( x - rV2.x, y - rV2.y ); }
	inline	grV2<T>		operator*	( const T& num )						{ return grV2<T>( x * num, y * num ); }
	inline	grV2<T>		operator*	( const grV2<T>& rV2 )					{ return grV2<T>( x * rV2.x, y * rV2.y ); }
	inline	grV2<T>		operator/	( const T& num )						{ return grV2<T>( x / num, y / num ); }
	inline	grV2<T>		operator/	( const grV2<T>& rV2 )					{ return grV2<T>( x / rV2.x, y / rV2.y ); }
			  
	//////////////////////////////////////////////////

	inline	grV2<T>&	operator=	( const T& num )						{ x = num; y = num; return *this; }
	inline	grV2<T>&	operator=	( const grV2<T>& rV2 )
			{
				if ( this == &rV2 )		{ return *this; }
				x = rV2.x; y = rV2.y;	{ return *this; }
			}
	inline	grV2<T>&	operator+=	( const T& num )						{ x += num; y += num; return *this; }
	inline	grV2<T>&	operator+=	( const grV2<T>& rV2 )					{ x += rV2.x; y += rV2.y; return *this; }
	inline	grV2<T>&	operator-=	( const T& num )						{ x -= num; y -= num; return *this; }
	inline	grV2<T>&	operator-=	( const grV2<T>& rV2 )					{ x -= rV2.x; y -= rV2.y; return *this; }
	inline	grV2<T>&	operator*=	( const T& num )						{ x *= num; y *= num; return *this; }
	inline	grV2<T>&	operator*=	( const grV2<T>& rV2 )					{ x *= rV2.x; y *= rV2.y; return *this; }
	inline	grV2<T>&	operator/=	( const T& num )						{ x /= num; y /= num; return *this; }
	inline	grV2<T>&	operator/=	( const grV2<T>& rV2 )					{ x /= rV2.x; y /= rV2.y; return *this; }

	//////////////////////////////////////////////////

	inline	bool		operator==	( const grV2<T>& rV2 )					{ return ( x == rV2.x ) && ( y == rV2.y ) ? true : false; }
	inline	bool		operator!=	( const grV2<T>& rV2 )					{ return ( x != rV2.x ) || ( y != rV2.y ) ? true : false; }

	//////////////////////////////////////////////////

	inline T& operator[]( const intU idx )
	{
		if ( idx < 0 || idx > 1 )
		{
#ifdef DEBUG
			std::cerr << "grV2::operator[]: Index was out of range" << std::endl;
#endif // DEBUG
			T t;
			return t;
		}

		return ( idx == 0 ) ? x : y;
	}

	//////////////////////////////////////////////////

	template <typename T>
	friend std::ostream& operator<<( std::ostream& out, const grV2<T>& rV2 );

	//////////////////////////////////////////////////

	T x, y;

};

// std::cout
//////////////////////////////////////////////////
template <typename T>
std::ostream& operator<< ( std::ostream& out, const grV2<T>& rV2 )
{
	return out << "(" << rV2.x << ", " << rV2.y << ")";
}

// Typedefs
//////////////////////////////////////////////////
typedef	grV2<double>	grV2d;
typedef	grV2<float>		grV2f;
typedef	grV2<intS>		grV2i;

#endif		//_GRV2_