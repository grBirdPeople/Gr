#ifndef		_H_GRMATH_
#define		_H_GRMATH_

#include	"grCommon.h"
#include	"grV2.h"

//class		grV2f;


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
	inline	sInt	Min			( const sInt a, const sInt b );
	inline	float	Min			( const float a, const float b );							// Untested

	// Max
	//////////////////////////////////////////////////
	inline	sInt	Max			( const sInt a, const sInt b );
	inline	float	Max			( const float a, const float b );							// Untested

	// Abs
	//////////////////////////////////////////////////
	inline	float	Abs			( const float value );
	inline	uInt	Abs			( const sInt value );

	// Sign
	//////////////////////////////////////////////////
	inline	sInt	Sign		( const sInt value );
	inline	float	Sign		( const float value );

	// Sqrt // Babylonian method stolen from overflow but changed from doubles to floats
	//////////////////////////////////////////////////
	inline	float	Sqrt		( const float value );

	// Lerp
	//////////////////////////////////////////////////
	inline	float	Lerp		( const float from, const float to, const float value );
	inline	grV2f	Lerp		( const grV2f& rVec1, const grV2f& rVec2, const float value );

	// MoveTo
	//////////////////////////////////////////////////
	inline	float	MoveTo		( const float from, const float to, const float speed );	// Untested

	// MoveToZero
	//////////////////////////////////////////////////
	inline	float	MoveToZero	( const float from, const float speed );					// Untested

	// Clamp
	//////////////////////////////////////////////////
	inline	sInt	Clamp		( const sInt value, const sInt min, const sInt max );		// Untested
	inline	float	Clamp		( const float value, const float min, const float max );	// Untested

	// RotatePoint
	//////////////////////////////////////////////////
	inline	void	RotatePoint	( grV2f* pPoint, const float rotationInDeg );
}

#endif	// _H_GRMATH_
