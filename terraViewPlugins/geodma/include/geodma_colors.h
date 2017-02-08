#ifndef __GEODMA_COLORS_H_
#define __GEODMA_COLORS_H_

#include <sstream>
#include <vector>

using namespace std;

class geodma_colors
{
  private:
    vector<int> R, G, B;
    
  public:
    
    /* Constructor and destructor */
    geodma_colors();
    ~geodma_colors();

    unsigned get_size();
    int get_R(unsigned i);
    int get_G(unsigned i);
    int get_B(unsigned i);
};

#endif //__GEODMA_COLORS_H_