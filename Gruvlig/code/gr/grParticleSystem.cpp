#include	"grParticleSystem.h"

#include	"grParticleEmitter.h"
#include	"grSPartAttribute.h"
#include	"grParticle.h"
#include	"grMath.h"
#include	"grRandom.h"


grCParticleSystem::grCParticleSystem( const uInt id, const sizeT partMax )
	: m_uPRand	( new grRandom() )
	, m_Id		( id )
	, m_PartMax	( partMax )
{}


grCParticleSystem::~grCParticleSystem( void )
{
	if ( m_uPRand != nullptr )
		m_uPRand.reset();
}


void
grCParticleSystem::Copy( vE<uInt>& rVeActvEmitr,
						 uP<uP<grCParticleEmitter>[]>& rArEmitr,
						 uP<uP<grSParticleAttribute>[]>& rArAtt )
{
	if ( rVeActvEmitr.size() > 0 )
	{
		sizeT actvEmitrSize = rVeActvEmitr.size();
		for ( sizeT i = 0; i < actvEmitrSize; ++i )
		{
			if ( rArEmitr[ i ]->m_UsrMods.any() == true )
			{
				vE<sizeT> usrMods;
				usrMods.reserve( ( sizeT )grCParticleEmitter::EUsrMods::SIZE );
				float x = 0.0f, y = 0.0f;
				uInt id = rVeActvEmitr[ i ];
				grSParticleAttribute& rEmitrAtt = *rArEmitr[ id ]->m_uPAtt.get();
				grSParticleAttribute& rSysAtt = *rArAtt[ id ].get();
				usrMods = rArEmitr[ id ]->GetUsrModsSortd();
				for ( sizeT i = 0; i < usrMods.size(); ++i )
				{
					switch ( usrMods[ i ] )
					{
						case ( sizeT )grCParticleEmitter::EUsrMods::POS:
						rSysAtt.EmitrPos = rEmitrAtt.EmitrPos;
						rSysAtt.PartRadiusPosOffset = rEmitrAtt.PartRadiusPosOffset;
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::DIR_EMITR:
						rSysAtt.EmitrRotInDeg = grMath::Clamp( rEmitrAtt.EmitrRotInDeg, 0.0f, 359.9f );
						grMath::RotatePoint( &rSysAtt.EmitrDir, rSysAtt.EmitrRotInDeg * grMath::DegToRad );
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::DIR_PART:
						rSysAtt.PartRotInDegMinMax.x = grMath::Clamp( rEmitrAtt.PartRotInDegMinMax.x, 0.0f, 359.9f );
						rSysAtt.PartRotInDegMinMax.y = grMath::Clamp( rEmitrAtt.PartRotInDegMinMax.y, 0.0f, 359.9f );
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::SPD:
						rSysAtt.PartSpdMinMax = grMath::Clamp( rEmitrAtt.PartSpdMinMax.x, rEmitrAtt.PartSpdMinMax.x, rEmitrAtt.PartSpdMinMax.y );
						rSysAtt.PartSpdMod = rEmitrAtt.PartSpdMod;
						rSysAtt.PartSpdOsc = rEmitrAtt.PartSpdOsc;
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::LIFE:
						x = grMath::Clamp( rEmitrAtt.PartLifeMinMax.x, rEmitrAtt.PartLifeMinMax.x, rEmitrAtt.PartLifeMinMax.y );
						y = grMath::Clamp( rEmitrAtt.PartLifeMinMax.y, rEmitrAtt.PartLifeMinMax.x, rEmitrAtt.PartLifeMinMax.y );
						rSysAtt.PartLifeMinMax.x = grMath::Clamp( rEmitrAtt.PartLifeMinMax.x, 0.0f, x );
						rSysAtt.PartLifeMinMax.y = grMath::Clamp( rEmitrAtt.PartLifeMinMax.y, 0.0f, y );
						break;

						default:
						break;
					}
				}
			}
		}
	}
}


