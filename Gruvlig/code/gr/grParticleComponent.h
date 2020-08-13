#ifndef _H_GRPARTICLECOMPONENT_
#define _H_GRPARTICLECOMPONENT_

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


struct grSScaleComponent
{
	grSScaleComponent()
		: ScaleStart( grV2f() )
		, ScaleEnd( grV2f() )
		, ScaleEqualValue( EEqualValue::YES )
	{}
	grSScaleComponent( const grSScaleComponent& ) = default;
	grSScaleComponent& operator=( const grSScaleComponent& ) = default;

	grRandXOR Rand;
	grV2f ScaleStart, ScaleEnd;
	EEqualValue ScaleEqualValue;
};


struct grSVelocityComponent
{
	grSVelocityComponent()
		: DegreesMinMax( grV2f() )
		, ForceMinMax( grV2f() )
		, DegreesEqualValue( EEqualValue::YES )
		, ForceEqualValue( EEqualValue::YES )
	{}
	grSVelocityComponent( const grSVelocityComponent& ) = default;
	grSVelocityComponent& operator=( const grSVelocityComponent& ) = default;

	grRandXOR Rand;
	grV2f DegreesMinMax;
	grV2f ForceMinMax;
	EEqualValue DegreesEqualValue;
	EEqualValue ForceEqualValue;
};


struct grSPositionComponent
{
	grSPositionComponent()
		: RadiusMinMax( grV2f() )
		, RotationSpeed( 0.0f )
		, Ellipse360( 0.0f )
		, PositionEqualValue( EEqualValue::YES )
		, PositionType( EPositionType::BOX )
	{}
	grSPositionComponent( const grSPositionComponent& ) = default;
	grSPositionComponent& operator=( const grSPositionComponent& ) = default;

	grRandXOR Rand;
	grV2f RadiusMinMax, PositionMax, EllipseRadius;
	float RotationSpeed, Ellipse360;
	EEqualValue PositionEqualValue;
	EPositionType PositionType;
};


struct grSLifeComponent
{
	grSLifeComponent()
		: LifeMinMax( grV2f() )
		, LifeEqualValue( EEqualValue::YES )
	{}
	grSLifeComponent( const grSLifeComponent& ) = default;
	grSLifeComponent& operator=( const grSLifeComponent& ) = default;

	grRandXOR Rand;
	grV2f LifeMinMax;
	EEqualValue LifeEqualValue;
};


struct grSParticleComponents
{
	pU<grSScaleComponent> Scale;
	pU<grSVelocityComponent> Velocity;
	pU<grSPositionComponent> Position;
	pU<grSLifeComponent> Life;
};

#endif // _H_GRPARTICLECOMPONENT_