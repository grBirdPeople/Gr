#ifndef		_H_GRPARTICLE_SETUPPB_
#define		_H_GRPARTICLE_SETUPPB_

class		grParticleAttributePB;


// grParticleSetupPB
//////////////////////////////////////////////////
struct grParticleSetupPB
{
	grParticleSetupPB( void )
		//: Id				( id )
		//, ParticlesActive	( 0 )
	{
		//for ( sizeT i = 0; i < PARTICLE_PER_SETUP; ++i )
		//	arrParticle[ i ] = nullptr;

		//pAttribute = std::make_unique<grParticleAttributePB>();
	}

	~grParticleSetupPB( void )
	{
		//pAttribute.reset();

		//for ( sInt i = 0; i < PARTICLE_PER_SETUP; ++i )
		//{
		//	if ( arrParticle[ i ] != nullptr )
		//		arrParticle[ i ].reset();
		//}
	}

	grParticleSetupPB( grParticleSetupPB const& ) = delete;
	grParticleSetupPB& operator=( grParticleSetupPB const& ) = delete;

	//////////////////////////////////////////////////

	//void Activate( void )
	//{
	//	//grParticleManagerPB& rPartMan = grParticleManagerPB::Instance();
	//	//rParticleManPB.Activate( Id );
	//}
	//void Deactivate( void )
	//{
	//	//rParticleManPB.Deactivate( Id );
	//}
	void SetParticleAttribute( const grParticleAttributePB& rAttribute )
	{
		//pAttribute->Position = rAttribute.Position;
		//pAttribute->Velocity = rAttribute.Velocity;
		//pAttribute->Acceleration = rAttribute.Acceleration;
		//pAttribute->Gravity = rAttribute.Gravity;
		//pAttribute->Mass = rAttribute.Mass;
		//pAttribute->SpawnCounter = rAttribute.SpawnCounter;
		//pAttribute->SpawnInMilliSec = rAttribute.SpawnInMilliSec;

		//// Range values
		//pAttribute->SpeedRange = rAttribute.SpeedRange;
		//pAttribute->DirectionRange = rAttribute.DirectionRange;
		//pAttribute->LifetimeRange = rAttribute.LifetimeRange;

		//// Osc values
		//pAttribute->SpeedOsc = rAttribute.SpeedOsc;

		//// Range flags
		//pAttribute->bSpeedRange = rAttribute.bSpeedRange;
		//pAttribute->bDirectionRange = rAttribute.bDirectionRange;
		//pAttribute->bLifetimeRange = rAttribute.bLifetimeRange;

		//// Osc flags
		//pAttribute->bSpeedOsc = rAttribute.bSpeedOsc;
	}

	//////////////////////////////////////////////////

	//arrParticle					arrParticle;
	//uPtr<grParticleAttributePB>	pAttribute;

	//uInt						Id,
	//							ParticlesActive;
};

#endif	// _H_GRPARTICLE_SETUPPB_