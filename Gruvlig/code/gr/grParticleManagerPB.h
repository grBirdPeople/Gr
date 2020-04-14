#ifndef		_H_GRPARTICLEMANAGERPB_
#define		_H_GRPARTICLEMANAGERPB_

#define		PARTICLE_SYTEMS			1				// If threads would happen each thread can have it's own
#define		PARTICLE_ATTRIBUTES		5				// weird naming but is actually max num of continuasly running particle systems
#define		PARTICLE_PER_ATTRIBUTE	128
//#define		PARTICLE_TIMESTEP		1.0f / 60.0f	// If physics update for particles

#include	"grSingleton.h"

#include	"grParticleSystemPB.h"
#include	"grParticlePB.h"


// Dynamically aligned loop que was the abs sickest name I could come up with
template<typename T>
class grDALQue
{
public:

	grDALQue( const uInt size )
		: arrT		( new T[ size ] )
		, Size		( size )
		, StrtIdx	( 0 )
		, Quantity	( 0 )
	{}
	~grDALQue( void )
	{
		if ( arrT != nullptr )
			delete[] arrT;
	}

	grDALQue( grDALQue const& ) = delete;
	grDALQue& operator=( grDALQue const& ) = delete;

	const uInt GetQuantity( void ) const
	{
		return Quantity;
	}
	void Push( const T& rT )
	{
		if ( Quantity == Size )
		{
#ifdef DEBUG
			std::puts( "DALQue::Push(): Size maxed out. No element was added.\n" );
#endif // DEBUG
			return;
		}

		uInt insrtIdx = StrtIdx + Quantity;
		if ( insrtIdx > Size - 1 )
			insrtIdx = insrtIdx - Size;

		arrT[ insrtIdx ] = rT;
		++Quantity;
	}
	T Pull( void )
	{
		assert( ( Quantity > 0 ) && "DALQue::Pull(): Que is empty" );

		T elem = arrT[ StrtIdx ];

		++StrtIdx;
		if( StrtIdx > Size - 1 )
			StrtIdx = StrtIdx - Size;

		--Quantity;
		return elem;

		return 0;
	}

private:

	T*		arrT;

	sizeT	Size,
			StrtIdx,
			Quantity;
};


// grParticleManagerPB
//////////////////////////////////////////////////
class grParticleManagerPB : public grSingleton<grParticleManagerPB>
{
public:

	grParticleManagerPB( void );
	~grParticleManagerPB( void );

	//////////////////////////////////////////////////

	grParticlAttributePB* const CreateParticleSystem( void );
	void Update( const float deltaT );

	//////////////////////////////////////////////////

private:

	uPtr<uPtr<SParticleBlock>[]>		m_uPArrPartBlock;
	uPtr<uPtr<grParticleSystemPB>[]>	m_uPSystems;

	uInt	m_CreatedSystems,
			m_CreatedBlocks,
			m_TotalParticles;
};


struct SParticleBlock
{
	SParticleBlock( const uInt id, const sizeT size )
		: uPArrParticle			( new grParticlePB*[ size ]() )
		, m_DeactivatePartQue	( new grDALQue<uInt>( size ) )
		, uPAttribute			( new grParticlAttributePB() )
		, SpawnCounter			( 0.0f )
		, SpawnInMilliSec		( 1.0f / 25.0f )	// Set to zero when particle API exists
		, Id					( id )
		, PartActive		( 0 )
		, PartSize				( size )
	{
		for ( sizeT i = 0; i < PartSize; ++i )
			uPArrParticle[ i ] = new grParticlePB();

		uPAttribute->Id = Id;
	}
	~SParticleBlock( void )
	{
		for ( sizeT i = 0; i < PartSize; ++i )
		{
			if ( uPArrParticle[ i ] != nullptr )
				DELANDNULL( uPArrParticle[ i ] );
		}
		delete[] uPArrParticle.release();

		if( m_DeactivatePartQue != nullptr )
			delete m_DeactivatePartQue.release();

		if ( uPAttribute != nullptr )
			delete uPAttribute.release();
	}
	SParticleBlock( SParticleBlock const& ) = delete;
	SParticleBlock& operator=( SParticleBlock  const& ) = delete;

	//////////////////////////////////////////////////

	uPtr<grParticlePB*[]>		uPArrParticle;
	uPtr<grDALQue<uInt>>		m_DeactivatePartQue;
	uPtr<grParticlAttributePB>	uPAttribute;

	float	SpawnCounter,
			SpawnInMilliSec;

	uInt	Id,
			PartActive;

	sizeT	PartSize;
};

#endif	// _H_GRPARTICLEMANAGERPB_