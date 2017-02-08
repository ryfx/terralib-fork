//This file contains definitions used by KML framework.
#ifndef TDK_KMLDEFINES_H
#define TDK_KMLDEFINES_H


#ifdef WIN32
	#ifdef TDK_KMLASDLL
		#define TDKKML_API  __declspec(dllexport)   // export DLL information
	#else
		#define TDKKML_API  __declspec(dllimport)   // import DLL information
	#endif
#else
	#define TDKKML_API
#endif

namespace tdk {

//This enum defines the type of object in a KML file.
enum KMLType {
  DOCUMENT, //!< Kml document.
  FOLDER, //!< Kml folder.
  PLACEMARK, //!< Kml placemark.
  STYLE, //!< Kml style.
  STYLEMAP //!< Kml style map.

};

} // namespace tdk

#endif
