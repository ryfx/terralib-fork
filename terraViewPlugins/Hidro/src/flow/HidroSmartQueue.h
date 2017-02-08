/*! \file HidroSmartQueue.h
    \brief SmartQueue Class for manager the memory for a queue.
*/

#ifndef HIDROSMARTQUEUE_H
  #define HIDROSMARTQUEUE_H

  #define MAXMEMPERCENTUSAGE 10

  #include <TeUtils.h>

  #include <queue>

  /**
  * class HidroSmartQueue
  * @brief This is the class manages the memory for a queue.
  *
  * @note The used memory ( RAM or mapped memory ) will be automatically choosed
  * following the current system resources.
  *
  * @author Alexandre Copertino Jardim <alexcj@dpi.inpe.br>
  * @ingroup Flow
  */
  class HidroSmartQueue
  {
  public:

    /**
     * @brief Default Constructor.
     */
    HidroSmartQueue();

    /**
     * @brief Default Destructor
     */
    ~HidroSmartQueue();

    
		/** @name Queue methods
		 *  Queue Methods re-implemented
		 */
		//@{
      bool empty();              
      void push( int* v );
      void pop();
      int* front();
      size_t size();
    //@}

    /** \brief Method that create a new pit.
		 *  \return a new pit (new int[4]).
		 */
    int* newPit();

    void newPit( unsigned int column, unsigned int line );

    void newPit( unsigned int column, unsigned int line, unsigned int treatment );

    /** \brief Send the first pit of the queue to the back.
		 */
    void sendFront2Back();

    /**
     * @brief Set the max of memory that the queue can use.
     *
     * \param maxMemPercentUsage the percentage of memory that the queue can use.
     *
     * \return the max of memory that the queue can use.
     */
    unsigned int setMaxMemPercentUsage( double maxMemPercentUsage );

    /**
     * @brief Get the max of memory that the queue can use.
     *
     * \return the max of memory that the queue can use.
     */
    unsigned int getMaxMem();

  protected:

    /** \brief Copy the back queue to the disk.
		 */
    void copyBack2File();

    /** \brief Copy the first disk queue to the front queue.
		 */
    void copyFile2Front();    

    /** \brief Front Memory queue.
		 */
    std::queue< int* > queue_front_;
    
    /** \brief Back Memory queue.
		 */
    std::queue< int* > queue_back_;
    
    /** \brief This queue store the file names.
		 */
    std::queue< char* > queue_file_;

    /** \brief Max size of queue tiles.
		 */
    unsigned int max_queue_size_;

    unsigned int max_ram_;
  };

#endif // HidroSmartQueue_H