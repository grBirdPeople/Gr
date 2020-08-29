#ifndef _H_GRPARTICLEDATA_
#define _H_GRPARTICLEDATA_

#include "grColor.h"
#include "grRandom.h"
#include "grV2.h"

using DistF = std::uniform_real_distribution<float>;
using DistUI = std::uniform_int_distribution<unsigned int>;


enum class EEmitType
{
	ETERNAL = 0,
	BURST
};


enum class EEqualValue
{
	YES = 0,
	NO
};


enum class EPositionType
{
	BOX_FILLED = 0,
	BOX_FRAMED,
	ELLIPSE
};


struct grSArrayData
{
	// TODO: Fix this when some kind of draw system exists
	pU<sf::Vertex[]> Verts;
	//

	pU<grColor::Rgba[]> ColorStart;
	pU<grColor::Rgba[]> ColorEnd;
	pU<grV2f[]> ScaleStart;
	pU<grV2f[]> ScaleEnd;
	pU<float[]> Mass;
	pU<grV2f[]> Acceleration;
	pU<grV2f[]> Velocity;
	pU<grV2f[]> Position;
	pU<float[]> Life;
};


struct grSEmitData
{
	grV2f SystemPosition = grV2f( ( float )grCore::Instance().GetWindowSize().x * 0.5f, ( float )grCore::Instance().GetWindowSize().y * 0.5f );
	float Dt = 0.0f;
	float EmitRateSec = 200.0f;;
	float EmitRateMs = 1.0f / EmitRateSec;
	float SpawnTimeAcc = EmitRateMs;
	float BurstTimeSec = 0.5f;
	float BurstTimeAcc = 0.0f;
	sizeT EmitAcc = 0;
	sizeT Capacity = 0;
	sizeT Size = 0;
	sizeT StartIdx = 0;
	sizeT EndIdx = 0;
	sizeT DoEmit = 1; // 1 == true // 0 == false
	EEmitType EmitType = EEmitType::ETERNAL;
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
	sizeT DoHsv = 1; // 1 == true // 0 == false
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
	// For filled box: X[ 0 ], Y[ 1 ], Unused[ 2 ], Unused[ 3 ] // For framedbox: Hori[ 0 ], Vert[ 1 ], ThicknessX[ 2 ], ThicknessY[ 3 ]
	DistF ArrDistBox[ 4 ];
	// Min[ 0 ], Max[ 1 ]
	grV2f ArrMinMax[ 2 ] = { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
	grRandXOR Rand;
	grV2f BoxOrigoSN = { 0.0f, 1.0f };
	grV2f BoxOrigoEW = { 1.0f, 0.0f };
	grV2f BoxFrameOffset = { 0.0f, 0.0f };
	float BoxFrameThickness = 0.0f;
	float BoxFrameDegAcc = 360.0f;
	EEqualValue EqualBoxX = EEqualValue::YES;
	EEqualValue EqualBoxY = EEqualValue::YES;
	EEqualValue EqualCircle = EEqualValue::YES;
	EPositionType PositionType = EPositionType::BOX_FILLED;


	//DistF DistX;
	//DistF DistY;
	//grV2f PositionOffsetMin = grV2f( 0.0f, 0.0f );
	//grV2f PositionOffsetMax = grV2f( 0.0f, 0.0f );
	//float Ellipse360 = grMath::Pi * 2.0f;
	//float EllipseStepCount = 0.0f;
	//float EllipseTiltCount = 0.0f;
};


struct grSLifeData
{
	grRandXOR Rand;
	DistF Dist;
	grV2f MinMax = grV2f( 1.0f, 1.0f );
	EEqualValue Equal = EEqualValue::YES;
};


class grCParticleData
{
public:
	// Unique data that represents particles
	grSArrayData ArrayData;
	// General data for spawning new particles
	grSEmitData EmitData;
	// Specific data for spawning new particles
	grSColorData ColorData;
	grSScaleData ScaleData;
	grSMassData MassData;
	grSVelocityData VelocityData;
	grSPositionData PositionData;
	grSLifeData LifeData;

	grCParticleData( const sizeT size )
	{
		// TODO: Fix this when some kind of draw system exists
		ArrayData.Verts = std::make_unique<sf::Vertex[]>( size );
		//

		ArrayData.ColorStart = std::make_unique<grColor::Rgba[]>( size );
		ArrayData.ColorEnd = std::make_unique<grColor::Rgba[]>( size );
		ArrayData.ScaleStart = std::make_unique<grV2f[]>( size );
		ArrayData.ScaleEnd = std::make_unique<grV2f[]>( size );
		ArrayData.Acceleration = std::make_unique<grV2f[]>( size );
		ArrayData.Velocity = std::make_unique<grV2f[]>( size );
		ArrayData.Position = std::make_unique<grV2f[]>( size );
		ArrayData.Mass = std::make_unique<float[]>( size );
		ArrayData.Life = std::make_unique<float[]>( size );

		EmitData.Capacity = size;
	}
	grCParticleData( const grCParticleData& ) = delete;
	grCParticleData& operator=( const grCParticleData& ) = delete;
	grCParticleData( grCParticleData&& ) noexcept = delete;
	grCParticleData& operator=( grCParticleData&& ) noexcept = delete;
};

#endif // _H_GRPARTICLEDATA_




// Saved for later testing with memory allocator

//enum class EParticleSystemSize
//{
//	SMALL = 25000,
//	MEDIUM = 50000,
//	LARGE = 100000
//};


//template<sizeT S>
//struct grSArrayData
//{
//	grColor::Rgba ColorStart[ S ];
//	grColor::Rgba ColorEnd[ S ];
//	grV2f ScaleStart[ S ];
//	grV2f ScaleEnd[ S ];
//	float Mass[ S ];
//	grV2f Acceleration[ S ];
//	grV2f Velocity[ S ];
//	grV2f Position[ S ];
//	float Life[ S ];
//
//	// TODO: Fix this when some kind of draw system exists
//	sf::Vertex Verts[ S ];
//	//
//};