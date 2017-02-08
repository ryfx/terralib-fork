#include <HidroBoostStronglyConnectedComponentsFactory.h>
#include <HidroBoostStronglyConnectedComponents.h>

#include <TeException.h>


HidroBoostStronglyConnectedComponentsFactory::HidroBoostStronglyConnectedComponentsFactory()
: HidroBoostAlgorithmFactory(std::string("Strongly Connected Components"))
{
};      

HidroBoostStronglyConnectedComponentsFactory::~HidroBoostStronglyConnectedComponentsFactory()
{
};


HidroBoostAlgorithms* HidroBoostStronglyConnectedComponentsFactory::build (const HidroBoostParams& arg)
{
	HidroBoostAlgorithms* instancePtr = new HidroBoostStronglyConnectedComponents();

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
