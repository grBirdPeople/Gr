#ifndef _H_GRPARTICLESYSTEM_
#define _H_GRPARTICLESYSTEM_

#include "grAlgo.h"
#include "grColor.h"
#include "grParticleArray.h"
#include "grParticleComponent.h"


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
	grSScaleComponent& Component;
	grSParticleArrays& Arrays;

	grSScaleSystem( grSScaleComponent& rComponent, grSParticleArrays& rArrays )
		: Component( rComponent )
		, Arrays( rArrays )
	{}
	grSScaleSystem( const grSScaleSystem& ) = default;
	grSScaleSystem& operator=( const grSScaleSystem& ) = default;

	void Init( const grV2f& rStart, const grV2f& rEnd )
	{
		Component.ScaleStart = rStart;
		Component.ScaleEnd = rEnd;
		InitStartEnd( Component.ScaleStart, Component.ScaleEnd, Component.ScaleEqualValue );
	}

	void Generate( const sizeT startIdx, const sizeT endIdx )
	{
		if ( Component.ScaleEqualValue == EEqualValue::NO )
		{
			float midX{ grMath::AbsF( Component.ScaleStart.x - Component.ScaleEnd.x ) * 0.5f };
			float midY{ grMath::AbsF( Component.ScaleStart.y - Component.ScaleEnd.y ) * 0.5f };

			auto distY2{ Component.Rand.DistF( Component.ScaleStart.y, midY ) };
			auto distY1{ Component.Rand.DistF( midY, Component.ScaleStart.y ) };
			auto distX2{ Component.Rand.DistF( Component.ScaleStart.x, midX ) };
			auto distX1{ Component.Rand.DistF( midX, Component.ScaleStart.x ) };

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f result( Component.ScaleStart.x > midX ? Component.Rand.Float( distX1 ) : Component.Rand.Float( distX2 ),
							  Component.ScaleStart.y > midY ? Component.Rand.Float( distY1 ) : Component.Rand.Float( distY2 ) );

				Arrays.ScaleStart[ i ] = result;
			}

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f result( Component.ScaleEnd.x > midX ? Component.Rand.Float( distX1 ) : Component.Rand.Float( distX2 ),
							  Component.ScaleEnd.y > midY ? Component.Rand.Float( distY1 ) : Component.Rand.Float( distY2 ) );

				Arrays.ScaleStart[ i ] = result;
			}
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			Arrays.ScaleStart[ i ] = Component.ScaleStart;
	}

	void Update( const sizeT alive, const float dt )
	{
		for ( sizeT i = 0; i < alive; ++i )
		{
			float step{ 1.0f / Arrays.Life[ i ] * dt };
			Arrays.ScaleStart[ i ] = grMath::LerpV2f( Arrays.ScaleStart[ i ], Arrays.ScaleEnd[ i ], step );
		}
	}
};


struct grSVelocitySystem : public grSBaseSystem
{
	grSVelocityComponent& Component;
	grSParticleArrays& Arrays;

	grSVelocitySystem( grSVelocityComponent& rComponent, grSParticleArrays& rArrays )
		: Component( rComponent )
		, Arrays( rArrays )
	{}
	grSVelocitySystem( const grSVelocitySystem& ) = default;
	grSVelocitySystem& operator=( const grSVelocitySystem& ) = default;

	void Init( const grV2f& rDegreesMinMax = grV2f(), const grV2f& rForceMinMax = grV2f( 1.0f, 1.0f ) )
	{
		Component.DegreesMinMax = grV2f( grMath::Clamp<float>( rDegreesMinMax.x, 0.0f, 350.0f ), grMath::Clamp<float>( rDegreesMinMax.y, 0.0f, 360.0f ) );
		Component.ForceMinMax = rForceMinMax;
		InitMinMax( Component.DegreesMinMax, Component.DegreesEqualValue );
		InitMinMax( Component.ForceMinMax, Component.ForceEqualValue );
	}

	void Generate( const sizeT startIdx, const sizeT endIdx )
	{
		if ( Component.DegreesEqualValue == EEqualValue::NO )
		{
			if ( Component.ForceEqualValue == EEqualValue::NO )
				DegressNoForceNo( startIdx, endIdx ); return;

			DegressNoForceYes( startIdx, endIdx ); return;
		}

		if ( Component.ForceEqualValue == EEqualValue::NO )
			DegreesYesForceNo( startIdx, endIdx ); return;

		DegressYesForceYes( startIdx, endIdx );
	}

	void DegressNoForceNo( const sizeT startIdx, const sizeT endIdx )
	{
		auto distDeg{ Component.Rand.DistF( Component.DegreesMinMax.x, Component.DegreesMinMax.y ) };
		auto distForce{ Component.Rand.DistF( Component.ForceMinMax.x, Component.ForceMinMax.y ) };
		for ( sizeT i = startIdx; i < endIdx; ++i )
			Arrays.Velocity[ i ] = grMath::DegToVec( Component.Rand.Float( distDeg ) ) * Component.Rand.Float( distForce ); // TODO: Missing mass division
	}

	void DegressNoForceYes( const sizeT startIdx, const sizeT endIdx )
	{
		grV2f v{ grMath::DegToVec( Component.DegreesMinMax.x ) };
		auto distForce{ Component.Rand.DistF( Component.ForceMinMax.x, Component.ForceMinMax.y ) };
		for ( sizeT i = startIdx; i < endIdx; ++i )
			Arrays.Velocity[ i ] = v * Component.Rand.Float( distForce ); // TODO: Missing mass division
	}

