//#include	<random>
//
//#include	"grAlgo.h"
//#include	"grCommon.h"
#include	"grCore.h"
//#include	"grNavNode.h"
//#include	"grV2.h"


int main()
{
	//const uInt arrSize = 10;
	//uInt arrNums[arrSize];

	//std::random_device rd;
	//
	//std::mt19937 engine( rd() );
	//std::uniform_int_distribution< uInt > rng( 0, 50 );
	//
	//for( uInt i = 0; i < arrSize; ++i )
	//	arrNums[ i ] = rng( engine );
	//	
	//	
	//for( uInt i = 0; i < arrSize; ++i )
	//	std::cout << arrNums[ i ] << '\n';
	//	
	//std::cout << "\n\n";
	//	
	//	
	//grAlgo::InsertionSort< uInt >( arrNums, arrSize, 1 );
	//	
	//	
	//for( uInt i = 0; i < arrSize; ++i )
	//	std::cout << arrNums[ i ] << '\n';

	//////////////////////////////////////////////////

	grCore& core = grCore::Initialize();
	core.Init();
	core.Run();
	core.DeInitialize();

}
