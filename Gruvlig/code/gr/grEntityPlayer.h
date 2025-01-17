#ifndef		_GRENTITYPLAYER_H_
#define		_GRENTITYPLAYER_H_

#include	"grIEntity.h"

class		grCtrlPlayer;


// grEntityPlayer
//////////////////////////////////////////////////
class grEntityPlayer : public grIEntity
{
public:

			grEntityPlayer	( const grEnums::EntityType type, const str& rName, const grV2f& rPos, const intU id );
			~grEntityPlayer	( void );

	//////////////////////////////////////////////////

	void	Update			( const float deltaT );
	
	//////////////////////////////////////////////////

private:

	float				m_SpdWalk = 300.0f;

	grCtrlPlayer*		m_pCtrl;
};

#endif		//_GRENTITYPLAYER_H_