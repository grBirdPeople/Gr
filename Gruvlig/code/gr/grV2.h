#ifndef		_GRV2_
#define		_GRV2_

//#include    <math.h>
//#include	<ostream>

#include	"grDefine.h"
#include	"grCommon.h"


// grV2 // Not safe for move copy and move assign
//////////////////////////////////////////////////
template <typename T>
struct grV2
{
	grV2( const T& X = 0.0f, const T& Y = 0.0f )
		: x( X ) ,y( Y )
	{}
	grV2( const grV2<T>& rV2 )
		: x( rV2.x ), y( rV2.y )
	{}

	//////////////////////////////////////////////////

	inline void Set ( const T X, const T Y ) { x = X; y = Y; }
	inline void Set ( const grV2<T>& rV2 ) { x = rV2.x; y = rV2.y; }
	inline T Magnitude ( void ) { return std::sqrt( MagnitudeSqr() ); }
	inline float MagnitudeSqr ( void ) { return ( x * x ) + ( y * y ); }
	inline void Normalize ( void ) { *this = *this / Magnitude(); }
	inline grV2<T> Normalized ( void ) { return *this / Magnitude(); }
	inline float Dot ( const grV2<T>& rV2 ) { return ( x * rV2.x ) + ( y * rV2.y ); }
	inline grV2<T> Cross ( void ) { return { y, -x }; }
	inline grV2<T> Between ( const grV2<T>& rV2 ) { return { rV2.x - x, rV2.y - y }; }
	inline grV2<T> LimitMin( const float min ) { return ( this->Magnitude() < min ) ? this->Normalized() * min : *this; }
	inline grV2<T> LimitMax( const float max ) { return ( this->Magnitude() > max ) ? this->Normalized() * max : *this; }

	//////////////////////////////////////////////////

	inline grV2<T> operator+ ( const T& num ) { return { x + num, y + num }; }
	inline grV2<T> operator+ ( const grV2<T>& rV2 ) { return { x + rV2.x, y + rV2.y }; }
	inline grV2<T> operator- ( const T& num ) { return { x - num, y - num }; }
	inline grV2<T> operator- ( const grV2<T>& rV2 ) { return { x - rV2.x, y - rV2.y }; }
	inline grV2<T> operator* ( const T& num ) { return { x * num, y * num }; }
	inline grV2<T> operator* ( const grV2<T>& rV2 ) { return { x * rV2.x, y * rV2.y }; }
	inline grV2<T> operator/ ( const T& num ) { return { x / num, y / num }; }
	inline grV2<T> operator/ ( const grV2<T>& rV2 ) { return { x / rV2.x, y / rV2.y }; }
			  
	//////////////////////////////////////////////////

	inline grV2<T>& operator= ( const T& num ) { x = num; y = num; return *this; }
	inline grV2<T>& operator= ( const grV2<T>& rV2 )
	{
		if ( this == &rV2 ) { return *this; }
		x = rV2.x; y = rV2.y; { return *this; }
	}
	inline grV2<T>& operator+= ( const T& num ) { x += num; y += num; return *this; }
	inline grV2<T>& operator+= ( const grV2<T>& rV2 ) { x += rV2.x; y += rV2.y; return *this; }
	inline grV2<T>& operator-= ( const T& num ) { x -= num; y -= num; return *this; }
	inline grV2<T>& operator-= ( const grV2<T>& rV2 ) { x -= rV2.x; y -= rV2.y; return *this; }
	inline grV2<T>& operator*= ( const T& num ) { x *= num; y *= num; return *this; }
	inline grV2<T>& operator*= ( const grV2<T>& rV2 ) { x *= rV2.x; y *= rV2.y; return *this; }
	inline grV2<T>& operator/= ( const T& num ) { x /= num; y /= num; return *this; }
	inline grV2<T>& operator/= ( const grV2<T>& rV2 ) { x /= rV2.x; y /= rV2.y; return *this; }

	//////////////////////////////////////////////////

	inline bool operator== ( const grV2<T>& rV2 ) { return ( x == rV2.x ) && ( y == rV2.y ) ? true : false; }
	inline bool operator!= ( const grV2<T>& rV2 ) { return ( x != rV2.x ) || ( y != rV2.y ) ? true : false; }

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
typedef	grV2<intU>		grV2u;

#endif		//_GRV2_