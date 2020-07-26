#include	"grDefine.h"
#include	"grCore.h"


int main()
{
	grCore& core = grCore::Initialize();
	core.Init();
	core.Run();
	core.DeInitialize();
}
