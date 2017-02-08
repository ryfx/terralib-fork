#ifndef HIDRO_BOOST_UTILS_H_
#define HIDRO_BOOST_UTILS_H_

#include <string>
#include <vector>

class HidroGraph;
class HidroVertex;
class HidroEdge;

std::vector<std::string> getBoostAlgorithmsNames();

bool createGraphComponent(HidroGraph* inputGraph, HidroGraph* outputGraph, const int& startVertexIdx, const int& endVertexIdx, int& idx);

bool copyVertexAttributes(HidroVertex* input, HidroVertex* output);

bool copyEdgeAttributes(HidroEdge* input, HidroEdge* output);

#endif // HIDRO_BOOST_UTILS_H_