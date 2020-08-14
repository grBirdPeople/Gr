#ifndef _H_GRPARTICLESYSTEM_
#define _H_GRPARTICLESYSTEM_

#include "grAlgo.h"
#include "grColor.h"
//#include "grParticleArray.h"
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

	void InitStartEnd( const grV2f& rStart, const grV2f& rEnd, EEqualValue& rEqual )
	{
		rEqual = grMath::CmpV2f( rStart, rEnd ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void InitMinMaxX2( grV2f& rMin, grV2f& rMax, EEqualValue& rEqual )
	{
		if ( rMin.x > rMax.x )
			grAlgo::Swap( rMin.x, rMax.x );
		if ( rMin.y > rMax.y )
			grAlgo::Swap( rMin.y, rMax.y );

		rEqual = grMath::CmpV2f( rMin, rMax ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void InitMinMax( grV2f& rMinMax, EEqualValue& rEqual )
	{
		if ( rMinMax.x > rMinMax.y )
			grAlgo::Swap( rMinMax.x, rMinMax.y );

		rEqual = grMath::CmpFloat( rMinMax.x, rMinMax.y ) ? EEqualValue::YES : EEqualValue::NO;
	}
};


struct grSScaleSystem : public grSBaseSystem
{
	grSScaleData& Data;
	grSArrayData& Array;

	grSScaleSystem( grSScaleData& rData, grSArrayData& rArray )
		: Data( rData )
		, Array( rArray )
	{}
	grSScaleSystem( const grSScaleSystem& ) = default;
	grSScaleSystem& operator=( const grSScaleSystem& ) = default;

	void Init( const grV2f& rStart, const grV2f& rEnd )
	{
		Data.ScaleStart = rStart;
		Data.ScaleEnd = rEnd;
		InitStartEnd( Data.ScaleStart, Data.ScaleEnd, Data.ScaleEqualValue );
	}

	void Generate( const sizeT startIdx, const sizeT endIdx )
	{
		if ( Data.ScaleEqualValue == EEqualValue::NO )
		{
			float midX{ grMath::AbsF( Data.ScaleStart.x - Data.ScaleEnd.x ) * 0.5f };
			float midY{ grMath::AbsF( Data.ScaleStart.y - Data.ScaleEnd.y ) * 0.5f };

			auto distY2{ Data.Rand.DistF( Data.ScaleStart.y, midY ) };
			auto distY1{ Data.Rand.DistF( midY, Data.ScaleStart.y ) };
			auto distX2{ Data.Rand.DistF( Data.ScaleStart.x, midX ) };
			auto distX1{ Data.Rand.DistF( midX, Data.ScaleStart.x ) };

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f result( Data.ScaleStart.x > midX ? Data.Rand.Float( distX1 ) : Data.Rand.Float( distX2 ),
							  Data.ScaleStart.y > midY ? Data.Rand.Float( distY1 ) : Data.Rand.Float( distY2 ) );

				Array.ScaleStart[ i ] = result;
			}

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f result( Data.ScaleEnd.x > midX ? Data.Rand.Float( distX1 ) : Data.Rand.Float( distX2 ),
							  Data.ScaleEnd.y > midY ? Data.Rand.Float( distY1 ) : Data.Rand.Float( distY2 ) );

				Array.ScaleStart[ i ] = result;
			}
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			Array.ScaleStart[ i ] = Data.ScaleStart;
	}

	void Update( const sizeT alive, const float dt )
	{
		for ( sizeT i = 0; i < alive; ++i )
		{
			float step{ 1.0f / Array.Life[ i ] * dt };
			Array.ScaleStart[ i ] = grMath::LerpV2f( Array.ScaleStart[ i ], Array.ScaleEnd[ i ], step );
		}
	}
};


struct grSVelocitySystem : public grSBaseSystem
{
	grSVelocityData& Data;
	grSArrayData& Array;

	grSVelocitySystem( grSVelocityData& rData, grSArrayData& rArray )
		: Data( rData )
		, Array( rArray )
	{}
	grSVelocitySystem( const grSVelocitySystem& ) = default;
	grSVelocitySystem& operator=( const grSVelocitySystem& ) = default;

	void Init( const grV2f& rDegreesMinMax = grV2f(), const grV2f& rForceMinMax = grV2f( 1.0f, 1.0f ) )
	{
		Data.DegreesMinMax = grV2f( grMath::Clamp<float>( rDegreesMinMax.x, 0.0f, 350.0f ), grMath::Clamp<float>( rDegreesMinMax.y, 0.0f, 360.0f ) );
		Data.ForceMinMax = rForceMinMax;
		InitMinMax( Data.DegreesMinMax, Data.DegreesEqualValue );
		InitMinMax( Data.ForceMinMax, Data.ForceEqualValue );
	}

	void Generate( const sizeT startIdx, const sizeT endIdx )
	{
		if ( Data.DegreesEqualValue == EEqualValue::NO )
		{
			if ( Data.ForceEqualValue == EEqualValue::NO )
				DegressNoForceNo( startIdx, endIdx ); return;

			DegressNoForceYes( startIdx, endIdx ); return;
		}

		if ( Data.ForceEqualValue == EEqualValue::NO )
			DegreesYesForceNo( startIdx, endIdx ); return;

		DegressYesForceYes( startIdx, endIdx );
	}

	void DegressNoForceNo( const sizeT startIdx, const sizeT endIdx )
	{
		auto distDeg{ Data.Rand.DistF( Data.DegreesMinMax.x, Data.DegreesMinMax.y ) };
		auto distForce{ Data.Rand.DistF( Data.ForceMinMax.x, Data.ForceMinMax.y ) };
		for ( sizeT i = startIdx; i < endIdx; ++i )
			Array.Velocity[ i ] = grMath::DegToVec( Data.Rand.Float( distDeg ) ) * Data.Rand.Float( distForce ); // TODO: Missing mass division
	}

	void DegressNoForceYes( const sizeT startIdx, const sizeT endIdx )
	{
		grV2f v{ grMath::DegToVec( Data.DegreesMinMax.x ) };
		auto distForce{ Data.Rand.DistF( Data.ForceMinMax.x, Data.ForceMinMax.y ) };
		for ( sizeT i = startIdx; i < endIdx; ++i )
			Array.Velocity[ i ] = v * Data.Rand.Float( distForce ); // TODO: Missing mass division
	}

	void DegreesYesForceNo( const sizeT startIdx, const sizeT endIdx )
	{
		auto distDeg{ Data.Rand.DistF( Data.DegreesMinMax.x, Data.DegreesMinMax.y ) };
		float f{ Data.ForceMinMax.x };
		for ( sizeT i = startIdx; i < endIdx; ++i )
			Array.Velocity[ i ] = grMath::DegToVec( Data.Rand.Float( distDeg ) ) * f; // TODO: Missing mass division
	}

	void DegressYesForceYes( const sizeT startIdx, const sizeT endIdx )
	{
		grV2f v{ grMath::DegToVec( Data.DegreesMinMax.x ) };
		float f{ Data.ForceMinMax.x };
		for ( sizeT i = startIdx; i < endIdx; ++i )
			Array.Velocity[ i ] = v * f; // TODO: Missing mass division
	}

	void Update( const sizeT alive, const float dt )
	{
		for ( sizeT i = 0; i < alive; ++i )
			Array.Velocity[ i ] += Array.Acceleration[ i ].x * dt;
	}
};


struct grSPositionSystem : public grSBaseSystem
{
	grSPositionData& Data;
	grSArrayData& Array;

	grSPositionSystem( grSPositionData& rData, grSArrayData& rArray )
		: Data( rData )
		, Array( rArray )
	{}
	grSPositionSystem( const grSPositionSystem& ) = default;
	grSPositionSystem& operator=( const grSPositionSystem& ) = default;

	void Init( const grV2f& rRadiusMinMax, const float rotationSpeed, EPositionType positionType )
	{
		Data.RadiusMinMax = rRadiusMinMax;
		Data.RotationSpeed = rotationSpeed;
		Data.Ellipse360 = grMath::Pi * 2.0f;
		Data.PositionType = positionType;
		InitMinMax( Data.RadiusMinMax, Data.PositionEqualValue );
	}

	void Generate( const grV2f& rSystemPosition, const sizeT startIdx, const sizeT endIdx, const float dt )
	{
		if ( Data.PositionEqualValue == EEqualValue::NO )
		{
			if ( Data.PositionType == EPositionType::BOX )
			{
				PositionNoBox( rSystemPosition, startIdx, endIdx );
				return;
			}
			PositionNoEllipse( rSystemPosition, startIdx, endIdx, dt );
			return;
		}

		if ( Data.PositionType == EPositionType::BOX )
		{
			PositionYesBox( rSystemPosition, startIdx, endIdx );
			return;
		}
		PositionYesEllipse( rSystemPosition, startIdx, endIdx );
	}

	void PositionNoBox( const grV2f& rSystemPosition, const sizeT startIdx, const sizeT endIdx )
	{

	}

	void PositionNoEllipse( const grV2f& rSystemPosition, const sizeT startIdx, const sizeT endIdx, const float dt )
	{

	}

	void PositionYesBox( const grV2f& rSystemPosition, const sizeT startIdx, const sizeT endIdx )
	{

	}

	void PositionYesEllipse( const grV2f& rSystemPosition, const sizeT startIdx, const sizeT endIdx )
	{

	}

	void Update( const sizeT alive, const float dt )
	{
		for ( sizeT i = 0; i < alive; ++i )
			Array.Position[ i ] += Array.Velocity[ i ].x * dt;
	}
};


struct grSLifeSystem : public grSBaseSystem
{
	grSLifeData& Data;
	grSArrayData& Array;

	grSLifeSystem( grSLifeData& rData, grSArrayData& rArray )
		: Data( rData )
		, Array( rArray )
	{}
	grSLifeSystem( const grSLifeSystem& ) = default;
	grSLifeSystem& operator=( const grSLifeSystem& ) = default;

	void Init( const grV2f& rLifeMinMax )
	{
		Data.LifeMinMax = rLifeMinMax;
		InitMinMax( Data.LifeMinMax, Data.LifeEqualValue );
	}

	void Generate( const sizeT startIdx, const sizeT endIdx )
	{
		if ( Data.LifeEqualValue == EEqualValue::NO )
		{
			auto dist{ Data.Rand.DistF( Data.LifeMinMax.x, Data.LifeMinMax.y ) };
			for ( sizeT i = startIdx; i < endIdx; ++i )
				Array.Life[ i ] = Data.Rand.Float( dist );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			Array.Life[ i ] = Data.LifeMinMax.x;
	}

	void Update( sizeT& alive, const float dt )
	{
		for ( sizeT i = 0; i < alive; ++i )
		{
			Array.Life[ i ] -= dt;
			if ( Array.Life[ i ] <= 0.0f )
				Kill( Array, i, --alive );

		}
	}

	void Kill( grSArrayData& rArray, const sizeT nowIdx, const sizeT last )
	{
		//grAlgo::Swap( Array.puVerts[ nowIdx ], Array.puVerts[ last ] );
		//grAlgo::Swap( rParticleArr->puColorStart[ nowIdx ], rParticleArr->puColorStart[ last ] );
		//grAlgo::Swap( rParticleArr->puColorEnd[ nowIdx ], rParticleArr->puColorEnd[ last ] );
		grAlgo::Swap( Array.ScaleStart[ nowIdx ], Array.ScaleStart[ last ] );
		grAlgo::Swap( Array.ScaleEnd[ nowIdx ], Array.ScaleEnd[ last ] );
		grAlgo::Swap( Array.Acceleration[ nowIdx ], Array.Acceleration[ last ] );
		grAlgo::Swap( Array.Velocity[ nowIdx ], Array.Velocity[ last ] );
		//grAlgo::Swap( rParticleArr->puMass[ nowIdx ], rParticleArr->puMass[ last ] );
		grAlgo::Swap( Array.Life[ nowIdx ], Array.Life[ last ] );

		// Most values are generated and set by operator= and does not need zeroing (all above)
		// Values that are generated and set by operator+= needs resetting obviously (all below)
		//Array.Acceleration[ last ] = { 0.0f, 0.0f };
	}
};


struct grSParticleSystem
{
	pU<grSScaleSystem> Scale;
	pU<grSVelocitySystem> Velocity;
	pU<grSPositionSystem> Position;
	pU<grSLifeSystem> Life;

	grSParticleSystem() = default;
	grSParticleSystem( const grSParticleSystem& ) = delete;
	grSParticleSystem& operator=( const grSParticleSystem& ) = delete;
	grSParticleSystem( grSParticleSystem&& ) noexcept = delete;
	grSParticleSystem& operator=( grSParticleSystem&& ) noexcept = delete;
};

#endif // _H_GRPARTICLESYSTEM_