#include <HidroFlowUtils.h>

#include <TeDatabase.h>
#include <TeRaster.h>
#include <TeView.h>
#include <TeRasterRemap.h>
#include <TeDecoderMemoryMap.h>
#include <TeImportRaster.h>
#include <TeLayer.h>
#include <TeProgress.h>

#include <iostream>
#include <fstream>

#include <HidroPersister.h>
#include <HidroFlowD8.h>
#include <HidroUtils.h>
#include <HidroMatrix.hpp>

#include <qmessageBox.h>

bool convertLDD( TeDatabase* db, std::string lddInputName, int inputLDDmap[10], std::string lddOutputName, int outputLDDmap[10], int dataType, bool useDummy )
{
  // 1- open Input LDD
  TeLayer* inputLayer = NULL;
  HidroUtils utils( db );
  inputLayer = utils.getLayerByName( lddInputName );
  if( !inputLayer )
  {
    QMessageBox::warning( 0, "Warning", "Error getting layer from database." );
    return false;
  }
  if( !inputLayer->hasGeometry( TeRASTER ) )
  {
    QMessageBox::warning( 0, "Warning", "InputLayer nao tem teraster." );
    return false;
  }   
  TeRaster* inputLDDRaster = inputLayer->raster();
  TeRasterParams inputParams = inputLDDRaster->params();

  // 2- create Output LDD
  // check if layer name already exits in the database
  int i = 0;
  while( db->layerExist( lddOutputName ) )
  {
    i++;
    lddOutputName = lddOutputName + "_" + Te2String(i);
  }   
  TeRasterParams params( inputParams );
  params.decoderIdentifier_ = "SMARTMEM";   
  params.mode_ = 'w';
  if( useDummy )
  {
    params.useDummy_ = true;
    params.setDummy( outputLDDmap[9] );      
  }
  else
  {
    params.useDummy_ = false;
  }
  
  switch ( dataType )
  {
    case 1: // TerraHidro
      params.setDataType( TeUNSIGNEDCHAR ); 
      break;
    case 2: // Hande
      params.setDataType( TeUNSIGNEDCHAR ); 
      break;
    case 3: // MGB
      params.setDataType( TeSHORT );
      break;
    default: // como eu não sei os valores maximo e minimo
      params.setDataType( TeINTEGER );
      break;
  }  

  TeRaster* outputLDDRaster = new TeRaster( params );
  // verify if output raster created is valid
  if( !outputLDDRaster->init() )
  {
    QMessageBox::warning(0, "Warning", "Error create TeRaster.");
    return false;
  }

  // 3- Convert
  int rasterColumns = params.ncols_;
  int rasterLines = params.nlines_;

  double input;
  int output;

  // Progress Bar
  TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("TerraHidro");
	TeProgress::instance()->setMessage("Convert the LDD.");
  TeProgress::instance()->setTotalSteps( rasterLines );

  for( int line = 0; line < rasterLines; line++ )
  {
    for( int column = 0; column < rasterColumns; column++ )
    {
        if( inputLDDRaster->getElement( column, line, input ) )
        {
          int i = 0;
          while( (int)input != inputLDDmap[i] && i < 9 )
          {               
              i++;
          }
          output = outputLDDmap[i];
        }
        else
        {
          output = outputLDDmap[9];            
        }
        outputLDDRaster->setElement( column, line, output );
    }
    TeProgress::instance()->setProgress( line );
  }

  // 4- Save the output raster in database
  TeLayer* layer = new TeLayer(lddOutputName, db, params.projection() );
  if (layer->id() <= 0)
  {
	  delete layer;
	  delete outputLDDRaster;
	  return false;
  }

  unsigned int blockW = params.blockWidth_;
  unsigned int blockH = params.blockHeight_;

  if (!TeImportRaster(layer, outputLDDRaster, blockW, blockH, outputLDDRaster->params().compression_[0], 
	  "", outputLDDRaster->params().dummy_[0], outputLDDRaster->params().useDummy_, outputLDDRaster->params().tiling_type_))
  {
	  delete layer;
	  delete outputLDDRaster;
	  return false;
  }

  // Finishi progress bar
  TeProgress::instance()->reset();

  return true;
}

std::vector<std::string> getHidroFlowAlgorithms()
{
	std::vector<std::string> algorithms;
	
	//algorithms.push_back(HIDRO_FLOW_DEM_8D_RANDOM_FLOW_NEW);
	algorithms.push_back(HIDRO_FLOW_DEM_8D_COMMON_FLOW_NEW);

	return algorithms;
}

