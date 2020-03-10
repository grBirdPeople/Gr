#ifndef		_H_GRICTRLOBJ_
#define		_H_GRICTRLOBJ_

#include	"grDefine.h"


class grICtrlObj
{
public:

					grICtrlObj		(void) : m_bActiveStatus(true)	{}
					
			bool	GetActiveStatus	(void )							{ return m_bActiveStatus; }
			void	SetActiveStatus	(const bool bActive)			{ m_bActiveStatus = bActive; }
	
	virtual	void	Update			(void) = 0;

private:

	bool	m_bActiveStatus;

};

#endif		//_H_GRICTRLOBJ_
