#ifndef		_H_GRRANDOM_
#define		_H_GRRANDOM_

#include	<random>

#include	"grCommon.h"

typedef	std::uniform_real_distribution<float>		uniform_float;
typedef	std::uniform_int_distribution<signed int>	uniform_intS;
typedef	std::uniform_int_distribution<unsigned int>	uniform_intU;


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

	inline	void	ReSeed			( void )
	{		
		m_Mt.seed( std::random_device{}() );
	}

	inline	float	GetRandFloat	( const float min, const float max )
	{
		m_RandFloat.param( std::uniform_real_distribution<float>::param_type( min, max ) );
		return m_RandFloat( m_Mt );
	}

	inline	sInt	GetRandIntS		( const sInt min, const sInt max )
	{
		m_RandIntS.param( std::uniform_int_distribution<signed int>::param_type( min, max ) );
		return ( sInt )m_RandIntS( m_Mt );
	}

	inline	uInt	GetRandIntU		( const uInt max )
	{
		m_RandIntU.param( std::uniform_int_distribution<unsigned int>::param_type( 0, max ) );
		return ( uInt )m_RandIntU( m_Mt );
	}

	//////////////////////////////////////////////////

private:

	std::mt19937	m_Mt;

	uniform_float	m_RandFloat;
	uniform_intS	m_RandIntS;
	uniform_intU	m_RandIntU;
};

#endif	// _H_GRRANDOM_