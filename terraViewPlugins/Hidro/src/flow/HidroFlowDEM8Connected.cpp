#include <HidroFlowDEM8Connected.h>
#include <HidroFlowUtils.h>
#include <HidroVertex.h>
#include <HidroEdge.h>
#include <HidroGraph.h>

#include <TeRaster.h>

#include <TeProgress.h>

bool generateFlow(TeRaster* inputRaster, TeRaster*& outputRaster)
{
//create output raster
	TeRasterParams params(inputRaster->params());
	params.decoderIdentifier_ = "MEM";
	params.mode_ = 'c';
	params.setDataType(TeUNSIGNEDCHAR);

	outputRaster = new TeRaster(params);

//verify if output raster created is valid
	if(!outputRaster->init())
	{
		return false;
	}

	double val[3][3], value[3][3];
	int cols = inputRaster->params().ncols_;//number of raster columns
	int rows = inputRaster->params().nlines_;//number of raster rows

//grid values of output raster
	value[0][0] = 32;
	value[0][1] = 16;
	value[0][2] = 8;
	value[1][0] = 64;
	value[1][1] = 0;
	value[1][2] = 4;
	value[2][0] = 128;
	value[2][1] = 1;
	value[2][2] = 2;

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Generating Raster Flow");
		TeProgress::instance()->setTotalSteps(rows);
	}

	for (int lin = 1; lin < rows; lin++)
	{
		for (int col = 1; col < cols; col++)
		{
//Get grid values
			inputRaster->getElement(col-1, lin-1, val[0][0], 0);
			inputRaster->getElement(col-1, lin,   val[0][1], 0);
			inputRaster->getElement(col-1, lin+1, val[0][2], 0);
			inputRaster->getElement(col,   lin-1, val[1][0], 0);
			inputRaster->getElement(col,   lin,   val[1][1], 0);
			inputRaster->getElement(col,   lin+1, val[1][2], 0);
			inputRaster->getElement(col+1, lin-1, val[2][0], 0);
			inputRaster->getElement(col+1, lin,   val[2][1], 0);
			inputRaster->getElement(col+1, lin+1, val[2][2], 0);

//auxiliary variable
			double vaux = -1;

			for(int l = 0; l < 3; l++)
			{
				for(int c = 0; c< 3; c++)
				{
					if (val[c][l] < val[1][1]) 
					{
//if value lower than grid center value
						if (vaux == -1)
						{
							vaux = val[c][l];
//set the ouput raster value
							outputRaster->setElement(col, lin, value[c][l], 0);
						}
						else if(val[c][l] < vaux)
						{
//set the output raster value, new value is founded
							vaux = val[c][l];

							outputRaster->setElement(col, lin, value[c][l], 0);
						}
					}
				}
			}

			if (vaux == -1)
			{
//if no value lower than grid center value
				outputRaster->setElement(col, lin, value[1][1], 0);
			}
		}

		if(TeProgress::instance())
		{
			TeProgress::instance()->setProgress(lin);
		}
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	return true;
}

bool generateMultiFlow(TeRaster* inputRaster, TeRaster*& outputRaster)
{
//create output raster
	TeRasterParams params(inputRaster->params());
	params.decoderIdentifier_ = "MEM";
	params.mode_ = 'c';
	params.setDataType(TeUNSIGNEDCHAR);

	outputRaster = new TeRaster(params);

//verify if output raster created is valid
	if(!outputRaster->init())
	{
		return false;
	}

	double val[3][3], value[3][3];
	int cols = inputRaster->params().ncols_;//number of raster columns
	int rows = inputRaster->params().nlines_;//number of raster rows

//grid values of output raster
	value[0][0] = 32;
	value[0][1] = 16;
	value[0][2] = 8;
	value[1][0] = 64;
	value[1][1] = 0;
	value[1][2] = 4;
	value[2][0] = 128;
	value[2][1] = 1;
	value[2][2] = 2;

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Generating Raster Flow");
		TeProgress::instance()->setTotalSteps(rows);
	}

	for (int lin = 1; lin < rows; lin++)
	{
		for (int col = 1; col < cols; col++)
		{
//Get grid values
			inputRaster->getElement(col-1, lin-1, val[0][0], 0);
			inputRaster->getElement(col-1, lin,   val[0][1], 0);
			inputRaster->getElement(col-1, lin+1, val[0][2], 0);
			inputRaster->getElement(col,   lin-1, val[1][0], 0);
			inputRaster->getElement(col,   lin,   val[1][1], 0);
			inputRaster->getElement(col,   lin+1, val[1][2], 0);
			inputRaster->getElement(col+1, lin-1, val[2][0], 0);
			inputRaster->getElement(col+1, lin,   val[2][1], 0);
			inputRaster->getElement(col+1, lin+1, val[2][2], 0);

//auxiliary variable
			double vaux = -1;

			for(int l = 0; l < 3; l++)
			{
				for(int c = 0; c< 3; c++)
				{
					if (val[c][l] < val[1][1]) 
					{
//if value lower than grid center value
						if (vaux == -1)
						{
							vaux = val[c][l];
//set the ouput raster value
							outputRaster->setElement(col, lin, value[c][l], 0);
						}
						else if(val[c][l] == vaux)
						{
							double val;

//set the ouput raster value, adding the new flow direction
							outputRaster->getElement(col, lin, val, 0);
							outputRaster->setElement(col, lin, value[c][l] + val, 0);
						}
						else if(val[c][l] < vaux)
						{
//set the output raster value, new value is founded
							vaux = val[c][l];

							outputRaster->setElement(col, lin, value[c][l], 0);
						}
					}
				}
			}

			if (vaux == -1)
			{
//if no value lower than grid center value
				outputRaster->setElement(col, lin, value[1][1], 0);
			}
		}

		if(TeProgress::instance())
		{
			TeProgress::instance()->setProgress(lin);
		}
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	return true;
}
