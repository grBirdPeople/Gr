#ifndef _H_GRPARTICLEDATA_
#define _H_GRPARTICLEDATA_

#include "grRandom.h"
#include "grV2.h"


enum class EEqualValue
{
	NO = 0,
	YES
};


enum class EPositionType
{
	BOX = 0,
	ELLIPSE
};


struct grSEmitData
{
	grV2f SystemPosition;
	float EmitRateSec;
	float EmitRateMs;
	float SpawnTimeAcc;
	sizeT Size;
	sizeT Alive;

	grSEmitData( const grV2f& systemPosition, const float emitRateSec, const sizeT size )
		: SystemPosition( systemPosition )
		, EmitRateSec( emitRateSec )
		, EmitRateMs( 1.0f / emitRateSec )
		, SpawnTimeAcc( 0.0f )
		, Size( size )
		, Alive( 0 )
	{}
	grSEmitData( const grSEmitData& ) = default;
	grSEmitData& operator=( const grSEmitData& ) = default;
};


struct grSScaleData
{
	grRandXOR Rand;
	grV2f ScaleStart, ScaleEnd;
	EEqualValue ScaleEqualValue;

	grSScaleData()
		: ScaleStart( grV2f() )
		, ScaleEnd( grV2f() )
		, ScaleEqualValue( EEqualValue::YES )
	{}
	grSScaleData( const grSScaleData& ) = default;
	grSScaleData& operator=( const grSScaleData& ) = default;
};


struct grSVelocityData
{
	grRandXOR Rand;
	grV2f DegreesMinMax;
	grV2f ForceMinMax;
	EEqualValue DegreesEqualValue;
	EEqualValue ForceEqualValue;

	grSVelocityData()
		: DegreesMinMax( grV2f() )
		, ForceMinMax( grV2f() )
		, DegreesEqualValue( EEqualValue::YES )
		, ForceEqualValue( EEqualValue::YES )
	{}
	grSVelocityData( const grSVelocityData& ) = default;
	grSVelocityData& operator=( const grSVelocityData& ) = default;
};


struct grSPositionData
{
	grRandXOR Rand;
	grV2f RadiusMinMax, PositionMax, EllipseRadius;
	float RotationSpeed, Ellipse360;
	EEqualValue PositionEqualValue;
	EPositionType PositionType;

	grSPositionData()
		: RadiusMinMax( grV2f() )
		, RotationSpeed( 0.0f )
		, Ellipse360( 0.0f )
		, PositionEqualValue( EEqualValue::YES )
		, PositionType( EPositionType::BOX )
	{}
	grSPositionData( const grSPositionData& ) = default;
	grSPositionData& operator=( const grSPositionData& ) = default;
};


struct grSLifeData
{
	grRandXOR Rand;
	grV2f LifeMinMax;
	EEqualValue LifeEqualValue;

	grSLifeData()
		: LifeMinMax( grV2f() )
		, LifeEqualValue( EEqualValue::YES )
	{}
	grSLifeData( const grSLifeData& ) = default;
	grSLifeData& operator=( const grSLifeData& ) = default;
};


struct grSArrayData
{
	pU<grV2f[]> ScaleStart;
	pU<grV2f[]> ScaleEnd;
	pU<grV2f[]> Acceleration;
	pU<grV2f[]> Velocity;
	pU<grV2f[]> Position;
	pU<float[]> Life;

	grSArrayData( const sizeT size )
		: ScaleStart( std::make_unique<grV2f[]>( size ) )
		, ScaleEnd( std::make_unique<grV2f[]>( size ) )
		, Acceleration( std::make_unique<grV2f[]>( size ) )
		, Velocity( std::make_unique<grV2f[]>( size ) )
		, Position( std::make_unique<grV2f[]>( size ) )
		, Life( std::make_unique<float[]>( size ) )
	{}
	grSArrayData( const grSArrayData& ) = delete;
	grSArrayData& operator=( const grSArrayData& ) = delete;
	grSArrayData( grSArrayData&& ) noexcept = delete;
	grSArrayData& operator=( grSArrayData&& ) noexcept = delete;
};


struct grSParticleData
{
	pU<grSEmitData> Emit;
	pU<grSScaleData> Scale;
	pU<grSVelocityData> Velocity;
	pU<grSPositionData> Position;
	pU<grSLifeData> Life;
	pU<grSArrayData> Array;

	grSParticleData( const grV2f& systemPosition, const float emitRateSec, const sizeT size )
		: Emit( std::make_unique<grSEmitData>( systemPosition, emitRateSec, size ) )
		, Array( std::make_unique<grSArrayData>( size ) )
	{}
	grSParticleData( const grSParticleData& ) = delete;
	grSParticleData& operator=( const grSParticleData& ) = delete;
	grSParticleData( grSParticleData&& ) noexcept = delete;
	grSParticleData& operator=( grSParticleData&& ) noexcept = delete;
};

#endif // _H_GRPARTICLEDATA_