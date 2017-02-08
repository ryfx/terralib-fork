#include <HidroBoostParams.h>


HidroBoostParams::HidroBoostParams()
{
  reset();  
}


HidroBoostParams::HidroBoostParams( const HidroBoostParams& external )
{
  operator=( external );
}


HidroBoostParams::~HidroBoostParams()
{
  reset();
}


const HidroBoostParams& HidroBoostParams::operator=(const HidroBoostParams& external )
{
	/* General parameters */
	_boostAlgorithmName		= external._boostAlgorithmName;

	_inputHidroGraph		= external._inputHidroGraph;
	_outputHidroGraph		= external._outputHidroGraph;

	_edgeCostAttribute		= external._edgeCostAttribute;

	_startVertexId			= external._startVertexId;
	_endVertexId			= external._endVertexId;

	_returnIntValue			= external._returnIntValue;

	return external;
}


std::string HidroBoostParams::decName() const
{
	return _boostAlgorithmName;
}


void HidroBoostParams::reset()
{
	/* general parameters */
	_boostAlgorithmName = "";
	_edgeCostAttribute	= "";

	_startVertexId		= "";
	_endVertexId		= "";

	_returnIntValue		= -1;

	_inputHidroGraph	= NULL;
	_outputHidroGraph	= NULL;

}