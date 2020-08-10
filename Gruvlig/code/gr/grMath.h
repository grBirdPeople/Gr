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
	const float EpsilonSml = ( float )1e-10;
	const float EpsilonLrg = 0.001f;
	const float Pi = 3.141592f;
	const float Deg2RadConv = Pi / 180.0f;
	const float Rad2DegConv = 180.0f / Pi;

	// Min
	//////////////////////////////////////////////////
	template<typename T>
	inline T Min( const T a, const T b )
	{
		return ( a < b ) ? a : ( b < a ) ? b : a;
	}

	// Max
	//////////////////////////////////////////////////
	template<typename T>
	inline T Max( const T a, const T b )
	{
		return ( a > b ) ? a : ( b > a ) ? b : a;
	}

	// RangeChecks
	//////////////////////////////////////////////////
	template<typename T>
	inline void RangeCheck( const T a, T& b )
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
	inline float AbsF( float value )
	{
		return ( value < 0.0f ) ? -value : value;
	}

	inline grV2f AbsV2f( grV2f& value )
	{
		return grV2f( AbsF( value.x ), AbsF( value.y ) );
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
			std::cerr << "grMath::Sqrt(): Can't sqrt zero or negative\n" << std::endl;
			return 0.0f;							// if negative number throw an exception?
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

		return ( float )ldexp( y, exp / 2 );		// multiply result by 2^(exp/2)
	}

	// Lerp
	//////////////////////////////////////////////////
	inline float Lerp( float start, float end, float value )
	{
		return ( value <= 0.0f ) ? start : ( value >= 1.0f ) ? end : ( end - start ) * value + start;
	}

	inline grV2f LerpV2f( grV2f& rStart, grV2f& rEnd, float value )
	{
		return ( value <= 0.0f ) ? rStart : ( value >= 1.0f ) ? rEnd :
			grV2f{ ( rStart.x + ( rEnd.x - rStart.x ) * value ),
			( rStart.y + ( rEnd.y - rStart.y ) * value ) };
	}

	// Compare float
	//////////////////////////////////////////////////
	inline bool CmpV2f( const grV2f& rA, const grV2f& rB )
	{
		float x{ AbsF( rA.x - rB.x ) };
		float y{ AbsF( rA.y - rB.y ) };
		return ( x < EpsilonLrg ) && ( y < EpsilonLrg );
	}

	inline bool CmpFloat( const float numOne, const float numTwo )
	{
		return AbsF( numOne - numTwo ) < EpsilonLrg;
	}

	// MoveTo
	//////////////////////////////////////////////////
	inline float MoveTo( float from, float to, float speed )	// Untested
	{
		float cpyFrom{ from };
		float sign{ Sign( to - cpyFrom ) };
		cpyFrom += sign * speed;
		return ( CmpFloat( Sign( to - cpyFrom ), sign ) == false ) ? to : cpyFrom;
	}

	// MoveToZero
	//////////////////////////////////////////////////
	inline float MoveToZero( float from, float speed )			// Untested
	{
		float cpyFrom{ from };
		float sign{ Sign( from ) };
		cpyFrom -= sign * speed;
		return ( CmpFloat( Sign( from ), sign ) == false ) ? 0.0f : cpyFrom;
	}

	// Clamp
	//////////////////////////////////////////////////
	template<typename T>
	inline T Clamp( const T value, const T min, const T max )
	{
		return ( value < min ) ? min : ( value > max ) ? max : value;
	}

	// RotatePoint
	//////////////////////////////////////////////////
	inline void RotatePoint( grV2f* pPoint, const float rotInRad ) // TODO: Change from ptr to ref when old particle code is gone
	{
		float sin{ std::sin( rotInRad ) };
		float cos{ std::cos( rotInRad ) };
		float x{ pPoint->x };
		float y{ pPoint->y };
		pPoint->x = x * cos - y * sin;
		pPoint->y = x * sin + y * cos;
	}

	// DegToRad
	//////////////////////////////////////////////////
	inline float DegToRad( const float deg )
	{
		return deg * Deg2RadConv;
	}

	// RadToDeg
	//////////////////////////////////////////////////
	inline float RadToDeg( const float rad )
	{
		return rad * Rad2DegConv;
	}

	// VecToRad
	//////////////////////////////////////////////////
	inline float VecToRad( const grV2f& rV )
	{
		return std::atan2f( rV.y, rV.x );
	}

	// VecToDeg
	//////////////////////////////////////////////////
	inline float VecToDeg( const grV2f& rV )
	{
		return VecToRad( rV ) * Rad2DegConv;
	}

	// RadToVec
	//////////////////////////////////////////////////
	inline grV2f RadToVec( float rad )
	{
		return grV2f( std::sin( rad ), -std::cos( rad ) );
	}

	// DegToVec
	//////////////////////////////////////////////////
	inline grV2f DegToVec( float deg )
	{
		return RadToVec( DegToRad( deg ) );
	}

	// Step
	//////////////////////////////////////////////////
	inline intU Step( const intU A, const intU B )
	{
		return ( A >= B ) ? 0 : 1;
	}

	inline float Step( const float A, const float B )
	{
		return ( A >= B ) ? 0.0f : 1.0f;
	}
}

#endif	// _H_GRMATH_
