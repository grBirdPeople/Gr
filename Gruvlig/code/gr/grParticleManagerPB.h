#ifndef		_H_GRPARTICLEMANAGERPB_
#define		_H_GRPARTICLEMANAGERPB_

#define		PARTICLE_SYTEMS			1				// If threads would happen each thread can have it's own
#define		PARTICLE_EMITTERS		8				// Not really an emitter but I don't know what to call it so
#define		PARTICLE_PER_ATTRIBUTE	128
//#define		PARTICLE_TIMESTEP		1.0f / 60.0f	// If physics update for particles

#include	"grSingleton.h"

#include	"grParticleSystemPB.h"
#include	"grParticlePB.h"


// Dynamically aligned loop que, or something like that
template<typename T>
class grLoopQue
{
public:

	grLoopQue( const uInt size )
		: arrT		( new T[ size ] )
		, Size		( size )
		, StrtIdx	( 0 )
		, Quantity	( 0 )
	{}
	~grLoopQue( void )
	{
		if ( arrT != nullptr )
			delete[] arrT;
	}

	grLoopQue( grLoopQue const& ) = delete;
	grLoopQue& operator=( grLoopQue const& ) = delete;

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

	friend class grCore;

	grParticleManagerPB( void );
	~grParticleManagerPB( void );

	//////////////////////////////////////////////////

	const sInt Create( void );
	grParticlAttributePB Get( const uInt id );
	void Set( const sInt id, const grParticlAttributePB& rAtt );

	//////////////////////////////////////////////////

private:

	void Update( const float deltaT );

	//////////////////////////////////////////////////

	uPtr<uPtr<SParticleBlock>[]>			m_uPArrPartBlocks;
	uPtr<uPtr<grParticleSystemPB>[]>		m_uPSystems;
	uPtr<grLoopQue<grParticlAttributePB>>	m_AttributeQue;	// If this is should work it needs to be an array, one que for each emitter created

	uInt	m_CreatedSystems,
			m_CreatedBlocks,
			m_TotalParticles;
};


struct SParticleBlock
{
	SParticleBlock( const uInt id, const sizeT size )
		: uPArrParticle			( new grParticlePB*[ size ]() )
		, m_DeactivatePartQue	( new grLoopQue<uInt>( size ) )
		, uPAttribute			( new grParticlAttributePB() )
		, SpawnCounter			( 0.0f )
		, SpawnInMilliSec		( 1.0f / 25.0f )	// Set to zero when particle API or their likes exists
		, Id					( id )
		, PartActive			( 0 )
		, PartSize				( size )
	{
		for ( sizeT i = 0; i < PartSize; ++i )
			uPArrParticle[ i ] = new grParticlePB();
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
	uPtr<grLoopQue<uInt>>		m_DeactivatePartQue;
	uPtr<grParticlAttributePB>	uPAttribute;

	float	SpawnCounter,
			SpawnInMilliSec;

	uInt	Id,
			PartActive;

	sizeT	PartSize;
};

#endif	// _H_GRPARTICLEMANAGERPB_