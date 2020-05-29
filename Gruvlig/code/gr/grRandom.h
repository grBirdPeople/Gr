#ifndef		_H_GRRANDOM_
#define		_H_GRRANDOM_

#include	<random>

#include	"grCommon.h"

using uniform_float = std::uniform_real_distribution<float>;
using uniform_sInt = std::uniform_int_distribution<signed int>;
using uniform_uInt = std::uniform_int_distribution<unsigned int>;


// grRandom
//////////////////////////////////////////////////
class grRandom
{
public:

	grRandom( void )
	{
		ReSeed();
	}

	//////////////////////////////////////////////////

	inline void ReSeed( void )
	{		
		m_Mt.seed( std::random_device{}() );
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

	//////////////////////////////////////////////////

private:

	std::mt19937	m_Mt;

	uniform_float	m_RandFloat;
	uniform_sInt	m_RandIntS;
	uniform_uInt	m_RandIntU;
};

#endif	// _H_GRRANDOM_