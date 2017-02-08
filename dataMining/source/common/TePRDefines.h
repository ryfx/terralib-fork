
/*! \file TePRDefines.h
	\brief This file contains a set of general definitions used by pattern recognition module
	\author Karine Reis Ferreira <karine@dpi.inpe.br>
	\author Emiliano <castejon@dpi.inpe.br>
*/
#ifndef  __PR_INTERNAL_DEFINES_H
#define  __PR_INTERNAL_DEFINES_H


//! Status of a classifier
enum TePRClassifierStatus { TePRStartedClassifier = 0, TePRTrainedClassifier = 1};


/** @defgroup PR_AS_DLL macros.
  @{
 */
#if defined( WIN32 ) || defined( __WIN32__ ) || defined ( _WIN32 ) || defined( WIN64 ) || defined( _WIN32_WCE )

#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning(disable: 4251)
#endif

#ifdef PR_AS_DLL
#define PR_DLL __declspec(dllexport)
#else
#define PR_DLL __declspec(dllimport)
#endif

#else
#define PR_DLL
#endif
/** @} */ 


#endif


