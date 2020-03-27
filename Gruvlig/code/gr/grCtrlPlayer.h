#ifndef		_H_GRCTRLPLAYER_
#define		_H_GRCTRLPLAYER_

#include	"grCommon.h"
#include	"grICtrlObj.h"
#include	"grV2.h"

class		grIEntity;
class		grInputManager;


class grCtrlPlayer : public grICtrlObj
{
public:

	grCtrlPlayer( grIEntity* pEntity );
	~grCtrlPlayer( void )
	{
		if ( m_pEntityOwner )
		{
			m_pEntityOwner = nullptr; }
	}

	grV2f& GetInputDir( void )
	{
		return m_InputDir;
	}
	virtual void Update( void );

private:

	grV2f		m_InputDir;

	grIEntity*	m_pEntityOwner;
	
	grInputManager& m_rInputMan;

};

#endif		//_H_GRCTRLPLAYER_
