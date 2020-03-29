#ifndef		_H_GRMATH_
#define		_H_GRMATH_

#include	<math.h>
#include	<limits>

#include	"grCommon.h"
#include	"grV2.h"


// grMath
//////////////////////////////////////////////////
namespace grMath
{
	// Const
	//////////////////////////////////////////////////
	const float Pi = 3.141592f;
	const float DegToRad = Pi / 180.0f;
	const float RadToDeg = 180 / Pi;
	const float Epsilon = 0.00001f;

	// Min
	//////////////////////////////////////////////////
	inline sInt	Min( sInt a, sInt b )
	{
		return ( a < b ) ? a : ( b < a ) ? b : a;
	}
	inline float Min( float a, float b )							// Untested
	{
		return ( a < b ) ? a : ( b < a ) ? b : a;
	}

	// Max
	//////////////////////////////////////////////////
	inline sInt	Max( sInt a, sInt b )
	{
		return ( a > b ) ? a : ( b > a ) ? b : a;
	}
	inline float Max( float a, float b )							// Untested
	{
		return ( a > b ) ? a : ( b > a ) ? b : a;
	}

	// Abs
	//////////////////////////////////////////////////
	inline uInt	Abs( sInt value )
	{
		return ( value < 0 ) ? ( uInt )-value : ( uInt )value;
	}
	inline float Abs( float value )
	{
		return ( value < 0.0f ) ? -value : value;
	}

	// Sign
	//////////////////////////////////////////////////
	inline sInt	Sign( sInt value )
	{
		return ( value < 0 ) ? -1 : ( value > 0 ) ? 1 : 0;
	}
	inline float Sign( float value )
	{
		return ( value < 0.0f ) ? -1.0f : ( value > 0.0f ) ? 1.0f : 0.0f;
	}

	// Sqrt // Babylonian method stolen from overflow but changed from doubles to floats
	//////////////////////////////////////////////////
	inline float Sqrt( const float value )
	{
		float x = value;
		if ( x <= 0.0f )
		{
			std::cerr << "grMath::Sqrt(): Can't sqrt negatives\n" << std::endl;
			return 0;								// if negative number throw an exception?
		}

		int exp = 0;
		x = ( float )frexp( ( double )x, &exp );	// extract binary exponent from x
		if ( exp & 1 )								// we want exponent to be even
		{
			--exp;
			x *= 2.0f;
		}

		float y = ( 1.0f + x ) / 2.0f;				// first approximation
		float z = 0.0f;
		while ( y != z )							// yes, we CAN compare doubles here!
		{
			z = y;
			y = ( y + x / y ) / 2.0f;
		}

		return ( float )ldexp( y, exp / 2 );			// multiply answer by 2^(exp/2)
	}

	// Lerp
	//////////////////////////////////////////////////
	inline float Lerp( float from, float to, float value )
	{
		return ( value <= 0.0f ) ? from : ( value >= 1.0f ) ? to : ( to - from ) * value + from;
	}
	inline grV2f Lerp( grV2f& rVec1, grV2f& rVec2, float value )
	{
		if ( value <= 0.0f )
		{
			return rVec1;
		}
		else if ( value >= 1.0f )
		{
			return rVec2;
		}

		grV2f vec;
		vec.x = rVec1.x + ( rVec2.x - rVec1.x ) * value;
		vec.y = rVec1.y + ( rVec2.y - rVec1.y ) * value;
		return vec;
	}

	// MoveTo
	//////////////////////////////////////////////////
	inline float MoveTo( float from, float to, float speed )		// Untested
	{
		float cpyFrom = from;
		float sign = Sign( to - cpyFrom );
		cpyFrom += sign * speed;
		return ( Sign( to - cpyFrom ) != sign ) ? to : cpyFrom;
	}

	// MoveToZero
	//////////////////////////////////////////////////
	inline float MoveToZero( float from, float speed )						// Untested
	{
		float cpyFrom = from;
		float sign = Sign( from );
		cpyFrom -= sign * speed;
		return ( Sign( from ) != sign ) ? 0.0f : cpyFrom;
	}

	// Clamp
	//////////////////////////////////////////////////
	inline sInt Clamp( sInt value, sInt min, sInt max )
	{
		return ( value < min ) ? min : ( value > max ) ? max : value;
	}
	inline float Clamp( float value, float min, float max )
	{
		return ( value < min ) ? min : ( value > max ) ? max : value;
	}

	// RotatePoint
	//////////////////////////////////////////////////
	inline void RotatePoint( grV2f* pPoint, const float rotInRad )
	{
		float sin = std::sin( rotInRad );
		float cos = std::cos( rotInRad );
		float x = pPoint->x;
		float y = pPoint->y;
		pPoint->x = x * cos - y * sin;
		pPoint->y = x * sin + y * cos;
	}

	// VecToRad
	//////////////////////////////////////////////////
	inline float VecToRad( grV2f& rVec )
	{
		return std::atan2f( rVec.y, rVec.x );
	}

	// VecToDeg
	//////////////////////////////////////////////////
	inline float VecToDeg( grV2f& rVec )
	{
		return std::atan2f( rVec.y, rVec.x ) * RadToDeg;
	}

	// RadToVec
	//////////////////////////////////////////////////
	inline grV2f RadToVec( float rad, const bool counterClock = false )
	{
		grV2f vec;
		if ( counterClock == true )
		{
			vec.x = std::sin( rad );
			vec.y = -std::cos( rad );
		}
		else
		{
			vec.x = std::cos( rad );
			vec.y = std::sin( rad );
		}
		return vec;
	}

	// DegToVec
	//////////////////////////////////////////////////
	inline grV2f DegToVec( float deg, const bool counterClock = false )
	{
		grV2f vec;
		float rad = deg * DegToRad;
		if ( counterClock == true )
		{
			vec.x = std::sin( rad );
			vec.y = -std::cos( rad );
		}
		else
		{
			vec.x = std::cos( rad );
			vec.y = std::sin( rad );
		}
		return vec;
	}

	// ZeroCmp
	//////////////////////////////////////////////////
	inline bool CmpFloat( const float numOne, const float numTwo )
	{
		return Abs( numOne - numTwo ) < Epsilon;
	}
}

#endif	// _H_GRMATH_
