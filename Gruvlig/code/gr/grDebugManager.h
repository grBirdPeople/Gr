#ifndef		_GRDEBUGMANAGER_H_
#define		_GRDEBUGMANAGER_H_

#include	<SFML/Graphics/RenderWindow.hpp>
#include	<SFML/Graphics/VertexArray.hpp>

#include	"grCommon.h"
#include	"grSingleton.h"

typedef		std::vector<sf::VertexArray>	vecVertexArray;

class		grBBox;


// grDebugManager
//////////////////////////////////////////////////
class grDebugManager : public grSingleton<grDebugManager>
{
public:
	
			void	AddBBox		( grBBox& rBBox, const sf::Color color );

	inline	void	DebugRender	( sf::RenderWindow& rRenderWin )
								{
									if (m_VecBBoxDebug.size() > 0)
									{
										for (auto& i : m_VecBBoxDebug)
										{
											rRenderWin.draw(i);
										}

										m_VecBBoxDebug.clear();
									}
								}
	
	//////////////////////////////////////////////////

private:

	vecVertexArray	m_VecBBoxDebug;

};

#endif		//_GRDEBUGMANAGER_H_
