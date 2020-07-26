#ifndef		_H_GRPARTICLEEMITTERPB_
#define		_H_GRPARTICLEEMITTERPB_

#include	"grParticlePB.h"
#include	"grStruct.h"


// grSParticleEmitter // Never allowed to directly copy the complete object
//////////////////////////////////////////////////
class grCParticleEmitterPB
{
public:

	friend struct grSParticleSystemPB;

	using vecUpPart = std::vector<pU<grSParticlePB>>;

	//////////////////////////////////////////////////

	grCParticleEmitterPB( const intU id, const sizeT size )
		: uPPartDeactivateQue		( new grStruct::LoopQue<intU>( size ) )		
		, uPArrPartDeactivateSortd	( new intU[ size ] )
		, SpawnCounter				( 0.0f )
		, SpawnInMilliSec			( 1.0f / 25.0f )	// Set to zero when particle API or their likes exists
		, Id						( id )
		, PartActive				( 0 )
		, PartSize					( size )
	{
		vecUpParticles.reserve( PartSize );
		for ( sizeT i = 0; i < PartSize; ++i )
			vecUpParticles.push_back( std::move( std::make_unique<grSParticlePB>() ) );

		uPPartAttribute = std::make_unique<grCParticleAttributePB>();
	}
	~grCParticleEmitterPB( void )
	{
		for ( sizeT i = 0; i < PartSize; ++i )
		{
			if ( vecUpParticles[ i ] != nullptr )
				delete vecUpParticles[ i ].release();
		}
		vecUpParticles.clear();

		if ( uPPartDeactivateQue != nullptr )
			delete uPPartDeactivateQue.release();

		if ( uPArrPartDeactivateSortd != nullptr )
			delete[] uPArrPartDeactivateSortd.release();

		if( uPPartAttribute != nullptr )
			delete uPPartAttribute.release();
	}
	grCParticleEmitterPB( grCParticleEmitterPB const& ) = delete;
	grCParticleEmitterPB& operator=( grCParticleEmitterPB const& ) = delete;

	//////////////////////////////////////////////////

	const intU GetId( void ) const
	{
		return Id;
	}
	grCParticleAttributePB& GetParticleAttribute( void ) const
	{
		return *uPPartAttribute.get(); 
	}
	void SetParticleAttribute( grCParticleAttributePB& rAtt )
	{
		*uPPartAttribute.get() = rAtt;
	}
	void SetPartSpawnPerSec( const float spawnInSec )
	{
		SpawnInMilliSec = 1.0f / grMath::Clamp( spawnInSec, 0.0f, 128 );	// TODO: Instead of using 128 get something real
	}

	//////////////////////////////////////////////////

private:

	vecUpPart	vecUpParticles;

	pU<grStruct::LoopQue<intU>>	uPPartDeactivateQue;

	pU<intU[]>	uPArrPartDeactivateSortd;

	pU<grCParticleAttributePB>	uPPartAttribute;


	float	SpawnCounter,
			SpawnInMilliSec;

	intU	Id,
			PartActive;

	sizeT	PartSize;
};

#endif	// _H_GRPARTICLEEMITTERPB_
