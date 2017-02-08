#include <HidroBoostDijkstraFactory.h>
#include <HidroBoostDijkstra.h>

#include <TeException.h>


HidroBoostDijkstraFactory::HidroBoostDijkstraFactory()
: HidroBoostAlgorithmFactory(std::string("Dijkstra Shortest Path"))
{
};      

HidroBoostDijkstraFactory::~HidroBoostDijkstraFactory()
{
};


HidroBoostAlgorithms* HidroBoostDijkstraFactory::build (const HidroBoostParams& arg)
{
	HidroBoostAlgorithms* instancePtr = new HidroBoostDijkstra();

	if( ! instancePtr )
	{
		throw TeException( FACTORY_PRODUCT_INSTATIATION_ERROR );
	}

	if(!instancePtr->reset(arg))
	{
		throw TeException( UNKNOWN_ERROR_TYPE );
	}

	return instancePtr;
}
