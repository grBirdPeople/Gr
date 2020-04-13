#ifndef		_H_GRPARTICLEMANAGERPB_
#define		_H_GRPARTICLEMANAGERPB_

#define		PARTICLE_SYTEMS			1				// If threads would happen
#define		PARTICLE_ATTRIBUTES		5
#define		PARTICLE_PER_ATTRIBUTE	10
#define		PARTICLE_TIMESTEP		1.0f / 60.0f	// If physics update for particles

#include	"grSingleton.h"

#include	"grParticleSystemPB.h"
#include	"grParticlePB.h"

using		arrParticle = grParticlePB*[ PARTICLE_ATTRIBUTES * PARTICLE_PER_ATTRIBUTE ];


// Dynamically aligned loop que, which is broken
template<typename T>
class grDALQue
{
public:

	grDALQue( const uInt size )
		: Size		( size )
		, FirstIdx	( 0 )
		, Quantity	( 0 )
	{
		arrT = new T[ Size ];
		memset( arrT, 0, Size );
	}

	~grDALQue( void )
	{
		delete[] arrT;
	}

	grDALQue( grDALQue const& ) = delete;
	grDALQue& operator=( grDALQue const& ) = delete;

	const uInt GetQuantity( void ) const
	{
		return Quantity;
	}

	void Push( const T& rT )
	{
		if ( Quantity == Size )
		{
#ifdef DEBUG
			std::puts( "DALQue::Push(): Size maxed out. No element was added.\n" );
#endif // DEBUG
			return;
		}

		arrT[ FirstIdx + Quantity ] = rT;
		++Quantity;
	}

	T Pull( void )
	{
		assert( ( Quantity > 0 ) && "DALQue::Pull(): Que is empty" );

		--Quantity;
		T firstIdx = arrT[ FirstIdx ];

		if ( ++firstIdx == Size )
			firstIdx = 0;

		return firstIdx;
	}

private:

	T* arrT;

	sizeT	Size,
		FirstIdx,
		Quantity;
};


// grParticleManagerPB
//////////////////////////////////////////////////
class grParticleManagerPB : public grSingleton<grParticleManagerPB>
{
public:

	grParticleManagerPB( void );
	~grParticleManagerPB( void );

	//////////////////////////////////////////////////

	//inline grParticleAttributePB CreateParticleAttribute( void );
	grParticleAttributePB* const CreateParticleSystem( void );
	void Update( const float deltaT );

	//////////////////////////////////////////////////

private:
	
	struct AttPartCombo
	{
		AttPartCombo( const uInt id, const sizeT size )
			: uPtrArrParticles	( new grParticlePB*[ size ]() )
			, uPtrAttribute		( new grParticleAttributePB() )
			, Id				( id )
			, ActiveParicles	( 0 )
			, Size				( size )
		{
			for ( sizeT i = 0; i < Size; ++i )
				uPtrArrParticles[ i ] = new grParticlePB();

			uPtrAttribute = std::make_unique<grParticleAttributePB>();
			uPtrAttribute->Id = Id;
		}
		~AttPartCombo( void )
		{
			for ( sizeT i = 0; i < Size; ++i )
			{
				if ( uPtrArrParticles[ i ] != nullptr )
					DELANDNULL( uPtrArrParticles[ i ] );
			}
			uPtrArrParticles.release();

			if ( uPtrAttribute != nullptr )
				uPtrAttribute.release();
		}
		AttPartCombo( AttPartCombo const& ) = delete;
		AttPartCombo& operator=( AttPartCombo  const& ) = delete;

		//////////////////////////////////////////////////

		uPtr<grParticlePB*[]>		uPtrArrParticles;
		uPtr<grParticleAttributePB>	uPtrAttribute;

		uInt	Id,
				ActiveParicles;

		sizeT	Size;
	};

	//////////////////////////////////////////////////

	uPtr<uPtr<AttPartCombo>[]>			m_uPtrArrAttPartCombo;
	uPtr<uPtr<grParticleSystemPB>[]>	m_uPtrSystems;

	uInt				m_CreatedSystems,
						m_CreatedArrCombo,
						m_TotalParticles;

	grDALQue<uInt>*		m_Que;

};

#endif	// _H_GRPARTICLEMANAGERPB_