#ifndef		_H_GRPARTICLE_EMITTER_
#define		_H_GRPARTICLE_EMITTER_

#define		EMITR_USR_SETTINGS	8	// Umh not so safe, probably look for option

#include	<bitset>

#include	"grCommon.h"
#include	"grV2.h"

#include	"grSPartAttribute.h"


class grCParticleEmitter
{
public:

	friend class grCParticleManager;
	friend class grCParticleSystem;

	grCParticleEmitter( const uInt id );
	~grCParticleEmitter( void );

	grCParticleEmitter( const grCParticleEmitter& ) = delete;
	grCParticleEmitter& operator=( const grCParticleEmitter& ) = delete;

	inline const uInt Id( void ) const
	{
		return m_Id;
	}
	inline void Position( const grV2f& rPos, const float radOffset = 0.0f )
	{
		m_uPAtt->Position = rPos;
		m_uPAtt->PosOffsetRadius = radOffset;
		m_UsrMods.set( ( sizeT )EUsrMods::POS );
	}
	inline void Direction( const float minDeg, const float maxDeg )		// Only werks with positive values
	{
		m_uPAtt->MinMaxDirInDeg = grV2f( minDeg, maxDeg );
		m_UsrMods.set( ( sizeT )EUsrMods::DIR_DEG );
	}
	inline void Speed( const float min, const float max )
	{
		m_uPAtt->MinMaxSpeed = grV2f( min, max );
		m_UsrMods.set( ( sizeT )EUsrMods::SPD );
	}
	inline void Lifetime( const float min, const float max )
	{
		m_uPAtt->MinMaxLife = grV2f( min, max );
		m_UsrMods.set( ( sizeT )EUsrMods::LIFE_SEC, true );
	}

private:

	// TODO: It's a little problematic that the enum decleration order needs to match the data copy switch in systems or vv. Look into auto thing.
	enum class EUsrMods	// Max size is define EMITR_USR_SETTINGS - 1
	{
		POS,
		DIR_DEG, 
		SPD,
		LIFE_SEC,
		SIZE
	};

	vE<uInt> GetUsrModsSortd( void )	// Called from grCParticleSystem::CpyEmitrAttData
	{
		vE<sizeT>	m_UsrModsSortdReturn;
		for ( sizeT i = 0; i < m_UsrMods.size(); ++i )
		{
			if ( m_UsrMods[ i ] == true )
				m_UsrModsSortdReturn.push_back( i );
		}
		m_UsrMods.reset();

		return m_UsrModsSortdReturn;
	}

	uP<grSParticleAttribute>	m_uPAtt;

	std::bitset<EMITR_USR_SETTINGS>	m_UsrMods;

	float	m_SpawnRate,
			m_SpawnTimer;

	uInt	m_Id,
			m_PartActive;
};

#endif	// _H_GRPARTICLE_EMITTER_


