#include <HidroBoostBidirectionGraphFactory.h>
#include <HidroBoostBidirectionGraph.h>

#include <TeException.h>


HidroBoostBidirectionalGraphFactory::HidroBoostBidirectionalGraphFactory()
: HidroBoostAlgorithmFactory(std::string("Bidirectional Graph"))
{
};      

HidroBoostBidirectionalGraphFactory::~HidroBoostBidirectionalGraphFactory()
{
};


HidroBoostAlgorithms* HidroBoostBidirectionalGraphFactory::build (const HidroBoostParams& arg)
{
	HidroBoostAlgorithms* instancePtr = new HidroBoostBidirectionalGraph();

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
