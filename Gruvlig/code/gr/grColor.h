#ifndef		_H_GRCOLOR_
#define		_H_GRCOLOR_

#include	"grMath.h"


namespace grColor
{
	// SRgba
	//////////////////////////////////////////////////
	struct SRgba
	{
		SRgba( const uint16_t r = 254, const uint16_t g = 254, const uint16_t b = 254, const uint16_t a = 254 )
			: R( ( float )r ) , G( ( float )g ) , B( ( float )b ) , A( ( float )a )
		{}

		float R, G, B, A;
	};

	struct Rgba // Unsigned int 16 as that is what SFML is using
	{
		Rgba( const uint16_t r = 254, const uint16_t g = 254, const uint16_t b = 254, const uint16_t a = 254 )
			: R( r ), G( g ), B( b ), A( a )
		{}

		bool Cmp( const Rgba& rOther )
		{
			return R == rOther.R && G == rOther.G && B == rOther.B && A == rOther.A ? true : false;
		}

		uint16_t R, G, B, A;
	};

	// SHsva
	//////////////////////////////////////////////////
	struct SHsva
	{
		SHsva( const float h = 0.0f, const float s = 0.0f, const float v = 0.0f, const float a = 0.0f )
			: H( h ) , S( s ) , V( v ) , A( a )
		{}

		float H, S, V, A;
	};

	// Rgba2Hsva // Taken from stackoverflow
	//////////////////////////////////////////////////
	inline SHsva Rgba2Hsva( SRgba& rRgba )
	{
		SHsva out( 0.0f, 0.0f, 0.0f, rRgba.A );

		float min = rRgba.R < rRgba.G ? rRgba.R : rRgba.G;
		min = min < rRgba.B ? min : rRgba.B;

		float max = rRgba.R > rRgba.G ? rRgba.R : rRgba.G;
		max = max > rRgba.B ? max : rRgba.B;

		out.V = max;                                // v
		float delta = max - min;
		if ( delta < 0.00001f )
		{
			out.S = 0.0f;
			out.H = 0.0f; // undefined, maybe nan?
			return out;
		}
		if ( max > 0.0f )
		{ // NOTE: if Max is == 0, this divide would cause a crash
			out.S = ( delta / max );                  // s
		}
		else
		{
			// if max is 0, then r = g = b = 0              
			// s = 0, h is undefined
			out.S = 0.0f;
			out.H = NAN;                            // its now undefined
			return out;
		}
		if ( rRgba.R >= max )                           // > is bogus, just keeps compilor happy
			out.H = ( rRgba.G - rRgba.B ) / delta;        // between yellow & magenta
		else
			if ( rRgba.G >= max )
				out.H = 2.0f + ( rRgba.B - rRgba.R ) / delta;  // between cyan & yellow
			else
				out.H = 4.0f + ( rRgba.R - rRgba.G ) / delta;  // between magenta & cyan

		out.H *= 60.0f;                              // degrees

		if ( out.H < 0.0f )
			out.H += 360.0f;

		return out;
	}

	// Hsva2Rgba // Taken from stackoverflow
	//////////////////////////////////////////////////
	inline SRgba Hsva2Rgba( SHsva& rHsva )
	{
		SRgba out;
		out.A = rHsva.A;

		if ( rHsva.S <= 0.0f )
		{       // < is bogus, just shuts up warnings
			out.R = rHsva.V;
			out.G = rHsva.V;
			out.B = rHsva.V;
			return out;
		}
		float hh = rHsva.H;
		if ( hh >= 360.0f ) hh = 0.0f;
		hh /= 60.0f;
		uint16_t i = ( uint16_t )hh;
		float ff = hh - i;
		float p = rHsva.V * ( 1.0f - rHsva.S );
		float q = rHsva.V * ( 1.0f - ( rHsva.S * ff ) );
		float t = rHsva.V * ( 1.0f - ( rHsva.S * ( 1.0f - ff ) ) );

		switch ( i )
		{
			case 0:
			out.R = rHsva.V;
			out.G = t;
			out.B = p;
			break;
			case 1:
			out.R = q;
			out.G = rHsva.V;
			out.B = p;
			break;
			case 2:
			out.R = p;
			out.G = rHsva.V;
			out.B = t;
			break;

			case 3:
			out.R = p;
			out.G = q;
			out.B = rHsva.V;
			break;
			case 4:
			out.R = t;
			out.G = p;
			out.B = rHsva.V;
			break;
			case 5:
			default:
			out.R = rHsva.V;
			out.G = p;
			out.B = q;
			break;
		}
		return out;
	}


	// Attempt at traslating code below from GLSL to C++ - Not working atm

	//// Hue2Rgb
	////////////////////////////////////////////////////
	//inline SRgba Hue2Rgb( const float h ) noexcept
	//{
	//	return SRgba( grMath::Clamp( grMath::Abs( h * 6.0f - 3.0f ) - 1.0f, 0.0f, 1.0f ),
	//				  grMath::Clamp( 2.0f - grMath::Abs( h * 6.0f - 2.0f ), 0.0f, 1.0f ),
	//				  grMath::Clamp( 2.0f - grMath::Abs( h * 6.0f - 4.0f ), 0.0f, 1.0f ) );
	//}

