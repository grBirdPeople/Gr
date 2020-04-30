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

	grCParticleEmitter( const intU id );
	~grCParticleEmitter( void );

	grCParticleEmitter( const grCParticleEmitter& ) = delete;
	grCParticleEmitter& operator=( const grCParticleEmitter& ) = delete;

	inline const intU Id( void ) const
	{
		return m_Id;
	}
	inline void DirectionEmitter( const float degAbs = 0.0f )
	{
		m_uPAtt->EmitrRotInDeg = degAbs;
		m_UsrMods.set( ( sizeT )EUsrMods::DIR_EMITR );
	}
	inline const float GetDirectionEmitter( void ) const
	{
		return m_uPAtt->EmitrRotInDeg;
	}
	inline void Position( const grV2f& rPos, const float radOffset = 0.0f )
	{
		m_uPAtt->EmitrPos = rPos;
		m_uPAtt->PartRadiusPosOffset = radOffset;
		m_UsrMods.set( ( intU )EUsrMods::POS );
	}
	inline void DirectionParticle( const grV2f& rMinMaxDegAbs = grV2f( 0.0f, 359.9f ) )
	{
		m_uPAtt->PartRotInDegMinMax = rMinMaxDegAbs;
		m_UsrMods.set( ( intU )EUsrMods::DIR_PART );
	}
	inline const grV2f& GetDirectionParticle( void ) const
	{
		return m_uPAtt->PartRotInDegMinMax;
	}
	inline void Speed( const grV2f& rMinMax = grV2f( 0.0f, 0.0f ), const grV2f& rMinMaxMod = grV2f( 0.0f, 0.0f ) )
	{
		m_uPAtt->PartSpdMinMax = rMinMax;
		m_uPAtt->PartSpdModMinMax = rMinMaxMod;
		m_UsrMods.set( ( intU )EUsrMods::SPD );
	}
	inline void Lifetime( const grV2f& rMinMax = grV2f( 0.0f, 0.0f ) )
	{
		m_uPAtt->PartLifeMinMax = rMinMax;
		m_UsrMods.set( ( intU )EUsrMods::LIFE, true );
	}

private:

	// TODO: It's a little problematic that the enum decleration order needs to match the data copy switch in systems or vv. Look into auto thing.
	enum class EUsrMods	// Max size is define EMITR_USR_SETTINGS - 1
	{
		POS,
		DIR_EMITR,
		DIR_PART, 
		SPD,
		LIFE,	// In sec
		SIZE
	};

	vE<intU> GetUsrModsSortd( void )	// Called from grCParticleSystem::CpyEmitrAttData
	{
		vE<sizeT> m_UsrModsSortdReturn;
		for ( sizeT i = 0; i < m_UsrMods.size(); ++i )
		{
			if ( m_UsrMods[ i ] == true )
				m_UsrModsSortdReturn.push_back( i );
		}
		m_UsrMods.reset();

		return m_UsrModsSortdReturn;
	}

	pU<grSParticleAttribute>	m_uPAtt;

	std::bitset<EMITR_USR_SETTINGS>	m_UsrMods;

	float	m_SpawnRate,
			m_SpawnTimer;

	intU	m_Id,
			m_PartActive;
};

#endif	// _H_GRPARTICLE_EMITTER_


