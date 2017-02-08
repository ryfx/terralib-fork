#ifndef HIDRO_BOOST_BIDIRECTIONALGRAPH_H
#define HIDRO_BOOST_BIDIRECTIONALGRAPH_H

#include <HidroBoostAlgorithms.h>

class HidroBoostBidirectionalGraph : public HidroBoostAlgorithms
{
public:
	/**
	* @brief Default Constructor.
	*/
	HidroBoostBidirectionalGraph();

	/**
	* @brief Default Destructor
	*/
	~HidroBoostBidirectionalGraph();

	/**
	* @brief Function used to get description from current algorithm selected
	*
	*
	* @return String with BGL algorithm description.
	*/
	virtual std::string getBGLalgorithmDescription();

protected:

	/**
	* @brief Function used to check internal parameters..
	*
	*
	* @return True if parameters is correctly and false in other case.
	*/
	virtual bool checkInternalParameters();

	/**
	* @brief Function used to measure objects, pure virtual function implementation.
	*
	*
	* @return True if the measure is correctly and false in other case.
	*/
	virtual bool execute();
};

#endif //HIDRO_BOOST_BIDIRECTIONALGRAPH_H
