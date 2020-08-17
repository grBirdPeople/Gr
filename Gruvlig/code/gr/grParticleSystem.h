#ifndef _H_GRPARTICLESYSTEM_
#define _H_GRPARTICLESYSTEM_

#include "grAlgo.h"
#include "grParticleData.h"

typedef std::uniform_real_distribution<float> FloatDist;


struct grSBaseSystem
{
	void SetStartEnd( grColor::Rgba& rStartMin, grColor::Rgba& rStartMax, grColor::Rgba& rEndMin, grColor::Rgba& rEndMax, EEqualValue& rStartEqual, EEqualValue& rEndEqual )
	{
		rStartMin = ClampColor( rStartMin );
		rStartMax = ClampColor( rStartMax );
		rEndMin = ClampColor( rEndMin );
		rEndMax = ClampColor( rEndMax );

		rStartEqual = rStartMin.Cmp( rStartMax ) ? EEqualValue::YES : EEqualValue::NO;
		rEndEqual = rEndMin.Cmp( rEndMax ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void SetStartEnd( const grV2f& rStartMin, const grV2f& rStartMax, const grV2f& rEndMin, const grV2f& rEndMax, EEqualValue& rStartEqual, EEqualValue& rEndEqual )
	{
		rStartEqual = grMath::CmpV2f( rStartMin, rStartMax ) ? EEqualValue::YES : EEqualValue::NO;
		rEndEqual = grMath::CmpV2f( rEndMin, rEndMax ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void SetMinMax( grV2f& rMinMax, EEqualValue& rEqual, const bool swap = true )
	{
		if ( swap )
		{
			if ( rMinMax.x > rMinMax.y )
				grAlgo::Swap( rMinMax.x, rMinMax.y );
		}
		rEqual = grMath::CmpF( rMinMax.x, rMinMax.y ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void SetMinMax( grV2f& rMin, grV2f& rMax, EEqualValue& rEqual, const bool swap = true )
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

private:
	grColor::Rgba ClampColor( const grColor::Rgba& rColor )
	{
		return {
			grMath::Clamp<uint8_t>( rColor.R, 0, 255 ),
			grMath::Clamp<uint8_t>( rColor.G, 0, 255 ),
			grMath::Clamp<uint8_t>( rColor.B, 0, 255 ),
			grMath::Clamp<uint8_t>( rColor.A, 0, 255 )
		};
	}
};


struct grSColorSystem : public grSBaseSystem
{
	typedef void( grSColorSystem::*ColorOption )( const sizeT startIdx, const sizeT endIdx );

	grSEmitData& rEmit;
	grSColorData& rColor;
	grSArrayData& rArray;

	ColorOption Option;

	grSColorSystem( const grSParticleData& rData )
		: rEmit( *rData.puEmit )
		, rColor( *rData.puColor )
		, rArray( *rData.puArray )
		, Option( &grSColorSystem::Option0 )
	{}
	grSColorSystem( const grSColorSystem& ) = default;
	grSColorSystem& operator=( const grSColorSystem& ) = default;

	void Init( const grColor::Rgba& rStartMin, const grColor::Rgba& rStartMax, const grColor::Rgba& rEndMin, const grColor::Rgba& rEndMax, const bool hsv )
	{
		rColor.ColorStartMin = rStartMin;
		rColor.ColorStartMax = rStartMax;
		rColor.ColorEndMin = rEndMin;
		rColor.ColorEndMax = rEndMax;
		rColor.bHsv = hsv;
		SetStartEnd( rColor.ColorStartMin, rColor.ColorStartMax, rColor.ColorEndMin, rColor.ColorEndMax, rColor.ColorStartEqual, rColor.ColorEndEqual );

		rColor.puDistArr[ 0 ] = DistSetup( rColor.ColorStartMin.R, rColor.ColorStartMax.R );
		rColor.puDistArr[ 1 ] = DistSetup( rColor.ColorStartMin.G, rColor.ColorStartMax.G );
		rColor.puDistArr[ 2 ] = DistSetup( rColor.ColorStartMin.B, rColor.ColorStartMax.B );
		rColor.puDistArr[ 3 ] = DistSetup( rColor.ColorStartMin.A, rColor.ColorStartMax.A );

		rColor.puDistArr[ 4 ] = DistSetup( rColor.ColorEndMin.R, rColor.ColorEndMax.R );
		rColor.puDistArr[ 5 ] = DistSetup( rColor.ColorEndMin.G, rColor.ColorEndMax.G );
		rColor.puDistArr[ 6 ] = DistSetup( rColor.ColorEndMin.B, rColor.ColorEndMax.B );
		rColor.puDistArr[ 7 ] = DistSetup( rColor.ColorEndMin.A, rColor.ColorEndMax.A );

		Option = rColor.ColorStartEqual == EEqualValue::NO && rColor.ColorEndEqual == EEqualValue::NO ?
			&grSColorSystem::Option0 :
			rColor.ColorStartEqual == EEqualValue::NO && rColor.ColorEndEqual == EEqualValue::YES ?
			&grSColorSystem::Option1 :
			rColor.ColorStartEqual == EEqualValue::YES && rColor.ColorEndEqual == EEqualValue::NO ?
			&grSColorSystem::Option2 :
			&grSColorSystem::Option3;
	}

	void Generate()
	{
		sizeT startIdx{ rEmit.StartIdx }, endIdx{ rEmit.EndIdx };
		( this->*Option )( startIdx, endIdx );
	}

	void Update()
	{
		// Should be a conditional here if both start and end colors are the same or not

		// RGB -> HSV lerp -> RGB
		rColor.bHsv ? LerpHsv() : LerpRgb();
	}

	IntUDist DistSetup( const uint16_t rA, const uint16_t rB )
	{
		return rA < rB ? rColor.Rand.DistU( grV2u{ rA, rB } ) : rColor.Rand.DistU( grV2u{ rB, rA } );
	}

	void ColorFinal( pU<grColor::Rgba[]>& rArr, const sizeT arrIdx, const sizeT distIdx )
	{
		rArr[ arrIdx ].R = ( uint8_t )( rColor.Rand.IntU( rColor.puDistArr[ distIdx ] ) );
		rArr[ arrIdx ].G = ( uint8_t )( rColor.Rand.IntU( rColor.puDistArr[ distIdx + 1 ] ) );
		rArr[ arrIdx ].B = ( uint8_t )( rColor.Rand.IntU( rColor.puDistArr[ distIdx + 2 ] ) );
		rArr[ arrIdx ].A = ( uint8_t )( rColor.Rand.IntU( rColor.puDistArr[ distIdx + 3 ] ) );
	}

	void Option0( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			ColorFinal( rArray.ColorStart, i, 0 );

		for ( sizeT i = startIdx; i < endIdx; ++i )
			ColorFinal( rArray.ColorEnd, i, 4 );
	}

	void Option1( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			ColorFinal( rArray.ColorStart, i, 0 );

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ColorEnd[ i ] = rColor.ColorEndMin;
	}

	void Option2( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ColorStart[ i ] = rColor.ColorStartMin;

		for ( sizeT i = startIdx; i < endIdx; ++i )
			ColorFinal( rArray.ColorEnd, i, 4 );
	}

	void Option3( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ColorStart[ i ] = rColor.ColorStartMin;

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ColorEnd[ i ] = rColor.ColorEndMin;
	}

	void LerpHsv()
	{
		float dt{ rEmit.Dt };
		sizeT alive{ rEmit.Alive };
		for ( sizeT i = 0; i < alive; ++i )
		{
			float lerpValue{ 1.0f / rArray.Life[ i ] * dt };

			// Faster to first store localy and then lerp instead of passing to lerp directly by indexing the array // Measured with std::chronos
			grColor::Hsva start{ grColor::Rgba2Hsva( rArray.ColorStart[ i ] ) };
			grColor::Hsva end{ grColor::Rgba2Hsva( rArray.ColorEnd[ i ] ) };

			start.H = grMath::Lerp( start.H, end.H, lerpValue );
			start.S = grMath::Lerp( start.S, end.S, lerpValue );
			start.V = grMath::Lerp( start.V, end.V, lerpValue );
			start.A = grMath::Lerp( start.A, end.A, lerpValue );

			rArray.ColorStart[ i ] = grColor::Hsva2Rgba( start );
		}
	}

	void LerpRgb()
	{
		float dt{ rEmit.Dt };
		sizeT alive{ rEmit.Alive };
		for ( sizeT i = 0; i < alive; ++i )
		{
			float lerpValue{ 1.0f / rArray.Life[ i ] * dt };

			grColor::Rgba start{ rArray.ColorStart[ i ] };
			grColor::Rgba end{ rArray.ColorEnd[ i ] };

			rArray.ColorStart[ i ].R = ( uint8_t )grMath::Lerp( ( float )start.R, ( float )end.R, lerpValue );
			rArray.ColorStart[ i ].G = ( uint8_t )grMath::Lerp( ( float )start.G, ( float )end.G, lerpValue );
			rArray.ColorStart[ i ].B = ( uint8_t )grMath::Lerp( ( float )start.B, ( float )end.B, lerpValue );
			rArray.ColorStart[ i ].A = ( uint8_t )grMath::Lerp( ( float )start.A, ( float )end.A, lerpValue );
		}
	}
};


struct grSScaleSystem : public grSBaseSystem
{
	typedef void( grSScaleSystem::*ScaleOption )( const sizeT startIdx, const sizeT endIdx );

	grSEmitData& rEmit;
	grSScaleData& rScale;
	grSArrayData& rArray;

	FloatDist DistStartX; // TODO: Move this to data object and do the same for all other systems
	FloatDist DistStartY;
	FloatDist DistEndX;
	FloatDist DistEndY;

	ScaleOption Option;

	grSScaleSystem( const grSParticleData& rData )
		: rEmit( *rData.puEmit )
		, rScale( *rData.puScale )
		, rArray( *rData.puArray )
		, Option( &grSScaleSystem::Option0 )
	{}
	grSScaleSystem( const grSScaleSystem& ) = default;
	grSScaleSystem& operator=( const grSScaleSystem& ) = default;

	void Init( const grV2f& rStartMin, const grV2f& rStartMax, const grV2f& rEndMin, const grV2f& rEndMax )
	{		
		rScale.ScaleStartMin = rStartMin;
		rScale.ScaleStartMax = rStartMax;
		rScale.ScaleEndMin = rEndMin;
		rScale.ScaleEndMax = rEndMax;
		SetStartEnd( rScale.ScaleStartMin, rScale.ScaleStartMax, rScale.ScaleEndMin, rScale.ScaleEndMax, rScale.ScaleStartEqual, rScale.ScaleEndEqual );

		DistStartX = rScale.ScaleStartMin.x < rScale.ScaleStartMax.x ?
			rScale.Rand.DistF( rScale.ScaleStartMin.x, rScale.ScaleStartMax.x ) :
			rScale.Rand.DistF( rScale.ScaleStartMax.x, rScale.ScaleStartMin.x );

		DistStartY = rScale.ScaleStartMin.y < rScale.ScaleStartMax.y ?
			rScale.Rand.DistF( rScale.ScaleStartMin.y, rScale.ScaleStartMax.y ) :
			rScale.Rand.DistF( rScale.ScaleStartMax.y, rScale.ScaleStartMin.y );

		DistEndX = rScale.ScaleEndMin.x < rScale.ScaleEndMax.x ?
			rScale.Rand.DistF( rScale.ScaleEndMin.x, rScale.ScaleEndMax.x ) :
			rScale.Rand.DistF( rScale.ScaleEndMax.x, rScale.ScaleEndMin.x );

		DistEndY = rScale.ScaleEndMin.y < rScale.ScaleEndMax.y ?
			rScale.Rand.DistF( rScale.ScaleEndMin.y, rScale.ScaleEndMax.y ) :
			rScale.Rand.DistF( rScale.ScaleEndMax.y, rScale.ScaleEndMin.y );

		Option = rScale.ScaleStartEqual == EEqualValue::NO && rScale.ScaleEndEqual == EEqualValue::NO ?
			&grSScaleSystem::Option0 :
			rScale.ScaleStartEqual == EEqualValue::NO && rScale.ScaleEndEqual == EEqualValue::YES ?
			&grSScaleSystem::Option1 :
			rScale.ScaleStartEqual == EEqualValue::YES && rScale.ScaleEndEqual == EEqualValue::NO ?
			&grSScaleSystem::Option2 :
			&grSScaleSystem::Option3;
	}

	void Generate()
	{
		sizeT startIdx{ rEmit.StartIdx }, endIdx{ rEmit.EndIdx };
		( this->*Option )( startIdx, endIdx );
	}

	void Update()
	{
		float dt{ rEmit.Dt };
		for ( sizeT i = 0; i < rEmit.Alive; ++i )
		{
			float step{ ( 1.0f / rArray.Life[ i ] ) * dt };
			grV2f start{ rArray.ScaleStart[ i ] };
			grV2f end{ rArray.ScaleEnd[ i ] };
			rArray.ScaleStart[ i ] = grMath::LerpV2f( start, end, step );
		}
	}

	void Option0( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ScaleStart[ i ] = { rScale.Rand.Float( DistStartX ), rScale.Rand.Float( DistStartY ) };

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ScaleEnd[ i ] = { rScale.Rand.Float( DistEndX ), rScale.Rand.Float( DistEndY ) };
	}

	void Option1( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ScaleStart[ i ] = { rScale.Rand.Float( DistStartX ), rScale.Rand.Float( DistStartY ) };

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ScaleEnd[ i ] = rScale.ScaleEndMin;
	}

	void Option2( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ScaleStart[ i ] = rScale.ScaleStartMin;

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ScaleEnd[ i ] = { rScale.Rand.Float( DistEndX ), rScale.Rand.Float( DistEndY ) };
	}

	void Option3( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ScaleStart[ i ] = rScale.ScaleStartMin;

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.ScaleEnd[ i ] = rScale.ScaleEndMin;
	}
};


struct grSMassSystem : public grSBaseSystem
{
	typedef std::uniform_real_distribution<float> MassDist;

	grSEmitData& rEmit;
	grSMassData& rMass;
	grSArrayData& rArray;

	FloatDist DistMass;

	grSMassSystem( const grSParticleData& rData )
		: rEmit( *rData.puEmit )
		, rMass( *rData.puMass )
		, rArray( *rData.puArray )
	{}
	grSMassSystem( const grSMassSystem& ) = default;
	grSMassSystem& operator=( const grSMassSystem& ) = default;

	void Init( const grV2f& rMinMax )
	{
		rMass.MassMinMax.x = grMath::Max( rMinMax.x, 1.0f );
		rMass.MassMinMax.y = grMath::Max( rMinMax.y, 1.0f );
		SetMinMax( rMass.MassMinMax, rMass.EqualMass );
		DistMass = rMass.Rand.DistF( rMass.MassMinMax.x, rMass.MassMinMax.y );
	}

	void Generate()
	{
		sizeT startIdx{ rEmit.StartIdx }, endIdx{ rEmit.EndIdx };
		if ( rMass.EqualMass == EEqualValue::NO )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rArray.Mass[ i ] = rMass.Rand.Float( DistMass );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArray.Mass[ i ] = rMass.MassMinMax.x;
	}
};


struct grSVelocitySystem : public grSBaseSystem
{
	grSEmitData& rEmit;
	grSVelocityData& rVelocity;
	grSArrayData& rArray;

	FloatDist VelDist;

	grSVelocitySystem( const grSParticleData& rData )
		: rEmit( *rData.puEmit )
		, rVelocity( *rData.puVelocity )
		, rArray( *rData.puArray )
	{}
	grSVelocitySystem( const grSVelocitySystem& ) = default;
	grSVelocitySystem& operator=( const grSVelocitySystem& ) = default;

	void Init( const grV2f& rDegreeMinMax, const grV2f& rForceMinMax )
	{
		rVelocity.DegreeMinMax = grV2f( grMath::Clamp<float>( rDegreeMinMax.x, 0.0f, 359.9f ), grMath::Clamp<float>( rDegreeMinMax.y, 0.0f, 359.9f ) );
		rVelocity.ForceMinMax = rForceMinMax;
		SetMinMax( rVelocity.DegreeMinMax, rVelocity.DegreeEqual, false );
		SetMinMax( rVelocity.ForceMinMax, rVelocity.ForceEqual );
	}

	void Generate()
	{
		// This is retarded but I'm retarded and can't figure out how to do it without using 6 different loops which would be better but look more retarded
		// It shall remain retarded for the moment(s)

		sizeT startIdx{ rEmit.StartIdx }, endIdx{ rEmit.EndIdx };
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			float d{ FindDegrees() };
			float f{ FindForce() };
			grV2f v{ grMath::DegToVec( d ) * f };
			rArray.Velocity[ i ] = v / rArray.Mass[ i ];
		}
	}

	void Update()
	{
		float dt{ rEmit.Dt };
		for ( sizeT i = 0; i < rEmit.Alive; ++i )
			rArray.Velocity[ i ] += rArray.Acceleration[ i ].x * dt;
	}

	float FindDegrees()
	{
		if ( rVelocity.DegreeEqual == EEqualValue::NO )
		{
			if ( rVelocity.DegreeMinMax.x > rVelocity.DegreeMinMax.y )
			{
				float d{ 0.0f };
				float diff{ 359.9f - rVelocity.DegreeMinMax.x };
				VelDist = rVelocity.Rand.DistF( 0.0f, diff + rVelocity.DegreeMinMax.y );
				d = rVelocity.Rand.Float( VelDist ) - diff;
				if ( d < 0.0f )
					d += 359.9f;

				return d;
			}

			auto distDeg{ rVelocity.Rand.DistF( rVelocity.DegreeMinMax.x, rVelocity.DegreeMinMax.y ) };
			return rVelocity.Rand.Float( distDeg );
		}

		return rVelocity.DegreeMinMax.x;
	}

	float FindForce()
	{
		if ( rVelocity.ForceEqual == EEqualValue::NO )
		{
			auto distForce{ rVelocity.Rand.DistF( rVelocity.ForceMinMax.x, rVelocity.ForceMinMax.y ) };
			return rVelocity.Rand.Float( distForce );
		}

		return rVelocity.ForceMinMax.x;
	}
};


struct grSPositionSystem : public grSBaseSystem
{
	grSEmitData& rEmit;
	grSPositionData& rPosition;
	grSArrayData& rArray;

	FloatDist DistPosX;
	FloatDist DistPosY;

	grSPositionSystem( const grSParticleData& rData )
		: rEmit( *rData.puEmit )
		, rPosition( *rData.puPosition )
		, rArray( *rData.puArray )
	{}
	grSPositionSystem( const grSPositionSystem& ) = default;
	grSPositionSystem& operator=( const grSPositionSystem& ) = default;

	void Init( const EPositionType positionType, const grV2f& rOffsetMin, const grV2f& rOffsetMax )
	{
		rPosition.PositionOffsetMin = rOffsetMin;
		rPosition.PositionOffsetMax = rOffsetMax;
		rPosition.PositionType = positionType;
		SetMinMax( rPosition.PositionOffsetMin, rPosition.PositionOffsetMax, rPosition.PositionEqual );

		DistPosX = rPosition.Rand.DistF( rPosition.PositionOffsetMin.x, rPosition.PositionOffsetMax.x );
		DistPosY = rPosition.Rand.DistF( rPosition.PositionOffsetMin.y, rPosition.PositionOffsetMax.y );
	}

	void Init( const EPositionType positionType, const grV2f& rRadiusMin, const grV2f& rRadiusMax, const grV2f& rStepMinMax, const grV2f& rTiltMinMax )
	{
		rPosition.EllipseRadiusMin = rRadiusMin;
		rPosition.EllipseRadiusMax = rRadiusMax;
		rPosition.EllipseStepMinMax = rStepMinMax;
		rPosition.EllipseTiltMinMax = rTiltMinMax;
		rPosition.PositionType = positionType;

		// position = Vector2( centerPos.x + ( radX * Mathf.Sin( Mathf.Deg2Rad * alpha ) ),
		// 					   centerPos.y + ( radY * Mathf.Cos( Mathf.Deg2Rad * alpha ) ) );
		   
		// alpha += 5f;//can be used as speed

		// position = Vector2( centerPos.x + ( radX * MCos( alpha ) * MCos( tilt ) ) - ( radY * MSin( alpha ) * MSin( tilt ) ),
		// 					   centerPos.y + ( radX * MCos( alpha ) * MSin( tilt ) ) + ( radY * MSin( alpha ) * MCos( tilt ) ) );
		// alpha += 5f;
	}

	void Generate()
	{
		//// Ellipse
		//grV2f v;
		//float radX{ 50.0f };
		//float radY{ 25.0f };

		//// Circular path pos
		//rPosition.EllipseStepCount += 4.0f * rEmit.Dt;
		//if( rPosition.EllipseStepCount > rPosition.Ellipse360 )
		//	rPosition.EllipseStepCount = 0.0f;

		//// Random circulr pos
		////auto distAlpha{ rPosition.Rand.DistF( 0.0f, rPosition.Ellipse360 ) };
		////rPosition.EllipseStepCount = rPosition.Rand.Float( distAlpha );

		//// Tilt
		//rPosition.EllipseTiltCount += grMath::DegToRad( 50.0f * rEmit.Dt );
		//if( rPosition.EllipseTiltCount > 360.0f )
		//	rPosition.EllipseTiltCount = 0.0f;
		//if( rPosition.EllipseTiltCount < 0.0f )
		//	rPosition.EllipseTiltCount = 360.0f;



		//// Regular
		//v.x = rEmit.SystemPosition.x + ( radX * std::sinf( rPosition.EllipseStepCount ) );
		//v.y = rEmit.SystemPosition.y + ( radY * std::cosf( rPosition.EllipseStepCount ) );

		//// Tilt
		////v.x = rEmit.SystemPosition.x + ( radX * std::cosf( rPosition.EllipseStepCount ) * std::cosf( rPosition.EllipseTiltCount ) ) -
		////	( radY * std::sinf( rPosition.EllipseStepCount ) * std::sinf( rPosition.EllipseTiltCount ) );

		////v.y = rEmit.SystemPosition.y + ( radX * std::cosf( rPosition.EllipseStepCount ) * std::sinf( rPosition.EllipseTiltCount ) ) +
		////	( radY * std::sinf( rPosition.EllipseStepCount ) * std::cosf( rPosition.EllipseTiltCount ) );

		//for ( sizeT i = rEmit.StartIdx; i < rEmit.EndIdx; ++i )
		//	rArray.Position[ i ] = v;



		// Box
		sizeT startIdx{ rEmit.StartIdx }, endIdx{ rEmit.EndIdx };
		if ( rPosition.PositionEqual == EEqualValue::NO )
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

	void Update()
	{
		float dt{ rEmit.Dt };
		for ( sizeT i = 0; i < rEmit.Alive; ++i )
			rArray.Position[ i ] += rArray.Velocity[ i ] * dt;
	}

	void PositionNotEqualBox()
	{
		for ( sizeT i = rEmit.StartIdx; i < rEmit.EndIdx; ++i )
		{
			grV2f v{ rPosition.Rand.Float( DistPosX ), rPosition.Rand.Float( DistPosY ) };
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

	void Init( const grV2f& rMinMax )
	{
		rLife.LifeMinMax = rMinMax;
		SetMinMax( rLife.LifeMinMax, rLife.LifeEqual );
	}

	void Generate()
	{
		sizeT startIdx{ rEmit.StartIdx }, endIdx{ rEmit.EndIdx };
		if ( rLife.LifeEqual == EEqualValue::NO )
		{
			auto dist{ rLife.Rand.DistF( rLife.LifeMinMax.x, rLife.LifeMinMax.y ) };
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rArray.Life[ i ] = rLife.Rand.Float( dist );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
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
		grAlgo::Swap( rArray.ColorStart[ nowIdx ], rArray.ColorStart[ last ] );
		grAlgo::Swap( rArray.ColorEnd[ nowIdx ], rArray.ColorEnd[ last ] );
		grAlgo::Swap( rArray.ScaleStart[ nowIdx ], rArray.ScaleStart[ last ] );
		grAlgo::Swap( rArray.ScaleEnd[ nowIdx ], rArray.ScaleEnd[ last ] );
		grAlgo::Swap( rArray.Mass[ nowIdx ], rArray.Mass[ last ] );
		grAlgo::Swap( rArray.Acceleration[ nowIdx ], rArray.Acceleration[ last ] );
		grAlgo::Swap( rArray.Velocity[ nowIdx ], rArray.Velocity[ last ] );
		grAlgo::Swap( rArray.Position[ nowIdx ], rArray.Position[ last ] );
		grAlgo::Swap( rArray.Life[ nowIdx ], rArray.Life[ last ] );

		// Most values are generated and set by operator= and does not need zeroing (all above)
		// Values that are generated and set by operator+= needs resetting obviously (all below)
		rArray.Acceleration[ last ] = { 0.0f, 0.0f };
	}
};


struct grSParticleSystem
{
	pU<grSColorSystem> puColor;
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
		puColor = std::make_unique<grSColorSystem>( rData );
		puScale = std::make_unique<grSScaleSystem>( rData );
		puMass = std::make_unique<grSMassSystem>( rData );
		puVelocity = std::make_unique<grSVelocitySystem>( rData );
		puPosition = std::make_unique<grSPositionSystem>( rData );
		puLife = std::make_unique<grSLifeSystem>( rData );
	}
};

#endif // _H_GRPARTICLESYSTEM_