#ifndef _H_GRPARTICLEARRAY_
#define _H_GRPARTICLEARRAY_

#include "grCommon.h"
#include "grV2.h"


struct grSParticleArrays
{
	grSParticleArrays( const sizeT size)
		: ScaleStart( std::make_unique<grV2f[]>( size ) )
		, ScaleEnd( std::make_unique<grV2f[]>( size ) )
		, Acceleration( std::make_unique<grV2f[]>( size ) )
		, Velocity( std::make_unique<grV2f[]>( size ) )
		, Position( std::make_unique<grV2f[]>( size ) )
		, Life( std::make_unique<float[]>( size ) )
	{}
	grSParticleArrays( const grSParticleArrays& ) = delete;
	grSParticleArrays& operator=( const grSParticleArrays& ) = delete;
	grSParticleArrays( grSParticleArrays&& ) noexcept = delete;
	grSParticleArrays& operator=( grSParticleArrays&& ) noexcept = delete;

	pU<grV2f[]> ScaleStart;
	pU<grV2f[]> ScaleEnd;
	pU<grV2f[]> Acceleration;
	pU<grV2f[]> Velocity;
	pU<grV2f[]> Position;
	pU<float[]> Life;
};

#endif // _H_GRPARTICLEARRAY_