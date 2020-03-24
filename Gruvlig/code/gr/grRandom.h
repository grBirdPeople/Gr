#ifndef		_H_GRRANDOM_
#define		_H_GRRANDOM_

#include	<random>

#include	"grCommon.h"


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

	inline	float	GetRandomFloat	( const float min, const float max )
	{
		m_RandFloat.param( std::uniform_real_distribution<float>::param_type( min, max ) );
		return m_RandFloat( m_Mt );
	}

	inline	sInt	GetRandomIntS	( const sInt min, const sInt max )
	{
		m_RandIntS.param( std::uniform_int_distribution<signed int>::param_type( min, max ) );
		return ( sInt )m_RandIntS( m_Mt );
	}

	inline	uInt	GetRandomIntU	( const uInt max )
	{
		m_RandIntU.param( std::uniform_int_distribution<unsigned int>::param_type( 0, max ) );
		return ( uInt )m_RandIntU( m_Mt );
	}

	//////////////////////////////////////////////////

private:

	std::mt19937								m_Mt;

	std::uniform_real_distribution<float>		m_RandFloat;
	std::uniform_int_distribution<signed int>	m_RandIntS;
	std::uniform_int_distribution<unsigned int>	m_RandIntU;
};

#endif	// _H_GRRANDOM_