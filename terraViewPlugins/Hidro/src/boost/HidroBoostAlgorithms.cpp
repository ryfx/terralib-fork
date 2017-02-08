#include <HidroBoostAlgorithms.h>

HidroBoostAlgorithms::HidroBoostAlgorithms(): _errorMessage("")
{
}

HidroBoostAlgorithms::~HidroBoostAlgorithms()
{

}

void HidroBoostAlgorithms::getParameters( HidroBoostParams& params ) const
{
	params = _internalParams;
}


bool HidroBoostAlgorithms::reset( const HidroBoostParams& newparams )
{
	_internalParams = newparams;

	return true;
}

bool HidroBoostAlgorithms::executeAlgorithm()
{
	if(!checkParameters())
	{
		return false;
	}

	if(!execute())
	{
		return false;
	}

	return true;
}

bool HidroBoostAlgorithms::checkParameters()
{
	_errorMessage = "";

//any strategy check internal parameters
	if(!checkInternalParameters())
	{
		return false;
	}

	return true;
}