bool generateRandLDD1 (TeRaster* inputRaster, TeRaster*& outputRaster, int col, int lin, unsigned char lddDirectionsMap[8])
{
	double altimetriaNeighbors[8], altimetriaCenter;
	double slopeNeighbors [8];

	if (inputRaster->getElement(col, lin, altimetriaCenter))
	{//if lddDirectionsMap is not dummy
		//Get input raster values
		if (!inputRaster->getElement(col-1, lin-1, altimetriaNeighbors[0]))
			altimetriaNeighbors[0] = -1;

		if (!inputRaster->getElement(col-1, lin,   altimetriaNeighbors[1]))
			altimetriaNeighbors[1] = -1;

		if (!inputRaster->getElement(col-1, lin+1, altimetriaNeighbors[2]))
			altimetriaNeighbors[2] = -1;

		if (!inputRaster->getElement(col,   lin-1, altimetriaNeighbors[3]))
			altimetriaNeighbors[0] = -1;

		if (!inputRaster->getElement(col,   lin+1, altimetriaNeighbors[4]))
			altimetriaNeighbors[2] = -1;

		if (!inputRaster->getElement(col+1, lin-1, altimetriaNeighbors[5]))
			altimetriaNeighbors[0] = -1;

		if (!inputRaster->getElement(col+1, lin,   altimetriaNeighbors[6]))
			altimetriaNeighbors[1] = -1;

		if (!inputRaster->getElement(col+1, lin+1, altimetriaNeighbors[7]))
			altimetriaNeighbors[2] = -1;

		for (int i = 0; i < 8; i++)
		{
            slopeNeighbors [i] = altimetriaCenter - altimetriaNeighbors[i];

			//Se vizinho da diagonal
			if (i == 0 || i == 2 || i == 5 || i == 7)
				slopeNeighbors [i] = slopeNeighbors [i] / 1.4142;

			if (altimetriaNeighbors[i] == -1 || slopeNeighbors [i] <= 0)
				slopeNeighbors [i] = -1;
		}

		unsigned char maxSlopeDirection;		
		double maxSlope = maxValue(slopeNeighbors , lddDirectionsMap, maxSlopeDirection);

		if (maxSlope == -1)//if there is a pit
			outputRaster->setElement(col, lin, 0);
		else
		{
			int ndirections = ndirectionCompute(slopeNeighbors , maxSlope);

			if (ndirections == 1)
				outputRaster->setElement(col, lin, maxSlopeDirection);
			else
			{//if there are multiples directions
				double v1[8];
				int itdirec = 0;
				int it = 0;

				for (int line = 0; line < 3 && itdirec != ndirections; line++)
				{
					for (int column = 0; column < 3 && itdirec != ndirections; column++)
					{
						if (line != 1 || column != 1)
						{
							if (slopeNeighbors [it] == maxSlope)
							{//If the lddDirectionsMap is the same as the lowest lddDirectionsMap
								//Stores the output values of multiples directions
								v1[itdirec] = lddDirectionsMap[it];
								itdirec++;
							}
							it++;
						}
					}
				}

				//Random selection from multiples directions
				srand(time(NULL));
				int it1 = rand() % itdirec;
				outputRaster->setElement(col, lin, v1[it1]);
			}
		}
	}
	else
		outputRaster->setElement(col, lin, 255);

	return true;
}


