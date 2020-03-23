#ifndef		_GRENTITYENEMY_H_
#define		_GRENTITYENEMY_H_

#include	"grIEntity.h"


// grEntityEnemy
//////////////////////////////////////////////////
class grEntityEnemy : public grIEntity
{
public:

			grEntityEnemy	( const grEnums::EntityType type, const str& rName, const grV2f& rPos, const uInt id );
			~grEntityEnemy	( void );
	
	//////////////////////////////////////////////////

	void	Update			( const float deltaT );
	
	//////////////////////////////////////////////////

private:


};

#endif		//_GRENTITYENEMY_H_
