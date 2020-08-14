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
	float Dt;
	float SpawnTimeAcc;
	sizeT Size;
	sizeT Alive;
	sizeT StartIdx;
	sizeT EndIdx;

	grSEmitData( const grV2f& systemPosition, const float emitRateSec, const sizeT size )
		: SystemPosition( systemPosition )
		, EmitRateSec( emitRateSec )
		, EmitRateMs( 1.0f / emitRateSec )
		, Dt( 0.0f )
		, SpawnTimeAcc( 0.0f )
		, Size( size )
		, Alive( 0 )
		, StartIdx( 0 )
		, EndIdx( 0 )
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
		: ScaleStart( grV2f( 1.0f, 1.0f ) )
		, ScaleEnd( grV2f( 1.0f, 1.0f ) )
		, ScaleEqualValue( EEqualValue::YES )
	{}
	grSScaleData( const grSScaleData& ) = default;
	grSScaleData& operator=( const grSScaleData& ) = default;
};


struct grSMassData
{
	grRandXOR Rand;
	grV2f MassMinMax;
	EEqualValue MassEqualValue;

	grSMassData()
		: MassMinMax( grV2f( 1.0f, 1.0f ) )
		, MassEqualValue( EEqualValue::YES )
	{}
	grSMassData( const grSMassData& ) = default;
	grSMassData& operator=( const grSMassData& ) = default;
};


struct grSVelocityData
{
	grRandXOR Rand;
	grV2f DegreesMinMax;
	grV2f ForceMinMax;
	EEqualValue DegreesEqualValue;
	EEqualValue ForceEqualValue;

	grSVelocityData()
		: DegreesMinMax( grV2f( 0.0f, 0.0f ) )
		, ForceMinMax( grV2f( 100.0f, 100.0f ) )
		, DegreesEqualValue( EEqualValue::YES )
		, ForceEqualValue( EEqualValue::YES )
	{}
	grSVelocityData( const grSVelocityData& ) = default;
	grSVelocityData& operator=( const grSVelocityData& ) = default;
};


struct grSPositionData
{
	grRandXOR Rand;
	grV2f PositionOffsetMin, PositionOffsetMax, EllipseRadiusMinMax;
	float RotationSpeed, Ellipse360;
	EEqualValue PositionEqualValue;
	EPositionType PositionType;

	grSPositionData()
		: PositionOffsetMin( grV2f( 0.0f, 0.0f ) )
		, PositionOffsetMax( grV2f( 0.0f, 0.0f ) )
		, EllipseRadiusMinMax( grV2f( 5.0f, 5.0f ) )
		, Ellipse360( grMath::Pi * 2.0f )
		, RotationSpeed( 0.0f )
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
		: LifeMinMax( grV2f( 2.0f, 2.0f ) )
		, LifeEqualValue( EEqualValue::YES )
	{}
	grSLifeData( const grSLifeData& ) = default;
	grSLifeData& operator=( const grSLifeData& ) = default;
};


struct grSArrayData
{
	pU<grV2f[]> ScaleStart;
	pU<grV2f[]> ScaleEnd;
	pU<float[]> Mass;
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
		, Mass( std::make_unique<float[]>( size ) )
		, Life( std::make_unique<float[]>( size ) )
	{}
	grSArrayData( const grSArrayData& ) = delete;
	grSArrayData& operator=( const grSArrayData& ) = delete;
	grSArrayData( grSArrayData&& ) noexcept = delete;
	grSArrayData& operator=( grSArrayData&& ) noexcept = delete;
};


struct grSParticleData
{
	pU<grSEmitData> puEmit;
	pU<grSScaleData> puScale;
	pU<grSMassData> puMass;
	pU<grSVelocityData> puVelocity;
	pU<grSPositionData> puPosition;
	pU<grSLifeData> puLife;
	pU<grSArrayData> puArray;

	grSParticleData() = default;
	grSParticleData( const grSParticleData& ) = delete;
	grSParticleData& operator=( const grSParticleData& ) = delete;
	grSParticleData( grSParticleData&& ) noexcept = delete;
	grSParticleData& operator=( grSParticleData&& ) noexcept = delete;

	void Init( const grV2f& systemPosition, const float emitRateSec, const sizeT size )
	{
		// General data for spawning new particles
		puEmit = std::make_unique<grSEmitData>( systemPosition, emitRateSec, size );

		// Specific data for spawning new particles
		puMass = std::make_unique<grSMassData>();
		puVelocity = std::make_unique<grSVelocityData>();
		puPosition = std::make_unique<grSPositionData>();
		puLife = std::make_unique<grSLifeData>();

		// All data that repersents a particle
		puArray = std::make_unique<grSArrayData>( size );
	}
};

#endif // _H_GRPARTICLEDATA_