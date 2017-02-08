#ifndef HIDRO_FIX_PFS_H_
#define HIDRO_FIX_PFS_H_

// Hidro template classes
#include <HidroMatrix.hpp>
#include <HidroSmartQueue.h>

// foreigner classes
class TeDatabase;
class TeRasterParams;

class BreachingNode
{
public:
  unsigned int x_;
  unsigned int y_;
  double altimetria_;
};

/**
 * @class HidroFixPFS
 * @brief This class fix the pits of an LDD using the PFS (Priority Fisrst Serarch) algorithm (Sedgewick, 1988).
 * @author Alexandre Copertino Jardim <alexcj@dpi.inpe.br>
 * @ingroup hidroFlow The Group for generate flow functions.
**/
class HidroFixPFS
{
public:  
  /**
   * @brief Main Constructor.
   * @param database Database pointer.
   * @param rasterParams DEM parameters.
   * @param demMatrix DEM.
   * @param lddMatrix LDD.
   * @param lddDirectionsMap LDD convention.
  **/
  HidroFixPFS( TeDatabase* database,
    TeRasterParams rasterParams,
    HidroMatrix<double>& demMatrix,
    HidroMatrix<unsigned char>& lddMatrix,
    unsigned char lddDirectionsMap[8],
    std::string demName,
    std::string lddName,
    int pitMaxTreatment,
    double boost );

  ~HidroFixPFS();

  /**
   * @brief Run the PFS (Priority Fisrst Serarch) algorithm to fix the pits (Sedgewick, 1988).
   * @return True for normal execution.
  **/
  bool pfs();  

  void fillPlaneAreas();

  void fillPlaneAreas2();

  void solveSimplePits();

protected:
  /**
   * @brief Initialize the pit queue.
   * @return True for normal execution.
  **/
  bool initPitQueue();
  
  /**
   * @brief Initialize the pit queue ordered by altimetria 3x3.
   * @return True for normal execution.
  **/
  bool initPitQueueOrdered();

  /**
   * @brief Compute the mean altimetria of a window of 3x3, 5x5, 7x7, etc. centred in x, y;
   * @param x Window center column.
   * @param y Window center line.
   * @param windowSize Window size (3x3, 5x5, 7x7, ...).
   * @param includeCenter Include the center cell in the calculus.
   * @return The mean.
  **/
  double meanNeigbhors( unsigned int x, unsigned int y, int windowSize, bool includeCenter );

  /**
   * @brief Verify how many neighbors are pits.
   * @param x Column of the cell.
   * @param y Line of the cell.
   * @return Number of neighbors that are pits.
  **/
  int pitsNeighbors( unsigned int x, unsigned int y );

  /**
   * @brief Recompute the LDD for Neighborhood.
   * @param x Column of the cell.
   * @param y Line of the cell.
  **/
  void recomputeNeighborhoodLDD( unsigned int x, unsigned int y );

  /**
   * @brief Print final results in txt arquive.   
  **/
  void print();

  /**
   * @brief Check if is boundary region of fix method ( -4 lines and columns of DEM ).
   * @param x Column of the cell.
   * @param y Line of the cell.
   * @return True if is boundary region.
  **/
  bool boundaryFixRegion( unsigned int x, unsigned int y );

  bool boundaryRegion( unsigned int x, unsigned int y );

  /**
   * @brief Verify if it is a plane area (all the neighboors has the same altimetria).
   * @param x Column of the cell.
   * @param y Line of the cell.
   * @return True if is plane are.
  **/
  bool isPlane( unsigned int x, unsigned int y );

  /**
   * @brief Verify if it is a plane area (all the neighboors has the same altimetria).
   * @param x Column of the cell.
   * @param y Line of the cell.
   * @return True if is plane are.
  **/
  bool isPlaneBorder( unsigned int x, unsigned int y );

  /**
   * @brief Verify how many neighbors are the same altimetria that center cell (include center cell).
   * @param x Column of the cell.
   * @param y Line of the cell.
   * @return Number of neighbors that are pits.
  **/
  int sameAlimetriaNeighboors( unsigned int x, unsigned int y );

private:
  // -- Members.
  TeDatabase* database_;
  TeRasterParams rasterParams_;
  HidroMatrix<double>& demMatrix_;
  HidroMatrix<unsigned char>& lddMatrix_;
  unsigned char* lddDirectionsMap_;

  // -- Fix parameters
  unsigned int pitMaxTreatment_;
  double boost_;

  // -- Raster limits.
  unsigned int rasterLines_;
  unsigned int rasterColumns_;

  // -- Caving Plane Areas.
  double caving_increment_;
  double double_problem_increment_;

  // -- Main Pit Queue.
  HidroSmartQueue pitQueue_;
  
  // -- Queue controllers and final information.
  unsigned int initialSize_;  
  unsigned int solvedPitsNumber_;
  unsigned int addPitsNumber_;
  unsigned int unsolvedPitsNumber_;
  unsigned int simplePits_;

  // -- DEM and LDD output name
  std::string demName_;
  std::string lddName_;
  std::string queueStatisticsFileName_;
  std::string pitCorrectionStatisticsFileName_;

  // -- Time
  std::string starTime_;
  std::string endTime_;
  std::string partialTime_;

  // -- Estatisticas
  unsigned int double_problem_;
  unsigned int pits_in_path_;

  double path_length_mean_;
  double visited_number_mean_;
  unsigned int pits_solved_;
  unsigned int pits_solved_order_;
  unsigned int big_path_;
  unsigned int big_path_number_;  
  unsigned int unsolved_double_problem_;
  unsigned int max_path_leght_; // Limited by coumputer memory
  double border_path_gradient_;
};

#endif // HIDRO_FIX_PFS_H_