	//// Rgb2Hcv
	////////////////////////////////////////////////////
	//inline SHsva Rgb2Hcv( SRgba& rgba ) noexcept
	//{
	//	SHsva p = ( rgba.G < rgba.B ) ? SHsva( rgba.B, rgba.G, -1.0f, 2.0f / 3.0f ) : SHsva( rgba.G, rgba.B, 0.0f, -1.0f / 3.0f );
	//	SHsva q = ( rgba.R < p.H ) ? SHsva( p.H, p.S, 0.0f, rgba.R ) : SHsva( rgba.R, p.S, p.V, p.H );
	//	float c = q.H - grMath::Min( q.A, q.S );
	//	float h = grMath::Abs( ( q.A, - q.S ) / ( 6.0f * c + grMath::EpsilonColor ) + q.V );
	//	return SHsva( h, c, q.H, rgba.A );
	//}

	//// Rgb2Hsv
	////////////////////////////////////////////////////
	//inline SHsva Rgb2Hsv( SRgba& rgba ) noexcept
	//{
	//	SHsva hcv = Rgb2Hcv( rgba );
	//	float s = hcv.S / ( hcv.V + grMath::EpsilonColor );
	//	return SHsva( hcv.H, s, hcv.V, rgba.A );
	//}

	//// Hsv2Rgb
	////////////////////////////////////////////////////
	//inline SRgba Hsv2Rgb( const SHsva& hsva ) noexcept
	//{
	//	SRgba rgba = Hue2Rgb( hsva.H );
	//	rgba.A = hsva.A;

	//	rgba.R -= 1.0f;
	//	rgba.G -= 1.0f;
	//	rgba.B -= 1.0f;

	//	rgba.R *= hsva.S;
	//	rgba.G *= hsva.S;
	//	rgba.B *= hsva.S;

	//	rgba.R += 1.0f;
	//	rgba.G += 1.0f;
	//	rgba.B += 1.0f;

	//	rgba.R *= hsva.V;
	//	rgba.G *= hsva.V;
	//	rgba.B *= hsva.V;

	//	return rgba;
	//}
}

#endif	// _H_GRCOLOR_




// HLSL code taken from http://www.chilliant.com/rgb2hsv.html
//float3 HSVtoRGB( in float3 HSV )
//{
//	float3 RGB = HUEtoRGB( HSV.x );
//	return ( ( RGB - 1 ) * HSV.y + 1 ) * HSV.z;
//}

//float3 HUEtoRGB( in float H )
//{
//	float R = abs( H * 6 - 3 ) - 1;
//	float G = 2 - abs( H * 6 - 2 );
//	float B = 2 - abs( H * 6 - 4 );
//	return saturate( float3( R, G, B ) );
//}

//float3 RGBtoHSV( in float3 RGB )
//{
//	float3 HCV = RGBtoHCV( RGB );
//	float S = HCV.y / ( HCV.z + Epsilon );
//	return float3( HCV.x, S, HCV.z );
//}

//float Epsilon = 1e-10;
//float3 RGBtoHCV( in float3 RGB )
//{
//	// Based on work by Sam Hocevar and Emil Persson
//	float4 P = ( RGB.g < RGB.b ) ? float4( RGB.bg, -1.0, 2.0 / 3.0 ) : float4( RGB.gb, 0.0, -1.0 / 3.0 );
//	float4 Q = ( RGB.r < P.x ) ? float4( P.xyw, RGB.r ) : float4( RGB.r, P.yzx );
//	float C = Q.x - min( Q.w, Q.y );
//	float H = abs( ( Q.w - Q.y ) / ( 6 * C + Epsilon ) + Q.z );
//	return float3( H, C, Q.x );
//}


// GLSL code taken from http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
//vec3 rgb2hsv( vec3 c )
//{
//	vec4 K = vec4( 0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0 );
//	vec4 p = mix( vec4( c.bg, K.wz ), vec4( c.gb, K.xy ), step( c.b, c.g ) );
//	vec4 q = mix( vec4( p.xyw, c.r ), vec4( c.r, p.yzx ), step( p.x, c.r ) );
//
//	float d = q.x - min( q.w, q.y );
//	float e = 1.0e-10;
//	return vec3( abs( q.z + ( q.w - q.y ) / ( 6.0 * d + e ) ), d / ( q.x + e ), q.x );
//}

//vec3 hsv2rgb( vec3 c )
//{
//	vec4 K = vec4( 1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0 );
//	vec3 p = abs( fract( c.xxx + K.xyz ) * 6.0 - K.www );
//	return c.z * mix( K.xxx, clamp( p - K.xxx, 0.0, 1.0 ), c.y );
//}
