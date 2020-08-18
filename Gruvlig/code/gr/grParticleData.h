#ifndef _H_GRPARTICLEDATA_
#define _H_GRPARTICLEDATA_

#include "grColor.h"
#include "grRandom.h"
#include "grV2.h"

typedef std::uniform_real_distribution<float> FloatDist;
typedef std::uniform_int_distribution<unsigned int> IntUDist;


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
	float Dt;
	sizeT Size;
	sizeT Alive;
	sizeT StartIdx;
	sizeT EndIdx;

	grSEmitData( const grV2f& systemPosition, const float emitRateSec, const sizeT size )
		: SystemPosition( systemPosition )
		, EmitRateSec( emitRateSec )
		, EmitRateMs( 1.0f / emitRateSec )
		, SpawnTimeAcc( EmitRateMs )
		, Dt( 0.0f )
		, Size( size )
		, Alive( 0 )
		, StartIdx( 0 )
		, EndIdx( 0 )
	{}
	grSEmitData( const grSEmitData& ) = default;
	grSEmitData& operator=( const grSEmitData& ) = default;
};


struct grSColorData
{
	IntUDist ArrDist[ 8 ]; // Start R[ 0 ], G[ 1 ], B[ 2 ], A[ 3 ] // End R[ 4 ], G[ 5 ], B[ 6 ], A[ 7 ]
	grColor::Rgba ArrMinMax[ 4 ]; // Start Min[ 0 ], Max[ 1 ] // End Min[ 2 ], Max[ 3 ]
	grRandXOR Rand;
	EEqualValue StartEqual;
	EEqualValue EndEqual;
	EEqualValue LerpEqual;
	bool bHsv;

	grSColorData()
		: StartEqual( EEqualValue::NO )
		, EndEqual( EEqualValue::NO )
		, LerpEqual( EEqualValue::NO )
		, bHsv( true )
	{}
	grSColorData( const grSColorData& ) = default;
	grSColorData& operator=( const grSColorData& ) = default;
};


struct grSScaleData
{
	FloatDist ArrDist[ 4 ]; // Start X[ 0 ], Y[ 1 ] // End X[ 2 ], Y[ 3 ]
	grV2f ArrMinMax[ 4 ];
	grRandXOR Rand;
	EEqualValue StartEqual;
	EEqualValue EndEqual;

	grSScaleData()
		: StartEqual( EEqualValue::NO )
		, EndEqual( EEqualValue::NO )
	{}
	grSScaleData( const grSScaleData& ) = default;
	grSScaleData& operator=( const grSScaleData& ) = default;
};


struct grSMassData
{
	grRandXOR Rand;
	grV2f MinMax;
	EEqualValue Equal;

	grSMassData()
		: MinMax( grV2f( 1.0f, 1.0f ) )
		, Equal( EEqualValue::NO )
	{}
	grSMassData( const grSMassData& ) = default;
	grSMassData& operator=( const grSMassData& ) = default;
};


struct grSVelocityData
{
	grRandXOR Rand;
	grV2f DegreeMinMax;
	grV2f ForceMinMax;
	EEqualValue DegreeEqual;
	EEqualValue ForceEqual;

	grSVelocityData()
		: DegreeMinMax( grV2f( 0.0f, 0.0f ) )
		, ForceMinMax( grV2f( 100.0f, 100.0f ) )
		, DegreeEqual( EEqualValue::NO )
		, ForceEqual( EEqualValue::NO )
	{}
	grSVelocityData( const grSVelocityData& ) = default;
	grSVelocityData& operator=( const grSVelocityData& ) = default;
};


struct grSPositionData
{
	grRandXOR Rand;
	grV2f PositionOffsetMin;
	grV2f PositionOffsetMax;
	grV2f EllipseRadiusMin;
	grV2f EllipseRadiusMax;
	grV2f EllipseStepMinMax;
	grV2f EllipseTiltMinMax;
	float Ellipse360;
	float EllipseStepCount;
	float EllipseTiltCount;
	EEqualValue PositionEqual;
	EPositionType PositionType;

	grSPositionData()
		: PositionOffsetMin( grV2f( 0.0f, 0.0f ) )
		, PositionOffsetMax( grV2f( 0.0f, 0.0f ) )
		, EllipseRadiusMin( grV2f( 5.0f, 5.0f ) )
		, EllipseRadiusMax( grV2f( 5.0f, 5.0f ) )
		, EllipseStepMinMax( grV2f( 5.0f, 5.0f ) )
		, Ellipse360( grMath::Pi * 2.0f )
		, EllipseStepCount( 0.0f )
		, EllipseTiltCount( 0.0f )
		, PositionEqual( EEqualValue::NO )
		, PositionType( EPositionType::BOX )
	{}
	grSPositionData( const grSPositionData& ) = default;
	grSPositionData& operator=( const grSPositionData& ) = default;
};


struct grSLifeData
{
	grRandXOR Rand;
	grV2f MinMax;
	EEqualValue Equal;

	grSLifeData()
		: MinMax( grV2f( 2.0f, 2.0f ) )
		, Equal( EEqualValue::NO )
	{}
	grSLifeData( const grSLifeData& ) = default;
	grSLifeData& operator=( const grSLifeData& ) = default;
};


struct grSArrayData
{
	pU<grColor::Rgba[]> ColorStart;
	pU<grColor::Rgba[]> ColorEnd;
	pU<grV2f[]> ScaleStart;
	pU<grV2f[]> ScaleEnd;
	pU<float[]> Mass;
	pU<grV2f[]> Acceleration;
	pU<grV2f[]> Velocity;
	pU<grV2f[]> Position;
	pU<float[]> Life;

	grSArrayData( const sizeT size )
		: ColorStart( std::make_unique<grColor::Rgba[]>( size ) )
		, ColorEnd( std::make_unique<grColor::Rgba[]>( size ) )
		, ScaleStart( std::make_unique<grV2f[]>( size ) )
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
	pU<grSColorData> puColor;
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
		puColor = std::make_unique<grSColorData>();
		puScale = std::make_unique<grSScaleData>();
		puMass = std::make_unique<grSMassData>();
		puVelocity = std::make_unique<grSVelocityData>();
		puPosition = std::make_unique<grSPositionData>();
		puLife = std::make_unique<grSLifeData>();

		// Unique data that represents particles
		puArray = std::make_unique<grSArrayData>( size );
	}
};

#endif // _H_GRPARTICLEDATA_