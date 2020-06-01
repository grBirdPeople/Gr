#ifndef		_H_GRDEFINE_
#define		_H_GRDEFINE_

//////////////////////////////////////////////////

// NDEBUG is used here since it disables assert and follows that whole standard.
// There is also _DEBUG which is more of a MS standard if I read it right.
#ifndef NDEBUG
	#define		DEBUG
#endif

#define		DELANDNULL(x)	delete x, x = nullptr

//////////////////////////////////////////////////

#endif		//_H_GRDEFINE_