void
grCParticleSystem::Spawn( uP<uP<grCParticleEmitter>[]>& rArEmitr,
									  vE<uInt>& rVeActiveEmitr,
									  vE<uInt>& rVeActivateQue,
									  const float deltaT )
{
	if ( rVeActiveEmitr.size() > 0 )
	{
		sizeT actvEmitrSize = rVeActiveEmitr.size();
		for ( sizeT i = 0; i < actvEmitrSize; ++i )
		{
			grCParticleEmitter& rEmitr = *rArEmitr[ rVeActiveEmitr[ i ] ].get();
			rEmitr.m_SpawnTimer -= deltaT;
			if ( rEmitr.m_SpawnTimer < 0.0f )
			{
				rEmitr.m_SpawnTimer += rEmitr.m_SpawnRate;
				if( rEmitr.m_PartActive < m_PartMax )
					rVeActivateQue.push_back( rEmitr.m_Id );
			}
		}
	}
}


void
grCParticleSystem::Activate( vE<uInt>& rVeActivateQue,
							 uP<uP<grCParticleEmitter>[]>& rArEmitr,
							 uP<uP<grSParticleAttribute>[]>& rArAtt,
							 grSParticle** pAr2DPart )
{
	if ( rVeActivateQue.size() > 0 )
	{
		sizeT queSize = rVeActivateQue.size();
		for ( sizeT i = 0; i < queSize; ++i )
		{
			grCParticleEmitter& rEmitr = *rArEmitr[ rVeActivateQue[ i ] ].get();
			grSParticleAttribute& rAtt = *rArAtt[ rEmitr.m_Id ].get();
			grSParticle& rPart = pAr2DPart[ rEmitr.m_Id ][ rEmitr.m_PartActive ];

			ActvPosition( rAtt, rPart );
			ActvVelocity( rAtt, rPart );
			ActvLife( rAtt, rPart );

			++rEmitr.m_PartActive;
		}

		rVeActivateQue.clear();
	}
}


void
grCParticleSystem::Update( vE<std::pair<uInt, uInt>>& rVeDeactivateQue,
						   vE<uInt>& rVeActiveEmitr,
						   uP<uP<grCParticleEmitter>[]>& rArEmitr,
						   uP<uP<grSParticleAttribute>[]>& rArAtt,
						   grSParticle** pAr2DPart,
						   const float deltaT )
{
	sizeT actvEmitrSize = rVeActiveEmitr.size();
	for ( sizeT emitrIdx = 0; emitrIdx < actvEmitrSize; ++emitrIdx )
	{
		sizeT emitrId = rVeActiveEmitr[ emitrIdx ];
		sizeT partSize = rArEmitr[ emitrId ]->m_PartActive;
		for ( sizeT partIdx = 0; partIdx < partSize; ++partIdx )
		{
			grSParticle& rPart = pAr2DPart[ emitrId ][ partIdx ];

			// Speed
			rPart.Spd = grMath::Lerp( rPart.Spd, rPart.Spd - rPart.SpdMod, 0.5f );
			grV2f SpdAccel = rPart.Direction * rPart.Spd;

			//if( rPart.Spd > 0.0f )
			//	rPart.Spd = grMath::Lerp( rPart.Spd, rPart.Spd * rPart.SpdOsc, 0.5f );


			//grV2f grav = grV2f( 0.0f, 75.0f );
			
			rPart.Position += ( SpdAccel ) * deltaT;

			rPart.Lifetime -= deltaT;
			if ( rPart.Lifetime < 0.0f )
				rVeDeactivateQue.push_back( std::pair<uInt, uInt>( emitrId, partIdx ) );
		}
	}
}


void grCParticleSystem::Deactivate( vE<std::pair<uInt, uInt>>& rVeDeactivateQue,
									uP<uP<grCParticleEmitter>[]>& rArEmitr,
									grSParticle** pAr2DPart )
{
	if ( rVeDeactivateQue.size() > 0 )
	{
		sizeT queSize = rVeDeactivateQue.size();
		for ( sizeT idx = 0; idx < queSize; ++idx )
		{
			uInt emitrId = rVeDeactivateQue[ idx ].first;
			uInt partIdx = rVeDeactivateQue[ idx ].second;
			sizeT partsActive = rArEmitr[ emitrId ]->m_PartActive;

			--rArEmitr[ emitrId ]->m_PartActive;

			grSParticle& rTooPart = pAr2DPart[ emitrId ][ partIdx ];
			grSParticle& rFromPart = pAr2DPart[ emitrId ][ partsActive - 1 ];
			rTooPart = rFromPart;
		}

		rVeDeactivateQue.clear();
	}
}


