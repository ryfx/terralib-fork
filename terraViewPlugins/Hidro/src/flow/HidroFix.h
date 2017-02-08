#ifndef HIDRO_FIX_H_
#define HIDRO_FIX_H_

// Hidro template classes
#include <HidroMatrix.hpp>
#include <HidroSmartQueue.h>

// foreigner classes
class TeDatabase;
class TeRasterParams;

/**
 * @class HidroMatrix
 * @brief This class fix the pits of an LDD.
 * @author Alexandre Copertino Jardim <alexcj@dpi.inpe.br>
 * @ingroup hidroFlow The Group for generate flow functions.
**/
class HidroFix
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
  HidroFix( TeDatabase* database,
    TeRasterParams rasterParams,
    HidroMatrix<double>& demMatrix,
    HidroMatrix<unsigned char>& lddMatrix,
    unsigned char lddDirectionsMap[8],
    std::string demName,
    std::string lddName,
    int pitMaxTreatment,
    double boost );

  /**
   * @brief Run the algorithm to fix the pits.
   * @return True for normal execution.
  **/
  bool run();

  /**
   * @brief Run the algorithm to fix the pits.
   * @return True for normal execution.
  **/
  bool run2();

  /**
   * @brief Run the algorithm to fix the pits.
   * @return True for normal execution.
  **/
  bool run3();

  /**
   * @brief Run the algorithm to fix the pits.
   * @return True for normal execution.
  **/
  bool recursive( int maxLevel, int level );

  /**
   * @brief Generate a LDD from a DEM.   
   * @return Number of pits.
   **/
  int ldd();

protected:
  /**
   * @brief Resample the DEM matrix to.
   * @param level How much resample the matrix.   
  **/
  void resample( int level );

  /**
   * @brief Back the LDD to original resolution.
   * @param level How much resample the matrix.   
  **/
  void resampleLDD( int level );

  /**
   * @brief Back the LDD to original resolution.
   * @param x Column of the cell.
   * @param y Line of the cell.
   * @param reLDDVal Resampled LDD val.
  **/
  void backLDD( unsigned int x, unsigned int y, unsigned char reLDDVal );

  /**
   * @brief Solve the pit queue.
   * @return True for normal execution.
  **/
  bool solveQueue();

  /**
   * @brief Recompute the LDD for Neighborhood.
   * @param x Column of the cell.
   * @param y Line of the cell.
  **/
  void recomputeNeighborhoodLDD( unsigned int x, unsigned int y );

  /**
   * @brief Solve peak pit.
   * @param x Column of the cell.
   * @param y Line of the cell.
  **/
  void solvePeak( unsigned int x, unsigned int y );

  /**
   * @brief Print final results in txt arquive.   
  **/
  void print();

  /**
   * @brief Initialize main pit queue.
   * @return True for normal execution.
  **/
  bool initPitQueue();

  /**
   * @brief Find the highest neighbor pit of a cell.
   * @param x Column of the cell.
   * @param y Line of the cell.
   * @param highPitNeighborX Column of the highest neighbor pit.
   * @param highPitNeighborY Line of the highest neighbor pit.
   * @return True if has a pit neighbor or False if has not a pit neighbor.
  **/
  bool highNeighborPit( unsigned int x, unsigned int y,
    unsigned int& highPitNeighborX, unsigned int& highPitNeighborY );

  /**
   * @brief Solve the queue pit, by recursive method. Its crash becouse stack overflow!!!
   * @param x Column of the pit.
   * @param y Line of the pit.
   * @return True if the pit was solved.
  **/
  bool recursiveMethod();

  /**
   * @brief Solve the pit, recompute LDD for neighbors and solve the neighbors pit starting 
   * by high neighbor pit. (recursive) .
   * @param x Column of the pit.
   * @param y Line of the pit.
   * @return True if the pit was solved.
  **/
  bool solvePit( unsigned int x, unsigned int y );

  /**
   * @brief Initialize main pit queue ordered.
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
   * @brief Find the pit regions and mark as 9 (special pit).
  **/
  void growUpPitRegions();

  /**
   * @brief Find the regions that has the same altimetria.
   * @param altmetria 
  **/
  void growUpAltimetriaRegions( unsigned int x, unsigned int y, double altimetria );

  /**
   * @brief Check if is boundary region of fix method ( -4 lines and columns of DEM ).
   * @param x Column of the cell.
   * @param y Line of the cell.
   * @return True if is boundary region.
  **/
  bool boundaryFixRegion( unsigned int x, unsigned int y );

  /**
   * @brief Verify how many neighbors are pits.
   * @param x Column of the cell.
   * @param y Line of the cell.
   * @return Number of neighbors that are pits.
  **/
  int pitsNeighbors( unsigned int x, unsigned int y );

private:
  // -- Members.
  TeDatabase* database_;
  TeRasterParams rasterParams_;
  HidroMatrix<double>& demMatrix_;
  HidroMatrix<unsigned char>& lddMatrix_;
  unsigned char* lddDirectionsMap_;

  // -- Fix parameters
  int pitMaxTreatment_;
  double boost_;

  // -- resamples
  HidroMatrix<double> resampleDEM_;
  HidroMatrix<unsigned char> resampleLDD_;

  // -- Raster limits.
  unsigned int rasterLines_;
  unsigned int rasterColumns_;

  // -- Main Pit Queue.
  HidroSmartQueue mainQueue_;
  HidroSmartQueue unsolvedQueue_;

  // -- Queue controllers and final information.
  unsigned int initialSize_;  
  unsigned int solvedPitsNumber_;
  unsigned int unsolvedPitsNumber_;

  // -- DEM and LDD output name
  std::string demName_;
  std::string lddName_;
  std::string queueStatisticsFileName_;
  std::string pitCorrectionStatisticsFileName_;

  // -- Time
  std::string starTime_;
  std::string endTime_;
  std::string partialTime_;
};

#endif // HIDRO_FIX_H_