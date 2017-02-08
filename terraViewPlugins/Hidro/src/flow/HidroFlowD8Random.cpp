#include <HidroFlowD8Random.h>
#include <HidroFlowUtils.h>
#include <HidroVertex.h>
#include <HidroEdge.h>
#include <HidroGraph.h>

#include <TeRaster.h>
#include <TeDatabase.h>

#include <TeProgress.h>

bool generateFlowD8Random(TeDatabase* db, TeRaster* inputRaster, TeRaster*& outputRaster, bool pitcorrec, std::string postProcName)
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

	int cols = inputRaster->params().ncols_;//columns number from raster
	int rows = inputRaster->params().nlines_;//rows number from raster

	//grid values of output raster
	double value[3][3];
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

	for (int lin = 1; lin < rows - 1; lin++)
	{
		for (int col = 1; col < cols - 1; col++)
		{
			if (!generateRandLDD(inputRaster, outputRaster, col, lin, value))
			{
				return false;
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

	if (pitcorrec == true)
	{
		string ldd = "random";

		if (postProcName == "Pit Correction - Median")
		{
		//if (!D8PitCorrection(db, inputRaster, outputRaster, value, ldd))
		//{
		//	return false;
		//}
		}
		else if (postProcName == "Pit Correction - Media")
		{
			//if (!D8PitCorrectionMedia(db, inputRaster, outputRaster, value, ldd))
			//	return false;
		}
	}

	if (!D8FlowCorrection(outputRaster))
	{
		return false;
	}

	return true;
}
