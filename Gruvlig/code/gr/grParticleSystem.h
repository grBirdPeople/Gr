#ifndef _H_GRPARTICLESYSTEM_
#define _H_GRPARTICLESYSTEM_

#include "grAlgo.h"
#include "grColor.h"
#include "grParticleData.h"


struct grSBaseSystem
{
	void InitColor( grColor::SRgba& rStart, grColor::SRgba& rEnd )
	{
		rStart.R = grMath::Clamp<float>( rStart.R, 0.0f, 255.0f );
		rStart.G = grMath::Clamp<float>( rStart.G, 0.0f, 255.0f );
		rStart.B = grMath::Clamp<float>( rStart.B, 0.0f, 255.0f );
		rStart.A = grMath::Clamp<float>( rStart.A, 0.0f, 255.0f );

		rEnd.R = grMath::Clamp<float>( rEnd.R, 0.0f, 255.0f );
		rEnd.G = grMath::Clamp<float>( rEnd.G, 0.0f, 255.0f );
		rEnd.B = grMath::Clamp<float>( rEnd.B, 0.0f, 255.0f );
		rEnd.A = grMath::Clamp<float>( rEnd.A, 0.0f, 255.0f );
	}

	void InitMinMax( grV2f& rMinMax, EEqualValue& rEqual, const bool swap = false )
	{
		if ( swap )
		{
			if ( rMinMax.x > rMinMax.y )
				grAlgo::Swap( rMinMax.x, rMinMax.y );
		}

		rEqual = grMath::CmpFloat( rMinMax.x, rMinMax.y ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void InitMinMax( grV2f& rMin, grV2f& rMax, EEqualValue& rEqual, const bool swap = false )
	{
		if ( swap )
		{
			if ( rMin.x > rMax.x )
				grAlgo::Swap( rMin.x, rMax.x );
			if ( rMin.y > rMax.y )
				grAlgo::Swap( rMin.y, rMax.y );
		}

		rEqual = grMath::CmpV2f( rMin, rMax ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void InitStartEnd( const grV2f& rStart, const grV2f& rEnd, EEqualValue& rEqual )
	{
		rEqual = grMath::CmpV2f( rStart, rEnd ) ? EEqualValue::YES : EEqualValue::NO;
	}
};


struct grSScaleSystem : public grSBaseSystem
{
	grSEmitData& rEmit;
	grSScaleData& rScale;
	grSArrayData& rArray;

	grSScaleSystem( const grSParticleData& rData )
		: rEmit( *rData.puEmit )
		, rScale( *rData.puScale )
		, rArray( *rData.puArray )
	{}
	grSScaleSystem( const grSScaleSystem& ) = default;
	grSScaleSystem& operator=( const grSScaleSystem& ) = default;

	void Init( const grV2f& rStart, const grV2f& rEnd )
	{
		rScale.ScaleStart = rStart;
		rScale.ScaleEnd = rEnd;
		InitStartEnd( rScale.ScaleStart, rScale.ScaleEnd, rScale.ScaleEqualValue );
	}

	void Generate()
	{
		sizeT startIdx{ rEmit.StartIdx };
		sizeT endIdx{ rEmit.EndIdx };
		if ( rScale.ScaleEqualValue == EEqualValue::NO )
		{
			float midX{ grMath::AbsF( rScale.ScaleStart.x - rScale.ScaleEnd.x ) * 0.5f };
			float midY{ grMath::AbsF( rScale.ScaleStart.y - rScale.ScaleEnd.y ) * 0.5f };

			auto distY2{ rScale.Rand.DistF( rScale.ScaleStart.y, midY ) };
			auto distY1{ rScale.Rand.DistF( midY, rScale.ScaleStart.y ) };
			auto distX2{ rScale.Rand.DistF( rScale.ScaleStart.x, midX ) };
			auto distX1{ rScale.Rand.DistF( midX, rScale.ScaleStart.x ) };

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f result( rScale.ScaleStart.x > midX ? rScale.Rand.Float( distX1 ) : rScale.Rand.Float( distX2 ),
							  rScale.ScaleStart.y > midY ? rScale.Rand.Float( distY1 ) : rScale.Rand.Float( distY2 ) );

				rArray.ScaleStart[ i ] = result;
			}

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f result( rScale.ScaleEnd.x > midX ? rScale.Rand.Float( distX1 ) : rScale.Rand.Float( distX2 ),
							  rScale.ScaleEnd.y > midY ? rScale.Rand.Float( distY1 ) : rScale.Rand.Float( distY2 ) );

				rArray.ScaleStart[ i ] = result;
			}
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ScaleStart[ i ] = rScale.ScaleStart;
	}

	void Update()
	{
		for ( sizeT i = 0; i < rEmit.Alive; ++i )
		{
			float step{ 1.0f / rArray.Life[ i ] * rEmit.Dt };
			rArray.ScaleStart[ i ] = grMath::LerpV2f( rArray.ScaleStart[ i ], rArray.ScaleEnd[ i ], step );
		}
	}
};


struct grSMassSystem : public grSBaseSystem
{
	grSEmitData& rEmit;
	grSMassData& rMass;
	grSArrayData& rArray;

	grSMassSystem( const grSParticleData& rData )
		: rEmit( *rData.puEmit )
		, rMass( *rData.puMass )
		, rArray( *rData.puArray )
	{}
	grSMassSystem( const grSMassSystem& ) = default;
	grSMassSystem& operator=( const grSMassSystem& ) = default;

	void Init( const grV2f& rMassMinMax )
	{
		rMass.MassMinMax.x = grMath::Max( rMassMinMax.x, 1.0f );
		rMass.MassMinMax.y = grMath::Max( rMassMinMax.y, 1.0f );
		InitMinMax( rMass.MassMinMax, rMass.MassEqualValue );
	}

	void Generate()
	{
		if ( rMass.MassEqualValue == EEqualValue::NO )
		{
			auto dist{ rMass.Rand.DistF( rMass.MassMinMax.x, rMass.MassMinMax.y ) };
			for ( sizeT i = rEmit.StartIdx; i < rEmit.EndIdx; ++i )
				rArray.Mass[ i ] = rMass.Rand.Float( dist );

			return;
		}

		for ( sizeT i = rEmit.StartIdx; i < rEmit.EndIdx; ++i )
			rArray.Mass[ i ] = rMass.MassMinMax.x;
	}
};


struct grSVelocitySystem : public grSBaseSystem
{
	grSEmitData& rEmit;
	grSVelocityData& rVelocity;
	grSArrayData& rArray;

	grSVelocitySystem( const grSParticleData& rData )
		: rEmit( *rData.puEmit )
		, rVelocity( *rData.puVelocity )
		, rArray( *rData.puArray )
	{}
	grSVelocitySystem( const grSVelocitySystem& ) = default;
	grSVelocitySystem& operator=( const grSVelocitySystem& ) = default;

	void Init( const grV2f& rDegreesMinMax, const grV2f& rForceMinMax )
	{
		rVelocity.DegreesMinMax = grV2f( grMath::Clamp<float>( rDegreesMinMax.x, 0.0f, 359.9f ), grMath::Clamp<float>( rDegreesMinMax.y, 0.0f, 359.9f ) );
		rVelocity.ForceMinMax = rForceMinMax;
		InitMinMax( rVelocity.DegreesMinMax, rVelocity.DegreesEqualValue, false );
		InitMinMax( rVelocity.ForceMinMax, rVelocity.ForceEqualValue );
	}

	void Generate()
	{
		sizeT startIdx{ rEmit.StartIdx };
		sizeT endIdx{ rEmit.EndIdx };
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			float d{ FindDegrees() };
			float f{ FindForce() };
			grV2f v{ grMath::DegToVec( d ) * f };
			rArray.Velocity[ i ] = v / rArray.Mass[ i ];
		}
	}

	float FindDegrees()
	{
		if ( rVelocity.DegreesEqualValue == EEqualValue::NO )
		{
			if ( rVelocity.DegreesMinMax.x > rVelocity.DegreesMinMax.y )
			{
				float d{ 0.0f };
				float diff{ 359.9f - rVelocity.DegreesMinMax.x };
				auto distDeg{ rVelocity.Rand.DistF( 0.0f, diff + rVelocity.DegreesMinMax.y ) };
				d = rVelocity.Rand.Float( distDeg ) - diff;
				if ( d < 0.0f )
					d += 359.9f;

				return d;
			}

			auto distDeg{ rVelocity.Rand.DistF( rVelocity.DegreesMinMax.x, rVelocity.DegreesMinMax.y ) };
			return rVelocity.Rand.Float( distDeg );
		}

		return rVelocity.DegreesMinMax.x;
	}

	float FindForce()
	{
		if ( rVelocity.ForceEqualValue == EEqualValue::NO )
		{
			auto distForce{ rVelocity.Rand.DistF( rVelocity.ForceMinMax.x, rVelocity.ForceMinMax.y ) };
			return { rVelocity.Rand.Float( distForce ) };
		}

		return { rVelocity.ForceMinMax.x };
	}

	void Update()
	{
		for ( sizeT i = 0; i < rEmit.Alive; ++i )
			rArray.Velocity[ i ] += rArray.Acceleration[ i ].x * rEmit.Dt;
	}
};


// TEST DRAW
#include	"grBBox.h"
#include	"grDebugManager.h"
// TEST DRAW

struct grSPositionSystem : public grSBaseSystem
{
	grSEmitData& rEmit;
	grSPositionData& rPosition;
	grSArrayData& rArray;

	grSPositionSystem( const grSParticleData& rData )
		: rEmit( *rData.puEmit )
		, rPosition( *rData.puPosition )
		, rArray( *rData.puArray )
	{}
	grSPositionSystem( const grSPositionSystem& ) = default;
	grSPositionSystem& operator=( const grSPositionSystem& ) = default;

	void Init( const EPositionType positionType, const grV2f& rPositionOffsetMin, const grV2f& rPositionOffsetMax )
	{
		rPosition.PositionOffsetMin = rPositionOffsetMin;
		rPosition.PositionOffsetMax = rPositionOffsetMax;
		rPosition.PositionType = positionType;
		InitMinMax( rPosition.PositionOffsetMin, rPosition.PositionOffsetMax, rPosition.PositionEqualValue );
	}

	void Generate()
	{
		sizeT startIdx{ rEmit.StartIdx };
		sizeT endIdx{ rEmit.EndIdx };
		if ( rPosition.PositionEqualValue == EEqualValue::NO )
		{
			if ( rPosition.PositionType == EPositionType::BOX )
			{
				PositionNotEqualBox();
				return;
			}
			//PositionNotEqualEllipse( rSystemPosition, startIdx, endIdx, dt );
			return;
		}

		if ( rPosition.PositionType == EPositionType::BOX )
		{
			PositionEqualBox();
			return;
		}
		//PositionEqualEllipse( rSystemPosition, startIdx, endIdx );
	}

	void PositionNotEqualBox()
	{
		auto distX{ rPosition.Rand.DistF( rPosition.PositionOffsetMin.x, rPosition.PositionOffsetMax.x ) };
		auto distY{ rPosition.Rand.DistF( rPosition.PositionOffsetMin.y, rPosition.PositionOffsetMax.y ) };
		for ( sizeT i = rEmit.StartIdx; i < rEmit.EndIdx; ++i )
		{
			grV2f v{ rPosition.Rand.Float( distX ), rPosition.Rand.Float( distY ) };
			rArray.Position[ i ] = v + rEmit.SystemPosition;
		}
	}

	void PositionNotEqualEllipse()
	{

	}

	void PositionEqualBox()
	{
		grV2f v{ rPosition.PositionOffsetMin + rEmit.SystemPosition };
		for ( sizeT i = rEmit.StartIdx; i < rEmit.EndIdx; ++i )
			rArray.Position[ i ] = v;
	}

	void PositionEqualEllipse()
	{

	}

	void Update()
	{
		for ( sizeT i = 0; i < rEmit.Alive; ++i )
			rArray.Position[ i ] += rArray.Velocity[ i ] * rEmit.Dt;



		// TEST DRAW
		for ( sizeT i = 0; i < rEmit.Alive; ++i )
			grDebugManager::Instance().AddBBox( { { 1.0f, 1.0f }, rArray.Position[ i ] }, sf::Color::Green );
		// TEST DRAW
	}
};


struct grSLifeSystem : public grSBaseSystem
{
	grSEmitData& rEmit;
	grSLifeData& rLife;
	grSArrayData& rArray;

	grSLifeSystem( const grSParticleData& rData )
		: rEmit( *rData.puEmit )
		, rLife( *rData.puLife )
		, rArray( *rData.puArray )
	{}
	grSLifeSystem( const grSLifeSystem& ) = default;
	grSLifeSystem& operator=( const grSLifeSystem& ) = default;

	void Init( const grV2f& rLifeMinMax )
	{
		rLife.LifeMinMax = rLifeMinMax;
		InitMinMax( rLife.LifeMinMax, rLife.LifeEqualValue );
	}

	void Generate()
	{
		if ( rLife.LifeEqualValue == EEqualValue::NO )
		{
			auto dist{ rLife.Rand.DistF( rLife.LifeMinMax.x, rLife.LifeMinMax.y ) };
			for ( sizeT i = rEmit.StartIdx; i < rEmit.EndIdx; ++i )
				rArray.Life[ i ] = rLife.Rand.Float( dist );

			return;
		}

		for ( sizeT i = rEmit.StartIdx; i < rEmit.EndIdx; ++i )
			rArray.Life[ i ] = rLife.LifeMinMax.x;
	}

	void Update()
	{
		for ( sizeT i = 0; i < rEmit.Alive; ++i )
		{
			rArray.Life[ i ] -= rEmit.Dt;;
			if ( rArray.Life[ i ] <= 0.0f )
				Kill( i, --rEmit.Alive );
		}
	}

	void Kill( const sizeT nowIdx, const sizeT last )
	{
		//grAlgo::Swap( Array.puVerts[ nowIdx ], Array.puVerts[ last ] );
		//grAlgo::Swap( rParticleArr->puColorStart[ nowIdx ], rParticleArr->puColorStart[ last ] );
		//grAlgo::Swap( rParticleArr->puColorEnd[ nowIdx ], rParticleArr->puColorEnd[ last ] );
		//grAlgo::Swap( rArray.ScaleStart[ nowIdx ], rArray.ScaleStart[ last ] );
		//grAlgo::Swap( rArray.ScaleEnd[ nowIdx ], rArray.ScaleEnd[ last ] );
		grAlgo::Swap( rArray.Acceleration[ nowIdx ], rArray.Acceleration[ last ] );
		grAlgo::Swap( rArray.Velocity[ nowIdx ], rArray.Velocity[ last ] );
		grAlgo::Swap( rArray.Position[ nowIdx ], rArray.Position[ last ] );
		grAlgo::Swap( rArray.Mass[ nowIdx ], rArray.Mass[ last ] );
		grAlgo::Swap( rArray.Life[ nowIdx ], rArray.Life[ last ] );

		// Most values are generated and set by operator= and does not need zeroing (all above)
		// Values that are generated and set by operator+= needs resetting obviously (all below)
		rArray.Acceleration[ last ] = { 0.0f, 0.0f };
	}
};


struct grSParticleSystem
{
	pU<grSScaleSystem> puScale;
	pU<grSMassSystem> puMass;
	pU<grSVelocitySystem> puVelocity;
	pU<grSPositionSystem> puPosition;
	pU<grSLifeSystem> puLife;

	grSParticleSystem() = default;
	grSParticleSystem( const grSParticleSystem& ) = delete;
	grSParticleSystem& operator=( const grSParticleSystem& ) = delete;
	grSParticleSystem( grSParticleSystem&& ) noexcept = delete;
	grSParticleSystem& operator=( grSParticleSystem&& ) noexcept = delete;

	void Init( const grSParticleData& rData )
	{
		puMass = std::make_unique<grSMassSystem>( rData );
		puVelocity = std::make_unique<grSVelocitySystem>( rData );
		puPosition = std::make_unique<grSPositionSystem>( rData );
		puLife = std::make_unique<grSLifeSystem>( rData );
	}
};

#endif // _H_GRPARTICLESYSTEM_