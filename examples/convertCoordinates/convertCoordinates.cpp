/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright © 2001-2004 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The library provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use
of this library and its documentation.
*************************************************************************************/
/* 
        This file showns an example of how to convert coordinates from a projection to another.

        Author: Lubia Vinhas  
*/

#include "../../src/terralib/kernel/TeProjection.h"
#include <iostream>

int main()
{
	TeDatum dSAD69 = TeDatumFactory::make("SAD69");		// SAD69 Spheroid

	TeDatum dWGS84 = TeDatumFactory::make("WGS84");		// WGS84 Spheroid

	TeUtm* pUTM = new TeUtm(dSAD69,-45.0*TeCDR);		// Origin latitude of -45.0
														// TeCDR means "Convert to Degrees from Radians"

	TePolyconic* pPolyconic = new TePolyconic(dWGS84,-45.0*TeCDR);	// Origin latitude of -45.0
																																			// TeCDR means "Converte Degrees from Radians"
	TeCoord2D pt1(340033.47, 7391306.21);		// Original coordinate in UTM

	// Conversion from the UTM to the Polyconic projection
	pUTM->setDestinationProjection(pPolyconic);

	TeCoord2D ll = pUTM->PC2LL(pt1);			// Convert to Lat Long
	TeCoord2D pt2 = pPolyconic->LL2PC(ll);		// Convert to output projection

	printf("UTM -> Polyconic \n");
	printf("(%.4f, %.4f) -> ",pt1.x(), pt1.y());
	printf("(%.4f, %.4f) \n",pt2.x(), pt2.y());

	// Conversion from the Polyconic to the UTM projection
	pPolyconic->setDestinationProjection(pUTM);
	ll = pPolyconic->PC2LL(pt2);
	pt1 = pUTM->LL2PC(ll);

	printf("\nPolyconic -> UTM \n");
	printf("(%.4f, %.4f) -> ",pt2.x(), pt2.y());
	printf("(%.4f, %.4f) \n",pt1.x(), pt1.y());

	cout << "\nPress Enter\n" ;
	cout.flush();
	getchar();
	return 0;
}
