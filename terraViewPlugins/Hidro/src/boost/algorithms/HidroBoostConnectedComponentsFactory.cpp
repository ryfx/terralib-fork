#include <HidroBoostConnectedComponentsFactory.h>
#include <HidroBoostConnectedComponents.h>

#include <TeException.h>


HidroBoostConnectedComponentsFactory::HidroBoostConnectedComponentsFactory()
: HidroBoostAlgorithmFactory(std::string("Connected Components"))
{
};      

HidroBoostConnectedComponentsFactory::~HidroBoostConnectedComponentsFactory()
{
};


HidroBoostAlgorithms* HidroBoostConnectedComponentsFactory::build (const HidroBoostParams& arg)
{
	HidroBoostAlgorithms* instancePtr = new HidroBoostConnectedComponents();

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
