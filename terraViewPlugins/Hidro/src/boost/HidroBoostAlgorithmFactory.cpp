#include <HidroBoostAlgorithmFactory.h>

HidroBoostAlgorithmFactory::HidroBoostAlgorithmFactory( const std::string& factoryName )
: TeFactory< HidroBoostAlgorithms, HidroBoostParams >( factoryName )
{
};      

HidroBoostAlgorithmFactory::~HidroBoostAlgorithmFactory()
{
};
