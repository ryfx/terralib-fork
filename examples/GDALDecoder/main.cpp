/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright 2001-2007 INPE and Tecgraf/PUC-Rio.

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

/*! \file main.cpp
    \brief This file contains a set of examples that use the GDAL raster decoder.
    \author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraLib
#include "../../src/terralib/drivers/gdal/TeGDALDecoder.h"
#include "../../src/terralib/kernel/TeRaster.h"
#include "../../src/terralib/kernel/TeRasterParams.h"

// STL
#include <cstdlib>
#include <iostream>
#include <vector>

// Registering the gdal decoder factory
static TeGDALDecoderFactory gdalDecoder("GDAL");

TeRaster* ReadRasterUsingGDAL(const std::string& path, char mode)
{
	// Setup the params that will be used
	TeRasterParams params;
	params.mode_ = mode;
	params.fileName_ = path;
	params.decoderIdentifier_ = "GDAL";

	// Reads the raster file using GDAL decoder
	TeRaster* raster = new TeRaster(params);
	if(!raster->init())
	{
		std::cout << "- Error reading the raster file " + path + " using GDAL decoder..." << std::endl;
		delete raster;
		return 0;
	}

	std::cout << "- Image file " + path + " read using GDAL!" << std::endl;

	return raster;
}

TeRaster* CreateRasterUsingGDAL(const std::string& path, TeRasterParams& params)
{
	params.fileName_ = path;
	params.mode_ = 'c';
	params.decoderIdentifier_ = "GDAL";

	// Creates the new raster
	TeRaster* raster = new TeRaster(params);
	if(!raster->init())
	{
		std::cout << "- Error creating the new raster " + path + " using GDAL decoder..." << std::endl;
		std::cout << " - Details: " << raster->params().errorMessage_;
		delete raster;
		return 0;
	}

	std::cout << "- Image file " + path + " created using GDAL!" << std::endl;

	return raster;
}

void CreateRandomImage(const std::string& path)
{
	// Setup the random image
	TeRasterParams params;
	params.nlines_ = 512;
	params.ncols_= 512;
	params.nBands(3);
	params.setDataType(TeUNSIGNEDCHAR);

	TeRaster* img = CreateRasterUsingGDAL(path, params);
	if(img == 0)
		return;

	// Randomize pixels color
	for(int b = 0; b < params.nBands(); ++b)
		for(int l = 0; l < params.nlines_; ++l)
			for(int c = 0; c < params.ncols_; ++c)
				img->setElement(c, l, rand() % 255, b);

	delete img;
}

void ConvertImage(TeRaster* inputRaster, const std::string& toFormat)
{
	TeRasterParams params(inputRaster->params());

	TeRaster* outputRaster = CreateRasterUsingGDAL("D:\\GeoData\\images\\gdal\\imgCopy." + toFormat, params);
	if(outputRaster == 0)
		return;

	// Copy the pixels
	double val;
	for(int b = 0; b < params.nBands(); ++b)
		for(int l = 0; l < params.nlines_; ++l)
			for(int c = 0; c < params.ncols_; ++c)
			{
				inputRaster->getElement(c, l, val, b);
				outputRaster->setElement(c, l, val, b);
			}

	delete outputRaster;
}

int main()
{
    std::cout << "- Testing GDAL raster decoder..." << std::endl;

	// List the all available GDAL drivers
	const std::vector<TeGDALDriverInfo>& drivers = TeGDALDecoder::getGDALDrivers();
	std::cout << "- Available GDAL drivers: " << drivers.size() << "\n" << std::endl;
	for(unsigned int i = 0; i < drivers.size(); ++i)
		drivers[i].print();

	// Full supported files filter
	std::cout << TeGDALDecoder::getSupportedFilesFilter() << std::endl;

	// Create a tiff file with random color pixels
	CreateRandomImage("D:\\GeoData\\images\\randomimage.tif");

	// Read a tiff file...
	TeRaster* myTiff = ReadRasterUsingGDAL("D:\\GeoData\\images\\gdal\\img.tif", 'r'); // 'r' to read
	/// ... and convert it to other formats
	for(unsigned int i = 0; i < drivers.size(); ++i)
	{
		if(drivers[i].hasCreateSupport())
			ConvertImage(myTiff, drivers[i].getExtension());
	}
	delete myTiff;

	// Read the created random image to update...
	TeRaster* myRandomImg = ReadRasterUsingGDAL("D:\\GeoData\\images\\randomimage.tif", 'w'); // 'w' to read + write access
	// Modifying the image. Fills the half size with black...
	for(int b = 0; b < myRandomImg->params().nBands(); ++b)
		for(int l = 0; l < myRandomImg->params().nlines_ * 0.5; ++l)
			for(int c = 0; c < myRandomImg->params().ncols_; ++c)
				myRandomImg->setElement(c, l, 0.0, b);
	
	delete myRandomImg;

    std::cout << "end!" << std::endl;
    std::cin.get();

	return 0;
}
