#ifndef _H_GRPARTICLEDATA_
#define _H_GRPARTICLEDATA_

#include "grColor.h"
#include "grRandom.h"
#include "grV2.h"

typedef std::uniform_real_distribution<float> DistF;
typedef std::uniform_int_distribution<unsigned int> DistUI;


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
	grV2f SystemPosition = grV2f( ( float )grCore::Instance().GetWindowSize().x * 0.5f, ( float )grCore::Instance().GetWindowSize().y * 0.5f );
	float Dt = 0.0f;
	float EmitRateSec = 200.0f;;
	float EmitRateMs = 1.0f / EmitRateSec;
	float SpawnTimeAcc = 0.0f;
	sizeT EmitAcc = 0;
	sizeT Size = 0;
	sizeT Alive = 0;
	sizeT StartIdx = 0;
	sizeT EndIdx = 0;

	grSEmitData( const sizeT size )
		: Size( size )
	{}
	grSEmitData( const grSEmitData& ) = delete;
	grSEmitData& operator=( const grSEmitData& ) = delete;
	grSEmitData( grSEmitData&& ) noexcept = delete;
	grSEmitData& operator=( grSEmitData&& ) noexcept = delete;
};


struct grSColorData
{
	// Start R[ 0 ], G[ 1 ], B[ 2 ], A[ 3 ] // End R[ 4 ], G[ 5 ], B[ 6 ], A[ 7 ]
	DistUI ArrDist[ 8 ];
	// Start Min[ 0 ], Max[ 1 ] // End Min[ 2 ], Max[ 3 ]
	grColor::Rgba ArrMinMax[ 4 ] = { { 0, 0, 0, 0 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 } , { 0, 0, 0, 0 } };
	grRandXOR Rand;
	EEqualValue StartEqual = EEqualValue::YES;
	EEqualValue EndEqual = EEqualValue::YES;
	EEqualValue LerpEqual = EEqualValue::YES;
	bool bHsv = true;
};


struct grSScaleData
{
	// Start X[ 0 ], Y[ 1 ] // End X[ 2 ], Y[ 3 ]
	DistF ArrDist[ 4 ];
	// Start Min[ 0 ], Max[ 1 ] // End Min[ 2 ], Max[ 3 ]
	grV2f ArrMinMax[ 4 ] = { { 0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.5f, 0.5f } , { 0.5f, 0.5f } };
	grRandXOR Rand;
	EEqualValue StartEqual = EEqualValue::YES;
	EEqualValue EndEqual = EEqualValue::YES;
};


struct grSMassData
{
	grRandXOR Rand;
	DistF Dist;
	grV2f MinMax = grV2f( 1.0f, 1.0f );
	EEqualValue Equal = EEqualValue::YES;
};


struct grSVelocityData
{
	grRandXOR Rand;
	DistF Dist;
	grV2f DegreeMinMax = grV2f( 315.0f, 45.0f );
	grV2f ForceMinMax = grV2f( 25.0f, 75.0f );
	EEqualValue DegreeEqual = EEqualValue::NO;
	EEqualValue ForceEqual = EEqualValue::NO;
};


struct grSPositionData
{
	grRandXOR Rand;
	DistF DistX;
	DistF DistY;
	grV2f PositionOffsetMin = grV2f( 0.0f, 0.0f );
	grV2f PositionOffsetMax = grV2f( 0.0f, 0.0f );
	grV2f EllipseRadiusMin = grV2f( 5.0f, 5.0f );
	grV2f EllipseRadiusMax = grV2f( 5.0f, 5.0f );
	grV2f EllipseStepMinMax = grV2f( 5.0f, 5.0f );
	grV2f EllipseTiltMinMax = grV2f( 0.0f, 0.0f );
	float Ellipse360 = grMath::Pi * 2.0f;
	float EllipseStepCount = 0.0f;
	float EllipseTiltCount = 0.0f;
	EEqualValue PositionEqual = EEqualValue::YES;
	EPositionType PositionType = EPositionType::BOX;
};


struct grSLifeData
{
	grRandXOR Rand;
	DistF Dist;
	grV2f MinMax = grV2f( 1.0f, 1.0f );
	EEqualValue Equal = EEqualValue::YES;
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

	void Init( const sizeT size )
	{
		// General data for spawning new particles
		puEmit = std::make_unique<grSEmitData>( size );

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