void
grCParticleSystem::ActvPosition( grSParticleAttribute& rAtt, grSParticle& rPart )
{
	rPart.Position = rAtt.EmitrPos;
	if ( grMath::CmpFloat( rAtt.PartRadiusPosOffset, 0.0f ) != true )
	{
		grV2f dir = grV2f( m_uPRand->Float( -1.0f, 1.0f ), m_uPRand->Float( -1.0f, 1.0f ) );
		float dist = m_uPRand->Float( 0.0f, rAtt.PartRadiusPosOffset );
		float rad = m_uPRand->Float( 0.0f, 359.9f ) * grMath::DegToRad;

		grMath::RotatePoint( &dir, rad );
		rPart.Position += dir * dist;
	}
}


void
grCParticleSystem::ActvVelocity( grSParticleAttribute& rAtt, grSParticle& rPart )
{
	// Direction
	float deg = 0.0f;
	if ( grMath::CmpFloat( rAtt.PartRotInDegMinMax.x, rAtt.PartRotInDegMinMax.y ) != true )
	{
		if ( rAtt.PartRotInDegMinMax.x > rAtt.PartRotInDegMinMax.y )	
		{
			float diff = ( 360.0f - rAtt.PartRotInDegMinMax.x );
			deg = m_uPRand->Float( 0.0f, diff + rAtt.PartRotInDegMinMax.y );
			deg -= diff;
			if ( 0.0f > deg )
				deg = 360.0f + deg;
		}
		else
		{
			deg = m_uPRand->Float( rAtt.PartRotInDegMinMax.x, rAtt.PartRotInDegMinMax.y );
		}
	}
	else
	{
		deg = rAtt.PartRotInDegMinMax.x;
	}

	deg += rAtt.EmitrRotInDeg;
	rPart.Direction = rAtt.EmitrDir;
	grMath::RotatePoint( &rPart.Direction, deg * grMath::DegToRad );

	// Speed
	if ( rAtt.PartSpdMinMax.x > rAtt.PartSpdMinMax.y )
		rAtt.PartSpdMinMax.x = rAtt.PartSpdMinMax.y;

	rAtt.PartSpdMinMax.x = grMath::Clamp( rAtt.PartSpdMinMax.x, rAtt.PartSpdMinMax.x, rAtt.PartSpdMinMax.y );

	rPart.Spd = ( grMath::CmpFloat( rAtt.PartSpdMinMax.x, rAtt.PartSpdMinMax.y ) != true )
		? m_uPRand->Float( rAtt.PartSpdMinMax.x, rAtt.PartSpdMinMax.y )
		: rAtt.PartSpdMinMax.x;

	// This combined with scale could work
	//float weight = m_uPRand->Float( 1.0f, 1.5f );
	//rPart.Spd /= weight;

	rPart.SpdMod = rAtt.PartSpdMod;

	rPart.SpdOsc = rAtt.PartSpdOsc;




	//grV2f dir = grV2f( 0.0f, -1.0f );	
	//grMath::RotatePoint( &dir, deg * grMath::DegToRad );

	//float speed = ( grMath::CmpFloat( rAtt.MinMaxSpeed.x, rAtt.MinMaxSpeed.y ) != true )
	//	? m_uPRand->Float( rAtt.MinMaxSpeed.x, rAtt.MinMaxSpeed.y )
	//	: rAtt.MinMaxSpeed.x;

	//rPart.Velocity = dir * speed;
}


void
grCParticleSystem::ActvLife( grSParticleAttribute& rAtt, grSParticle& rPart )
{
	rPart.Lifetime = ( grMath::CmpFloat( rAtt.PartLifeMinMax.x, rAtt.PartLifeMinMax.y ) != true )
		? m_uPRand->Float( rAtt.PartLifeMinMax.x, rAtt.PartLifeMinMax.y )
		: rAtt.PartLifeMinMax.x;
}
