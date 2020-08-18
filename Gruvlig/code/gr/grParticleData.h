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
	pU<IntUDist[]> puDistArr; // Start R[ 0 ], G[ 1 ], B[ 2 ], A[ 3 ] // End R[ 4 ], G[ 5 ], B[ 6 ], A[ 7 ]
	grRandXOR Rand;
	grColor::Rgba ColorStartMin;
	grColor::Rgba ColorStartMax;
	grColor::Rgba ColorEndMin;
	grColor::Rgba ColorEndMax;
	EEqualValue ColorStartEqual;
	EEqualValue ColorEndEqual;
	bool bHsv;

	grSColorData()
		: puDistArr( std::make_unique<IntUDist[]>( 8 ) )
		, ColorStartMin( grColor::Rgba() )
		, ColorStartMax( grColor::Rgba() )
		, ColorEndMin( grColor::Rgba() )
		, ColorEndMax( grColor::Rgba() )
		, ColorStartEqual( EEqualValue::YES )
		, ColorEndEqual( EEqualValue::YES )
		, bHsv( true )
	{}
	grSColorData( const grSColorData& ) = default;
	grSColorData& operator=( const grSColorData& ) = default;
};


struct grSScaleData
{
	pU<FloatDist[]> puDistArr; // Start X[ 0 ], Y[ 1 ] // End X[ 4 ], Y[ 5 ]
	grRandXOR Rand;
	grV2f ScaleStartMin;
	grV2f ScaleStartMax;
	grV2f ScaleEndMin;
	grV2f ScaleEndMax;
	EEqualValue ScaleStartEqual;
	EEqualValue ScaleEndEqual;

	grSScaleData()
		: puDistArr( std::make_unique<FloatDist[]>( 4 ) )
		, ScaleStartMin( grV2f( 1.0f, 1.0f ) )
		, ScaleStartMax( grV2f( 1.0f, 1.0f ) )
		, ScaleEndMin( grV2f( 1.0f, 1.0f ) )
		, ScaleEndMax( grV2f( 1.0f, 1.0f ) )
		, ScaleStartEqual( EEqualValue::YES )
		, ScaleEndEqual( EEqualValue::YES )
	{}
	grSScaleData( const grSScaleData& ) = default;
	grSScaleData& operator=( const grSScaleData& ) = default;
};


struct grSMassData
{
	grRandXOR Rand;
	grV2f MassMinMax;
	EEqualValue EqualMass;

	grSMassData()
		: MassMinMax( grV2f( 1.0f, 1.0f ) )
		, EqualMass( EEqualValue::YES )
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
		, DegreeEqual( EEqualValue::YES )
		, ForceEqual( EEqualValue::YES )
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
		, PositionEqual( EEqualValue::YES )
		, PositionType( EPositionType::BOX )
	{}
	grSPositionData( const grSPositionData& ) = default;
	grSPositionData& operator=( const grSPositionData& ) = default;
};


struct grSLifeData
{
	grRandXOR Rand;
	grV2f LifeMinMax;
	EEqualValue LifeEqual;

	grSLifeData()
		: LifeMinMax( grV2f( 2.0f, 2.0f ) )
		, LifeEqual( EEqualValue::YES )
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