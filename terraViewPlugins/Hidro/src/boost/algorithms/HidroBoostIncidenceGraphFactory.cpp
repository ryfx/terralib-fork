#include <HidroBoostBidirectionGraphFactory.h>
#include <HidroBoostIncidenceGraphFactory.h>
#include <HidroBoostIncidenceGraph.h>

#include <TeException.h>


HidroBoostIncidenceGraphFactory::HidroBoostIncidenceGraphFactory()
: HidroBoostAlgorithmFactory(std::string("Incidence Graph"))
{
};      

HidroBoostIncidenceGraphFactory::~HidroBoostIncidenceGraphFactory()
{
};


HidroBoostAlgorithms* HidroBoostIncidenceGraphFactory::build (const HidroBoostParams& arg)
{
	HidroBoostAlgorithms* instancePtr = new HidroBoostIncidenceGraph();

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
