#ifndef		_H_GRMATH_
#define		_H_GRMATH_

#include	<math.h>

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

	// Min
	//////////////////////////////////////////////////
	inline	sInt	Min			( const sInt a, const sInt b )
	{
		return ( a < b ) ? a : ( b < a ) ? b : a;
	}
	inline	float	Min			( const float a, const float b )							// Untested
	{
		return ( a < b ) ? a : ( b < a ) ? b : a;
	}

	// Max
	//////////////////////////////////////////////////
	inline	sInt	Max			( const sInt a, const sInt b )
	{
		return ( a > b ) ? a : ( b > a ) ? b : a;
	}
	inline	float	Max			( const float a, const float b )							// Untested
	{
		return ( a > b ) ? a : ( b > a ) ? b : a;
	}

	// Abs
	//////////////////////////////////////////////////
	inline	float	Abs			( const float value )
	{
		return ( value < 0.0f ) ? -value : value;
	}
	inline	uInt	Abs			( const sInt value )
	{
		return ( value < 0 ) ? ( uInt )-value : ( uInt )value;
	}

	// Sign
	//////////////////////////////////////////////////
	inline	sInt	Sign		( const sInt value )
	{
		return ( value < 0 ) ? -1 : ( value > 0 ) ? 1 : 0;
	}
	inline	float	Sign		( const float value )
	{
		return ( value < 0.0f ) ? -1.0f : ( value > 0.0f ) ? 1.0f : 0.0f;
	}

	// Sqrt // Babylonian method stolen from overflow but changed from doubles to floats
	//////////////////////////////////////////////////
	inline	float	Sqrt		( const float value )
	{
		float x = value;
		if ( x <= 0 )
		{
			std::cerr << "grMath::Sqrt(): Can't sqrt negatives\n" << std::endl;
			return 0;							// if negative number throw an exception?
		}

		int exp = 0;
		x = ( float )frexp( ( double )x, &exp );	// extract binary exponent from x
		if ( exp & 1 )							// we want exponent to be even
		{
			--exp;
			x *= 2.0f;
		}

		float y = ( 1.0f + x ) / 2.0f;			// first approximation
		float z = 0.0f;
		while ( y != z )						// yes, we CAN compare doubles here!
		{
			z = y;
			y = ( y + x / y ) / 2.0f;
		}

		return ( float )ldexp( y, exp / 2 );		// multiply answer by 2^(exp/2)
	}

	// Lerp
	//////////////////////////////////////////////////
	inline	float	Lerp		( const float from, const float to, const float value )
	{
		return ( value <= 0.0f ) ? from : ( value >= 1.0f ) ? to : ( to - from ) * value + from;
	}
	inline	grV2f	Lerp		( const grV2f& rVec1, const grV2f& rVec2, const float value )
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
	inline	float	MoveTo		( const float from, const float to, const float speed )		// Untested
	{
		float cpyFrom = from;
		float sign = Sign( to - cpyFrom );
		cpyFrom += sign * speed;
		return ( Sign( to - cpyFrom ) != sign ) ? to : cpyFrom;
	}

	// MoveToZero
	//////////////////////////////////////////////////
	inline	float	MoveToZero	( const float from, const float speed )						// Untested
	{
		float cpyFrom = from;
		float sign = Sign( from );
		cpyFrom -= sign * speed;
		return ( Sign( from ) != sign ) ? 0.0f : cpyFrom;
	}

	// Clamp
	//////////////////////////////////////////////////
	inline	sInt	Clamp		( const sInt value, const sInt min, const sInt max )		// Untested
	{
		sInt cpyValue = ( value < min ) ? min ? ( value > max ) : max : value;
		return cpyValue;
	}
	inline	float	Clamp		( const float value, const float min, const float max )		// Untested
	{
		float cpyValue = ( value < min ) ? min ? ( value > max ) : max : value;
		return cpyValue;
	}

	// RotatePoint
	//////////////////////////////////////////////////
	inline	void	RotatePoint	( grV2f* pPoint, const float rotInRad )
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
	inline	float	VecToRad	( const grV2f& rVec )
	{
		return std::atan2f( rVec.y, rVec.x );
	}

	// VecToDeg
	//////////////////////////////////////////////////
	inline	float	VecToDeg	( const grV2f& rVec )
	{
		return std::atan2f( rVec.y, rVec.x ) * RadToDeg;
	}

	// RadToVec
	//////////////////////////////////////////////////
	inline	grV2f	RadToVec	( const float rad, const bool counterClock = false )
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
	inline	grV2f	DegToVec	( const float deg, const bool counterClock = false )
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
}

#endif	// _H_GRMATH_