	void DegreesYesForceNo( const sizeT startIdx, const sizeT endIdx )
	{
		auto distDeg{ Component.Rand.DistF( Component.DegreesMinMax.x, Component.DegreesMinMax.y ) };
		float f{ Component.ForceMinMax.x };
		for ( sizeT i = startIdx; i < endIdx; ++i )
			Arrays.Velocity[ i ] = grMath::DegToVec( Component.Rand.Float( distDeg ) ) * f; // TODO: Missing mass division
	}

	void DegressYesForceYes( const sizeT startIdx, const sizeT endIdx )
	{
		grV2f v{ grMath::DegToVec( Component.DegreesMinMax.x ) };
		float f{ Component.ForceMinMax.x };
		for ( sizeT i = startIdx; i < endIdx; ++i )
			Arrays.Velocity[ i ] = v * f; // TODO: Missing mass division
	}

	void Update( const sizeT alive, const float dt )
	{
		for ( sizeT i = 0; i < alive; ++i )
			Arrays.Velocity[ i ] += Arrays.Acceleration[ i ].x * dt;
	}
};


struct grSPositionSystem : public grSBaseSystem
{
	grSPositionComponent& Component;
	grSParticleArrays& Arrays;

	grSPositionSystem( grSPositionComponent& rComponent, grSParticleArrays& rArrays )
		: Component( rComponent )
		, Arrays( rArrays )
	{}
	grSPositionSystem( const grSPositionSystem& ) = default;
	grSPositionSystem& operator=( const grSPositionSystem& ) = default;

	void Init( const grV2f& rRadiusMinMax, const float rotationSpeed, EPositionType positionType )
	{
		Component.RadiusMinMax = rRadiusMinMax;
		Component.RotationSpeed = rotationSpeed;
		Component.Ellipse360 = grMath::Pi * 2.0f;
		Component.PositionType = positionType;
		InitMinMax( Component.RadiusMinMax, Component.PositionEqualValue );
	}

	void Generate( const grV2f& rSystemPosition, const sizeT startIdx, const sizeT endIdx, const float dt )
	{
		if ( Component.PositionEqualValue == EEqualValue::NO )
		{
			if ( Component.PositionType == EPositionType::BOX )
			{
				PositionNoBox( rSystemPosition, startIdx, endIdx );
				return;
			}
			PositionNoEllipse( rSystemPosition, startIdx, endIdx, dt );
			return;
		}

		if ( Component.PositionType == EPositionType::BOX )
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
			Arrays.Position[ i ] += Arrays.Velocity[ i ].x * dt;
	}
};


struct grSLifeSystem : public grSBaseSystem
{
	grSLifeComponent& Component;
	grSParticleArrays& Arrays;

	grSLifeSystem( grSLifeComponent& rComponent, grSParticleArrays& rArrays )
		: Component( rComponent )
		, Arrays( rArrays )
	{}
	grSLifeSystem( const grSLifeSystem& ) = default;
	grSLifeSystem& operator=( const grSLifeSystem& ) = default;

	void Init( const grV2f& rLifeMinMax )
	{
		Component.LifeMinMax = rLifeMinMax;
		InitMinMax( Component.LifeMinMax, Component.LifeEqualValue );
	}

	void Generate( const sizeT startIdx, const sizeT endIdx )
	{
		if ( Component.LifeEqualValue == EEqualValue::NO )
		{
			auto dist{ Component.Rand.DistF( Component.LifeMinMax.x, Component.LifeMinMax.y ) };
			for ( sizeT i = startIdx; i < endIdx; ++i )
				Arrays.Life[ i ] = Component.Rand.Float( dist );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			Arrays.Life[ i ] = Component.LifeMinMax.x;
	}

	void Update( sizeT& alive, const float dt )
	{
		for ( sizeT i = 0; i < alive; ++i )
		{
			Arrays.Life[ i ] -= dt;
			if ( Arrays.Life[ i ] <= 0.0f )
				Kill( Arrays, i, --alive );
		}
	}

	void Kill( grSParticleArrays& rArray, const sizeT nowIdx, const sizeT last )
	{
		//grAlgo::Swap( Array.puVerts[ nowIdx ], Array.puVerts[ last ] );
		//grAlgo::Swap( rParticleArr->puColorStart[ nowIdx ], rParticleArr->puColorStart[ last ] );
		//grAlgo::Swap( rParticleArr->puColorEnd[ nowIdx ], rParticleArr->puColorEnd[ last ] );
		grAlgo::Swap( Arrays.ScaleStart[ nowIdx ], Arrays.ScaleStart[ last ] );
		grAlgo::Swap( Arrays.ScaleEnd[ nowIdx ], Arrays.ScaleEnd[ last ] );
		grAlgo::Swap( Arrays.Acceleration[ nowIdx ], Arrays.Acceleration[ last ] );
		grAlgo::Swap( Arrays.Velocity[ nowIdx ], Arrays.Velocity[ last ] );
		//grAlgo::Swap( rParticleArr->puMass[ nowIdx ], rParticleArr->puMass[ last ] );
		grAlgo::Swap( Arrays.Life[ nowIdx ], Arrays.Life[ last ] );

		// Most values are generated and set by operator= and does not need zeroing (all above)
		// Values that are generated and set by operator+= needs resetting obviously (all below)
		//Array.Acceleration[ last ] = { 0.0f, 0.0f };
	}
};


struct grSParticleSystems
{
	pU<grSScaleSystem> Scale;
	pU<grSVelocitySystem> Velocity;
	pU<grSPositionSystem> Position;
	pU<grSLifeSystem> Life;
};

#endif // _H_GRPARTICLESYSTEM_