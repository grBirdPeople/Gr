#ifndef _H_GRRANDOM_
#define _H_GRRANDOM_

#include <random>
		 
#include "xorshift/xorshift.h"
#include "grCommon.h"
#include "grV2.h"

using uniform_float = std::uniform_real_distribution<float>;
using uniform_sInt = std::uniform_int_distribution<signed int>;
using uniform_uInt = std::uniform_int_distribution<unsigned int>;


// grRandXOR // Faster then mersenne twister
//////////////////////////////////////////////////
class grRandXOR
{
public:
	grRandXOR()
	{
		Seed();
	}
	grRandXOR( const grRandXOR& ) = delete;
	grRandXOR& operator=( const grRandXOR& ) = delete;
	grRandXOR( grRandXOR&& ) noexcept = delete;
	grRandXOR& operator=( grRandXOR&& ) noexcept = delete;

	inline void Seed()
	{
		m_Eng.seed( std::random_device{}( ) );		
	}

	inline std::uniform_real_distribution<float> DistF( const float min, const float max )
	{
		std::uniform_real_distribution<float> dist( min, max );
		return dist;
	}

	inline std::uniform_real_distribution<float> DistF( const grV2f& rMinMax )
	{
		std::uniform_real_distribution<float> dist( rMinMax.x, rMinMax.y );
		return dist;
	}

	inline float Float( const std::uniform_real_distribution<float>& rDist )
	{
		return rDist( m_Eng );
	}

	inline std::uniform_int_distribution<signed int> DistS( const intS min, const intS max )
	{
		std::uniform_int_distribution<signed int> dist( ( signed int )min, ( signed int )max );
		return dist;
	}

	inline std::uniform_int_distribution<signed int> DistS( const grV2s& rMinMax )
	{
		std::uniform_int_distribution<signed int> dist( ( signed int )rMinMax.x, ( signed int )rMinMax.y );
		return dist;
	}

	inline intS IntS( const std::uniform_int_distribution<signed int>& rDist )
	{
		return rDist( m_Eng );
	}

private:
	xorshift m_Eng;
};


// grRandMT // Mersenne twister
//////////////////////////////////////////////////
class grRandMT
{
public:

	grRandMT( void )
	{	
		Seed();
	}

	grRandMT( const grRandMT& ) = delete;
	grRandMT& operator=( const grRandMT& ) = delete;
	grRandMT( grRandMT&& ) noexcept = delete;
	grRandMT& operator=( grRandMT&& ) noexcept = delete;

	inline void Seed( void )
	{		
		m_Mt.seed( std::random_device{}() );
	}

	inline grV2f V2fx2( const grV2f& rV1, const grV2f& rV2 )
	{
		grV2f v;
		m_RandFloat.param( std::uniform_real_distribution<float>::param_type( rV1.x, rV2.x ) );
		v.x = m_RandFloat( m_Mt );
		m_RandFloat.param( std::uniform_real_distribution<float>::param_type( rV1.y, rV2.y ) );
		v.y = m_RandFloat( m_Mt );
		return v;
	}

	inline float V2f( const grV2f& rV )
	{
		m_RandFloat.param( std::uniform_real_distribution<float>::param_type( rV.x, rV.y ) );
		return m_RandFloat( m_Mt );
	}

	inline float Float( const float min, const float max )
	{
		m_RandFloat.param( std::uniform_real_distribution<float>::param_type( min, max ) );
		return m_RandFloat( m_Mt );
	}

	inline intS IntS( const intS min, const intS max )
	{
		m_RandIntS.param( std::uniform_int_distribution<signed int>::param_type( ( signed int )min, ( signed int )max ) );
		return ( intS )m_RandIntS( m_Mt );
	}

	inline intU IntU( const intU min, const intU max )
	{
		m_RandIntU.param( std::uniform_int_distribution<unsigned int>::param_type( ( unsigned int )min , ( unsigned int )max ) );
		return ( intU )m_RandIntU( m_Mt );
	}

private:
	std::mt19937	m_Mt;

	uniform_float	m_RandFloat;
	uniform_sInt	m_RandIntS;
	uniform_uInt	m_RandIntU;
};

#endif	// _H_GRRANDOM_