bool D8FlowCorrection(TeRaster*& outputRaster)
{
	int rows = outputRaster->params().nlines_;
	int cols = outputRaster->params().ncols_;

	double lddValue = 0, v1 = 0, v2 = 0;

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Correcting Flows");
		TeProgress::instance()->setTotalSteps(rows);
	}

	for (int lin = 1; lin < rows - 1; lin++)
	{
		for (int col = 1; col < cols - 1; col++)
		{
			if (outputRaster->getElement(col, lin, lddValue))
			{//if the lddDirectionsMap is not dummy
				//Correction of flow directions
				if (lddValue == 2)
				{
					outputRaster->getElement(col+1, lin, v1);
					outputRaster->getElement(col, lin+1, v2);

					if (v1 == 8)
					{
						if (v2 != 64)
							outputRaster->setElement(col, lin, 4);
					}
					else if (v2 == 128)
					{
						if (v1 != 16)
							outputRaster->setElement(col, lin, 1);
					}

				}//end if lddValue == 2
				else if (lddValue == 8)
				{
					outputRaster->getElement(col-1, lin, v1);
					outputRaster->getElement(col, lin+1, v2);

					if (v1 == 2)
					{
						if (v2 != 64)
							outputRaster->setElement(col, lin, 4);
					}
					else if (v2 == 32)
					{						
						if (v1 != 1)
							outputRaster->setElement(col, lin, 16);
					}

				}//end if lddValue == 8
				else if (lddValue == 32)
				{
					outputRaster->getElement(col-1, lin, v1);
					outputRaster->getElement(col, lin-1, v2);

					if (v1 == 128)
					{
						if (v2 != 4)
							outputRaster->setElement(col, lin, 64);
					}
					else if (v2 == 8)
					{
						if (v1 != 1)
							outputRaster->setElement(col, lin, 16);
					}

				}//end if lddValue == 32
				else if (lddValue == 128)
				{
					outputRaster->getElement(col+1, lin, v1);
					outputRaster->getElement(col, lin-1, v2);

					if (v1 == 32)
					{
						if (v2 != 4)
							outputRaster->setElement(col, lin, 64);
					}
					else if (v2 == 2)
					{
						if (v1 != 16)
							outputRaster->setElement(col, lin, 1);
					}

				}//end if lddValue == 128
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

bool D8PitCorrection(TeDatabase* db, TeRaster* inputRaster, TeRaster*& outputRaster, TeLayer*& outputLayer, unsigned char lddDirectionsMap[8],
					 std::string ldd, std::string layerName)
{
	int cols = outputRaster->params().ncols_; //number of columns from raster
	int rows = outputRaster->params().nlines_; //number of rows from raster

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Correcting pits fom a raster flow");
		TeProgress::instance()->setTotalSteps(rows * 200);
	}

	TeRasterParams params = inputRaster->params();
	params.mode_ = 'w';
	TeRaster* copyInputRaster = new TeRaster(params);

	if(!copyInputRaster->init())
	{
		delete copyInputRaster;
		return NULL;
	}

	int nstep = 0;
	int nPits = 0;

	//fstream sprcfile;
	//sprcfile.open("..\\..\\..\\..\\Fernanda\\TerraHidro\\srtm50_newldd_1.txt", ios::out);

	/* Verifica nº de fossos sem o tratamento */
	for (int lin = 1; lin < rows - 1; lin++)
	{
		for (int col = 1; col < cols - 1; col++)
		{//For all raster cells:
			double lddval = 0;

			outputRaster->getElement(col, lin, lddval);

			if (lddval == 0)
				nPits++;
		}
	}

	//sprcfile << "Usando o valor da mediana \n";
	//sprcfile << "Tamanho máximo do filtro: 15x15 \n";
	//sprcfile << "Número de fossos sem tratamento: " << nPits << "\n\n";
	//sprcfile << "Número de fossos com tratamento usando filtro da mediana:" << "\n";

	int it = 1;

	while (it <= 200)
	{
		for (int lin = 1; lin < rows - 1; lin++)
		{
			for (int col = 1; col < cols - 1; col++)
			{//For all raster cells:
				double lddval = 0;

				if (outputRaster->getElement(col, lin, lddval))
				{
					if (lddval == 0)
					{//if there is a pit
						double altimetriaCenter[9];

						copyInputRaster->getElement( col-1, lin-1, altimetriaCenter[0] );
						copyInputRaster->getElement( col-1, lin,   altimetriaCenter[1] );
						copyInputRaster->getElement( col-1, lin+1, altimetriaCenter[2] );
						copyInputRaster->getElement( col,   lin-1, altimetriaCenter[3] );
						copyInputRaster->getElement( col,   lin,   altimetriaCenter[4] );//grid center
						copyInputRaster->getElement( col,   lin+1, altimetriaCenter[5] );
						copyInputRaster->getElement( col+1, lin-1, altimetriaCenter[6] );
						copyInputRaster->getElement( col+1, lin,   altimetriaCenter[7] );
						copyInputRaster->getElement( col+1, lin+1, altimetriaCenter[8] );

						std::vector<double> median;
						std::vector<double> vecvals;
						int grid = 3;

						for (int i = 0; i < 9; i++)
							vecvals.push_back(altimetriaCenter[i]);

						for (int cont = 0; cont < 9; cont++)
						{
							int id = 0;
							double minval = vecvals[0];

							for (int i = 0; i != vecvals.size(); i++)
							{
								if (vecvals[i] < minval)
								{
									minval = vecvals[i];
									id = i;
								}
							}

							median.push_back(minval);
							vecvals.erase(vecvals.begin() + id);
						}

						double alt = median[4];
						int cell = 4;

						if (alt == altimetriaCenter[4])
						{
							double val1;
							int j = -1;// grid = 3;

							do
							{
								j--;
								grid += 2;

								if (grid > 15)
									break;

								median.clear();
								vecvals.clear();

								int ncells = grid * grid;

								int line = lin + j;//line of the grid where media is calculated

								for (int l = 0; l < grid; l++)
								{
									int column = col + j;//column of the grid where media is calculated

									for (int c = 0; c < grid; c++)
									{
										copyInputRaster->getElement(column, line, val1);
										vecvals.push_back(val1);

										column++;
									}

									line++;
								}

								for (int cont = 0; cont < ncells; cont++)
								{
									int id = 0;
									double minval = vecvals[0];

									for (int i = 0; i != vecvals.size(); i++)
									{
										if (vecvals[i] < minval)
										{
											minval = vecvals[i];
											id = i;
										}
									}

									median.push_back(minval);
									vecvals.erase(vecvals.begin() + id);
								}

								cell = (ncells / 2);
								alt = median[cell];

							}
							while (alt <= altimetriaCenter[4]);

						}//end if (alt == altimetriaCenter[4])

						/** **/
						if (alt <= altimetriaCenter[4])
						{
							int i = 1;

							do
							{
								int pos = cell + i;

								if (pos == median.size())
									break;

								alt = median[pos];
								i++;
							}
							while (alt <= altimetriaCenter[4]);

						}
						/** **/

						if (alt > altimetriaCenter[4])
						{
							copyInputRaster->setElement(col, lin, alt);

							//gerar novamente o fluxo
							int line = lin - 1;
							for (int l = 0; l < 3; l++)
							{
								int column = col - 1;
								for (int c = 0; c < 3; c++)
								{

									if (ldd == "common")
									{
										//Alexandre generateCommonLDD(copyInputRaster, outputRaster, column, line, lddDirectionsMap);
									}

									column++;
								}

								line++;
							}

						}

					}//end if lddval == 0
				}
			}

			if(TeProgress::instance())
			{
				TeProgress::instance()->setProgress(nstep);
			}

			nstep++;
		}

		/* Verifica nº de fossos durante tratamento de fosso*/
		nPits = 0;
		for (int lin1 = 1; lin1 < rows - 1; lin1++)
		{
			for (int col1 = 1; col1 < cols - 1; col1++)
			{
				double valPt = 0;
				outputRaster->getElement(col1, lin1, valPt);

				if (valPt == 0)
				{
					nPits++;
				}
			}
		}

		//sprcfile << it << "ª iteração: ";
		//sprcfile << nPits << " fossos" << "\n";

		if (nPits == 0)
			break;

		it++;
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	//sprcfile.close();

	string outLayerName = layerName + "_DEMcorr";
	int i = 0;

	while (db->layerExist(outLayerName))
	{
		i++;
		outLayerName = layerName + "_DEMcorr" + Te2String(i);
	}

	outputLayer = new TeLayer(outLayerName, db, copyInputRaster->projection());

	if (outputLayer->id() <= 0)
    {
		delete copyInputRaster;
		delete outputLayer;
		return false;
	}

	if (!TeImportRaster(outputLayer, copyInputRaster, params.ncols_, params.nlines_))
    {
		delete copyInputRaster;
		delete outputLayer;
		return false;
	}

	delete copyInputRaster;

	return true;
}


bool D8PitCorrectionMedia(TeDatabase* db, TeRaster* inputRaster, TeRaster*& outputRaster, TeLayer*& outputLayer, unsigned char lddDirectionsMap[8],
						  std::string ldd, std::string layerName)
{
	int cols = outputRaster->params().ncols_; //number of columns from raster
	int rows = outputRaster->params().nlines_; //number of rows from raster

  // Copy the input raster
	TeRasterParams params = inputRaster->params();
	params.mode_ = 'w';
	params.decoderIdentifier_ = "SMARTMEM";
	TeRaster* copyInputRaster = new TeRaster(params);
	if(!copyInputRaster->init())
	{
		delete copyInputRaster;
		return NULL;
	}
	// copy raster values
	TeRasterRemap remap;
	remap.setInput(inputRaster);
	remap.setOutput(copyInputRaster);
	if(!remap.apply(true))
	{
		delete copyInputRaster;
		return NULL;
	}

	int nstep = 0;
	int nPits = 0;


	//fstream sprcfile;
	//sprcfile.open("..\\..\\..\\..\\Fernanda\\TerraHidro\\elevation_media.txt", ios::out);


	// Verifica nº total de fossos sem o tratamento
	for (int lin = 1; lin < rows - 1; lin++)
	{
		for (int col = 1; col < cols - 1; col++)
		{//For all raster cells:
			double lddval = 0;

			outputRaster->getElement(col, lin, lddval);

			if (lddval == 0)
				nPits++;
		}
	}


	//sprcfile << "Usando o valor da media \n";
	//sprcfile << "Tamanho máximo do filtro: 15x15 \n";
	//sprcfile << "Número total de fossos: " << nPits << "\n";
	nPits = 0;

	// Verifica n° de fossos a serem tratados
	for (int lin = 2; lin < rows-2; lin++)
	{
		for (int col = 2; col < cols-2; col++)
		{//For all raster cells:
			double lddval = 0;

			outputRaster->getElement(col, lin, lddval);

			if (lddval == 0)
				nPits++;
		}
	}

	//sprcfile << "Número de fossos a serem tratados: " << nPits << "\n\n";
	//sprcfile << "Número de fossos com tratamento usando filtro da media:" << "\n";


	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Correcting pits fom a raster flow");
		TeProgress::instance()->setTotalSteps(rows * 300);
	}

	int it = 0;
	int gridsize = 0;

	while (it < 300)
	{
    // For each cell excluding boudary region
		for (int lin = 2; lin < rows - 2; lin++)
		{
			for (int col = 2; col < cols - 2; col++)
			{
				double lddval = 0;

				if (outputRaster->getElement(col, lin, lddval))
				{
          //if there is a pit
					if (lddval == 0)
					{
						double val[9];

						if (!copyInputRaster->getElement( col-1, lin-1, val[0] ))
							val[0] = -1;
						if (!copyInputRaster->getElement( col-1, lin,   val[1] ))
							val[1] = -1;
						if (!copyInputRaster->getElement( col-1, lin+1, val[2] ))
							val[2] = -1;
						if (!copyInputRaster->getElement( col,   lin-1, val[3] ))
							val[3] = -1;
						if (!copyInputRaster->getElement( col,   lin,   val[4] ))
							val[4] = -1;
						if (!copyInputRaster->getElement( col,   lin+1, val[5] ))
							val[5] = -1;
						if (!copyInputRaster->getElement( col+1, lin-1, val[6] ))
							val[6] = -1;
						if (!copyInputRaster->getElement( col+1, lin,   val[7] ))
							val[7] = -1;
						if (!copyInputRaster->getElement( col+1, lin+1, val[8] ))
							val[8] = -1;

						std::vector<double> vecvals;
						int grid = 3;
						double media = 0;

						for( int i = 0; i < 9; i++ )
						{
							if (val[i] != -1)
                vecvals.push_back(val[i]);
						}

						for( int i = 0; i != vecvals.size(); i++ )
						{
							media += vecvals[i];
						}

						media = media / vecvals.size();

						int cell = 4;

            // verifica se a altimetria calculada é menor que altimetria a ser corrigida (antiga)
            // caso seja verdadeiro aumentar o tamanho da janela
						if (media <= val[4])
						{
							double val1;
							int j = -1;// grid = 3;

							do
							{
								j--;
								grid += 2;
								media = 0;

								if (grid > 15)
								{
									gridsize++;
									break;
								}

								//median.clear();
								vecvals.clear();

								int ncells = grid * grid;

								int line = lin + j;//line of the grid where media is calculated

								for (int l = 0; l < grid; l++)
								{
									int column = col + j;//column of the grid where media is calculated

									for (int c = 0; c < grid; c++)
									{
										if (!copyInputRaster->getElement(column, line, val1))
											val1 = -1;

										if (val1 != -1)
                      vecvals.push_back(val1);

										column++;
									}

									line++;
								}

								for (int i = 0; i != vecvals.size(); i++)
								{
									media += vecvals[i];
								}

								media = media / vecvals.size();

							}
							while (media <= val[4]);

						}//end if (alt == val[4])


            // Se a altimetria calculada for maior que a antiga atualiza o valor no DEM corrigido
						if (media > val[4])
						{
							copyInputRaster->setElement(col, lin, media);

							// gera novamente o fluxo apenas onde a altimetria foi alterada
							int line = lin - 1;            
							for (int l = 0; l < 3; l++)
							{
								int column = col - 1;
								for (int c = 0; c < 3; c++)
								{

									if (ldd == "random")
									{
										generateRandLDD1(copyInputRaster, outputRaster, column, line, lddDirectionsMap);
									}

									else if (ldd == "common")
									{
										// Alexandre generateCommonLDD(copyInputRaster, outputRaster, column, line, lddDirectionsMap);
									}
									column++;
								}

								line++;
							}
						}
					}//end if lddval == 0
				}
			}

			if(TeProgress::instance())
			{
				TeProgress::instance()->setProgress(nstep);
			}
			nstep++;
		}

		/* Verifica nº de fossos durante tratamento de fosso*/
		nPits = 0;
		for (int lin1 = 2; lin1 < rows - 2; lin1++)
		{
			for (int col1 = 2; col1 < cols - 2; col1++)
			{
				double valPt = 0;
				outputRaster->getElement(col1, lin1, valPt);

				if (valPt == 0)
				{
					nPits++;
				}
			}
		}

		//sprcfile << it << "ª iteração: ";
		//sprcfile << nPits << " fossos" << "\n";


		if (nPits == 0)
			break;

		it++;
	}//end while (it <= 300)

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}


	//sprcfile.close();


  // Salva o DEM corrigido no Banco acionando "_DEMcorr[i]" no final do nome

	std::string DEMcorrName = layerName + "_DEMcorr";
	int i = 0;
	while (db->layerExist(DEMcorrName))
	{
		i++;
		DEMcorrName = layerName + "_DEMcorr" + Te2String(i);
	}

	outputLayer = new TeLayer(DEMcorrName, db, copyInputRaster->projection());

	if (outputLayer->id() <= 0)
    {
		delete outputLayer;

		delete copyInputRaster;
		return false;
	}


	if (!TeImportRaster(outputLayer, copyInputRaster, params.ncols_, params.nlines_))
    {
		delete outputLayer;
		delete copyInputRaster;
		return false;
	}
	delete copyInputRaster;

	return true;
}

double minValue(double slopeNeighbors[8], unsigned char lddDirectionsMap[8], unsigned char& minSlopeDirection)
{
  double vmin = TeMAXFLOAT;
  minSlopeDirection = 0; //used if exist only one direction
	  
  for (int i = 0; i < 8; i++)
  {
	  if( slopeNeighbors[i] != -1 )
	  {
		  if( slopeNeighbors[i] < vmin)
		  {
			  vmin = slopeNeighbors[i];
			  minSlopeDirection = lddDirectionsMap[i];
		  }
	  }
  }
  
  return vmin;
}

double maxValue(double slopeNeighbors[8], unsigned char lddDirectionsMap[8], unsigned char& maxSlopeDirection)
{
	double maxSlope = slopeNeighbors[0];
	maxSlopeDirection = lddDirectionsMap[0];

	for (int i = 1; i < 8; i++)
	{
		if (slopeNeighbors[i] > maxSlope)
		{
			maxSlope = slopeNeighbors[i];
			maxSlopeDirection = lddDirectionsMap[i];
		}
	}

  return maxSlope;
}


int ndirectionCompute( double altimetriaCenter[8], double maxSlope )
{
  int ndirections = 0;

  for( int i = 0; i < 8; i++ )
  {
    if( maxSlope == altimetriaCenter[i] )
		ndirections++;
  }

  return ndirections;
}

double altimetriaMeanCompute( HidroMatrix<double>& demMatrix, int gridlin, int gridcol)
{	
	double altimetria = 0.0, media = 0.0;
	int ncell = 0;

  // Compute the start and end of window (3x3)
  int startColumn = gridcol - 1;
  int endColumn = gridcol +  2;
  int startLine = gridlin - 1;
  int endLine = gridlin + 2;

  // Verify Borders. The HidroMatrix dont do it!
  if( startColumn < 0 )
    startColumn = 0;
  if( endColumn > demMatrix.GetColumns() )
    endColumn = demMatrix.GetColumns();
  if( startLine < 0 )
    startLine = 0;
  if( endLine > demMatrix.GetLines() )
    endLine = demMatrix.GetLines();

	for( int l = startLine; l < endLine; l++ )
	{		
		for( int c = startColumn; c < endColumn; c++ )
		{			
			if( demMatrix.getElement( c, l, altimetria ) )
			{
				media += altimetria;
				ncell += 1;//number of cells in the grid
			}			
		}		
	}

	//Calculates media from grid
	media = media / ncell;

	return media;
}

bool generatePtsPits(TeDatabase* db, TeView* view, TeRaster* raster, std::string layerName, std::string lddName)
{
	int i = 1;
	int nlines = raster->params().nlines_;
	int ncols = raster->params().ncols_;

	TePointSet ptset;

	for (int line = 2; line < nlines-2; line++)
	{
		for (int col = 2; col < ncols-2; col++)
		{
			double altimetriaCenter;
			raster->getElement(col, line, altimetriaCenter);

			if (altimetriaCenter == 0)
			{
				TePoint pt;
				TeCoord2D coord = raster->index2Coord(TeCoord2D(col, line));
				pt.add(coord);
				ptset.add(pt);
			}
		}
	}

  if( ptset.empty() )
    return false;

	TeLayer* layer = NULL;

	std::string layerPtsName = layerName + "_" + lddName + "_pits";
	TeProjection* proj = raster->projection();
	while (db->layerExist(layerPtsName) == true)
	{
		i++;
		layerPtsName += "_" + i;
	}

	layer= new TeLayer(layerPtsName, db, proj);

	if (!layer)
	{
		return false;
	}

	TeTable pointTable;
		
	if (!createTable(db, pointTable, layerPtsName))
	{
		return false;
	}

	if (!valuesTable(layer, pointTable, raster, ptset))
	{
		return false;
	}

	if (!saveLayer(db, layer, pointTable))
	{
		return false;
	}

	HidroPersister hidro(db);

    hidro.createTheme(layer->name(), "", view, layer);

	return true;
}

bool createTable(TeDatabase* db, TeTable& pointTable, std::string layerName)
{
	std::string tablePointName = layerName + "_Table";

	TeAttributeList attrList;

	TeAttribute id;
	id.rep_.type_ = TeSTRING;
	id.rep_.numChar_ = 100;
	id.rep_.isPrimaryKey_ = true;
	id.rep_.name_ = "linkcolumn";
	attrList.push_back(id);

	TeAttribute x;
	x.rep_.type_ = TeREAL;
	x.rep_.isPrimaryKey_ = false;
	x.rep_.name_ = "x_coord";
	attrList.push_back(x);

	TeAttribute y;
	y.rep_.type_ = TeREAL;
	y.rep_.isPrimaryKey_ = false;
	y.rep_.name_ = "y_coord";
	attrList.push_back(y);

	pointTable.name(tablePointName);
	pointTable.setAttributeList(attrList); 
	pointTable.setLinkName("linkcolumn");
	pointTable.setUniqueName("linkcolumn");
	 
	//creates attributes table
	if (!db->createTable(tablePointName, attrList))
	{
		return false;
	}

	return true;
}

bool valuesTable(TeLayer* layer, TeTable& pointTable, TeRaster* raster, TePointSet ptset)
{
	TePointSet ps;
	int itstep = 0, i = 0, id = 0;

	if (TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro Plugin");
		TeProgress::instance()->setMessage("Inserting points of pits in the table...");
		TeProgress::instance()->setTotalSteps(ptset.size());
	}

	//insert attributes values in the table
	TePointSet::iterator itPS = ptset.begin();

	while(itPS != ptset.end())
	{
		std::string objId = Te2String(id, 0);

		double x = itPS->location().x();
		double y = itPS->location().y();

		TeTableRow row;
		row.push_back(objId);
		row.push_back(Te2String(x, 4));
		row.push_back(Te2String(y, 4));
		pointTable.add(row);

		TePoint pt (*itPS);
		pt.objectId(objId);

		ps.add(pt);

		//add attr table in layer
		if (!layer->saveAttributeTable(pointTable))
		{
			return false;
		}

		pointTable.clear();

		//add points in layer
		if (!layer->addPoints(ps))
		{
			return false;
		}

		ps.clear();
		
		if (TeProgress::instance())
		{
			TeProgress::instance()->setProgress(itstep);
		}

		++id;
		++itstep;
		++ itPS;
	}
	
	if (TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	return true;
}

bool saveLayer(TeDatabase* db, TeLayer* layer, TeTable& pointTable)
{
	if (TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro Plugin");
		TeProgress::instance()->setMessage("Saving output layer...");
		TeProgress::instance()->setTotalSteps(2);
	} 

	if (!layer->database()->insertMetadata(layer->tableName(TePOINTS),layer->database()->getSpatialIdxColumn(TePOINTS), 0.0005,0.0005,layer->box()))
	{
		return false;
	}

	if (TeProgress::instance())
	{
		TeProgress::instance()->setProgress(1);
	}

	if (!layer->database()->createSpatialIndex(layer->tableName(TePOINTS),layer->database()->getSpatialIdxColumn(TePOINTS), (TeSpatialIndexType)TeRTREE))
	{
		return false;
	}

	if (TeProgress::instance())
	{
		TeProgress::instance()->setProgress(2);
	}

	if (!db->insertTableInfo(layer->id(), pointTable))
	{
		return false;
	}

	layer->addAttributeTable(pointTable);

	if (TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	return true;
}

bool slopeCompute( HidroMatrix<double>& demMatrix, int col, int lin, double slopeNeighbors[8] )
{
  double altimetriaCenter;
  double altimetriaNeighbors;

  // altimetria in the center cell
  if( !demMatrix.getElement( col, lin, altimetriaCenter ) )
  {
    return false;
  }

  //upper left
	if( !demMatrix.getElement( col-1, lin-1, altimetriaNeighbors ) )
	{		
		slopeNeighbors [0] = -1;
	}
	else
		slopeNeighbors[0] = (altimetriaCenter - altimetriaNeighbors) / M_SQRT2;

  //up
	if( !demMatrix.getElement( col, lin-1,   altimetriaNeighbors ) )
	{		
		slopeNeighbors[1] = -1;
	}
	else
		slopeNeighbors [1] = altimetriaCenter - altimetriaNeighbors;

  //upper right
	if( !demMatrix.getElement( col+1, lin-1, altimetriaNeighbors ) )
	{		
		slopeNeighbors[2] = -1;
	}
	else
		slopeNeighbors[2] = (altimetriaCenter - altimetriaNeighbors) / M_SQRT2;

  //left
	if( !demMatrix.getElement(col-1,   lin, altimetriaNeighbors ) )
	{		
		slopeNeighbors[3] = -1;
	}
	else
		slopeNeighbors[3] = altimetriaCenter - altimetriaNeighbors;

  //right
	if( !demMatrix.getElement(col+1,   lin, altimetriaNeighbors ) )
	{		
		slopeNeighbors[4] = -1;
	}
	else
		slopeNeighbors[4] = altimetriaCenter - altimetriaNeighbors;

  //lower left
	if( !demMatrix.getElement( col-1, lin+1, altimetriaNeighbors ) )
	{
		slopeNeighbors[5] = -1;
	}
	else
		slopeNeighbors[5] = (altimetriaCenter - altimetriaNeighbors) / M_SQRT2;

  //lower   
	if( !demMatrix.getElement( col, lin+1,   altimetriaNeighbors ) )
	{
		slopeNeighbors[6] = -1;
	}
	else
		slopeNeighbors[6] = altimetriaCenter - altimetriaNeighbors;

  //lower right
	if( !demMatrix.getElement( col+1, lin+1, altimetriaNeighbors ) )
	{
		slopeNeighbors[7] = -1;
	}
	else
    slopeNeighbors[7] = (altimetriaCenter - altimetriaNeighbors) / M_SQRT2;

  return true;
}

bool isBorder( HidroMatrix<double>& demMatrix, int col, int lin )
{
  if( col < 1 || col > (demMatrix.GetColumns() - 2) ||
    lin < 1 || lin > (demMatrix.GetLines() - 2) )
  {
    return true;
  }
  else if( demMatrix.hasDummy() )
  {
    // Compute the start and end of window (3x3)
    unsigned int startColumn = col - 1;
    unsigned int endColumn = col +  2;
    unsigned int startLine = lin - 1;
    unsigned int endLine = lin + 2;
  	
    double altimetria;

	  for (unsigned int l = startLine; l < endLine; l++)
	  {				
		  for (unsigned int c = startColumn; c < endColumn; c++)
		  {
        if( !demMatrix.getElement(c, l, altimetria) )
        {
          // is dummy.
          return true;
        }
      }
    }
  }

  return false;
}

std::string
Time::actualTimeString()
{
  __time64_t ltime;
  _time64( &ltime );
  std::string stime( _ctime64( &ltime ) );
  return stime;
}


std::string
Time::partialString()
{
  float duration = ( clock() - start_  ) / CLOCKS_PER_SEC;  
  std::string stime;
  stime.clear();

  // se for so segundos
  if( duration < 60 )
  {
    stime.append( Te2String( duration, 2 ) );
    stime.append( " s" );
    return stime;
  }
  // se for minutos
  else if ( duration < 3600 )
  {
    int minutos = duration / 60;
    duration = duration - minutos*60;
    int segundos = duration;
    if( minutos < 10 )
      stime.append( "0" );
    stime.append( Te2String( minutos ) );
    stime.append( ":" );
    if( segundos < 10 )
      stime.append( "0" );
    stime.append( Te2String( segundos ) );
    //stime.append( " m:s" );
  }
  // se for horas
  else
  {
    int horas = duration / 3600;
    duration = duration - horas*3600;
    int minutos = duration / 60;
    duration = duration - minutos*60;
    int segundos = duration;
    stime.append( Te2String( horas ) );
    stime.append( ":" );
    if( minutos < 10 )
      stime.append( "0" );
    stime.append( Te2String( minutos ) );
    stime.append( ":" );
    if( segundos < 10 )
      stime.append( "0" );
    stime.append( Te2String( segundos ) );
    //stime.append( " h:m:s" );
  }
  
  return stime;
}

void
Time::reset()
{
  start_ = clock();
  partial_ = start_;
}

void Time::start()
{
  start_ = clock();
  partial_ = start_;
}

bool Time::hasPassedOneHouer()
{
  float duration = ( clock() - partial_  ) / CLOCKS_PER_SEC;

  if( duration > 3600 )
  {
    partial_ = clock();
    return true;
  }

  return false;
}