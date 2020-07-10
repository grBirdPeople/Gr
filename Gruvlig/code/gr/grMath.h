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
	const float EpsilonColor = ( float )1e-10;
	const float Epsilon = 0.001f;
	const float Pi = 3.141592f;
	const float DegToRad = Pi / 180.0f;
	const float RadToDeg = 180.0f / Pi;

	// Min
	//////////////////////////////////////////////////
	inline intU	Min( const intU a, const intU b )
	{
		intU value = ( a < b ) ? a : ( b < a ) ? b : a;
		return value;
	}
	inline intS	Min( const intS a, const intS b )
	{
		intS value = ( a < b ) ? a : ( b < a ) ? b : a;
		return value;
	}
	inline float Min( const float a, const float b )
	{
		float value = ( a < b ) ? a : ( b < a ) ? b : a;
		return value;
	}

	// Max
	//////////////////////////////////////////////////
	inline intU Max( const intU a, const intU b )
	{
		intU value = ( a > b ) ? a : ( b > a ) ? b : a;
		return value;
	}
	inline intS	Max( const intS a, const intS b )
	{
		intS value = ( a > b ) ? a : ( b > a ) ? b : a;
		return value;
	}
	inline float Max( const float a, const float b )
	{
		float value = ( a > b ) ? a : ( b > a ) ? b : a;
		return value;
	}

	// RangeCheckV2f
	//////////////////////////////////////////////////
	inline void RangeCheckIntU( const intU a, intU& b )
	{
		if ( a > b )
			b = a;
	}
	inline void RangeCheckV2f( grV2f& rV )
	{
		if ( rV.x > rV.y )
			rV.y = rV.x;
	}
	inline void RangeCheckV2fx2( const grV2f& rMin, grV2f& rMax )
	{
		if ( rMin.x > rMax.x )
			rMax.x = rMin.x;
		if ( rMin.y > rMax.y )
			rMax.y = rMin.y;
	}

	// Abs
	//////////////////////////////////////////////////
	inline float Abs( float value )
	{
		return ( value < 0.0f ) ? -value : value;
	}
	inline intS	Abs( intS value )
	{
		return ( value < 0 ) ? -value : value;
	}
	inline grV2f Abs( grV2f& value )
	{
		return grV2f( Abs( value.x ), Abs( value.y ) );
	}

	// Sign
	//////////////////////////////////////////////////
	inline intS	Sign( intS value )
	{
		return ( value < 0 ) ? -1 : ( value > 0 ) ? 1 : 0;
	}
	inline float Sign( float value )
	{
		return ( value < 0.0f ) ? -1.0f : ( value > 0.0f ) ? 1.0f : 0.0f;
	}

	// Sqrt // Babylonian method copied from overflow. Changed from doubles to floats
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

		return ( float )ldexp( y, exp / 2 );		// multiply answer by 2^(exp/2)
	}

	// Lerp
	//////////////////////////////////////////////////
	inline float Lerp( float start, float end, float value )
	{
		return ( value <= 0.0f ) ? start : ( value >= 1.0f ) ? end : ( end - start ) * value + start;
	}
	inline grV2f Lerp( grV2f& rStart, grV2f& rEnd, float value )
	{
		if ( value <= 0.0f )
		{
			return rStart;
		}
		else if ( value >= 1.0f )
		{
			return rEnd;
		}

		grV2f v;
		v.x = rStart.x + ( rEnd.x - rStart.x ) * value;
		v.y = rStart.y + ( rEnd.y - rStart.y ) * value;
		return v;
	}

	// MoveTo
	//////////////////////////////////////////////////
	inline float MoveTo( float from, float to, float speed )	// Untested
	{
		float cpyFrom = from;
		float sign = Sign( to - cpyFrom );
		cpyFrom += sign * speed;
		return ( Sign( to - cpyFrom ) != sign ) ? to : cpyFrom;
	}

	// MoveToZero
	//////////////////////////////////////////////////
	inline float MoveToZero( float from, float speed )			// Untested
	{
		float cpyFrom = from;
		float sign = Sign( from );
		cpyFrom -= sign * speed;
		return ( Sign( from ) != sign ) ? 0.0f : cpyFrom;
	}

	// Clamp
	//////////////////////////////////////////////////
	inline float Clamp( float value, float min, float max )
	{
		return ( value < min ) ? min : ( value > max ) ? max : value;
	}
	inline intS Clamp( intS value, intS min, intS max )
	{
		return ( value < min ) ? min : ( value > max ) ? max : value;
	}
	inline intS Clamp( intU value, intU min, intU max )
	{
		return ( value < min ) ? min : ( value > max ) ? max : value;
	}

	// RotatePoint
	//////////////////////////////////////////////////
	inline void RotatePoint( grV2f* pPoint, const float rotInRad ) // TODO: Change from ptr to ref when old particle code is gone
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
	inline float VecToRad( const grV2f& rV )
	{
		grV2f v = rV;
		return std::atan2f( v.y, v.x );
	}

	// VecToDeg
	//////////////////////////////////////////////////
	inline float VecToDeg( const grV2f& rV )
	{
		grV2f v = rV;
		return std::atan2f( v.y, v.x ) * RadToDeg;
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
	inline bool CmpIntU( const intU numOne, const intU numTwo )
	{
		return ( numOne == numTwo );
	}
	inline bool CmpFloat( const float numOne, const float numTwo )
	{
		return Abs( numOne - numTwo ) < Epsilon;
	}
	inline bool CmpV2f( const grV2f& rA, const grV2f& rB )
	{
		float x = Abs( rA.x - rB.x );
		float y = Abs( rA.y - rB.y );
		return ( x < Epsilon ) && ( y < Epsilon );
	}

	// Step
	//////////////////////////////////////////////////
	inline intU Step( const intU A, const intU B )
	{
		return ( A >= B ) ? 0 : 1;
	}
	inline intU Step( const float A, const float B )
	{
		return ( A >= B ) ? 0 : 1;
	}
}

#endif	// _H_GRMATH_
