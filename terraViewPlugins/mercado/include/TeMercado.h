/*! \file TeMercado.h
    This file contains algorithms for mercado algorithm.
	\author Eric Silva Abreu
*/

#ifndef  __FLOWPLUGIN_TeMercado_H
#define  __FLOWPLUGIN_TeMercado_H

#include <TeDatabase.h>

//! Classe que auxilia no processo de geracao de mercados
/*!
    Instancias dessa classe permite a criacao de mercados dominantes e mercados de cada objeto,
	utilizada juntamente com a interface de mercado. Tambem gera representacao de pontos a partir
	de um dado vetorial poligonal, isso eh necessario para esse processo de geracao de mercado, 
	essa classe tambem gera uma tabela objetoxevento a partir da tabela de entrada referentes
	ao evento em estudo.

 \sa
	TeFlowUtils, flowMercado
 */
class TeMercado
{

public:

	//! Constructor
	TeMercado(TeDatabase* database);

	//! Destructor
	~TeMercado();


	//! Lista as possiveis tabelas externas que podem ser utilizdas para a criacao de uma camada de informacao com fluxos
	/*!
		\param tables	Lista de saida com os nomes de tabelas externas que nao estao associadas a nenhuma outra tabela do banco
		\return			Retorna verdadeiro se for possivel localizar tabelas e falso em caso de erro
	*/
	bool listCandidateFlowDataTables(vector<string>& tables);

	//! Get all distincts objects from specific column
	/*!
		\param tableName	Name of a external table where will be get the elements
		\param columnName	Name of a column from external table
		\return				Return a vector with all elements if it works or a empty vector if its failed
	*/
	vector<string> getDistinctObjects(const string& tableName, const string& columnName);

	vector<string> getDistinctLabelObjects(const string& tableName, const string& columnName, const string& columnLabelName);

	vector<string> getDistinctTableObjects(const string& fileTableName, const string& layerName, const string& layerTableName, const string& columnName, const string& columnLabelName);

	vector<string> getLabelObjects(const string& tableName);

	string getIdFromLabel(const string& tableName, const string& labelName);

	string getLabelFromId(const string& tableName, const string& id);

	//! Create a table that will be used in mercado process
	/*!
		\param extTableName		Name of a external table where will be get the elements
		\param originColumn		Column name that will be used to get origin data information
		\param destinyColumn	Column name that will be used to get destiny data information
		\return					Return true if table created correctly, and false otherwise
	*/
	bool createMercadoTable(const string& extTableName, const string& originColumn, const string& destinyColumn, const string& linkTable, const string& linkColumn, TeTable& mercTable);

	bool createMercadoTable(const string& fileTableName, const string& originColumn, const string& linkTable, const string& linkColumn, TeTable& mercTable);

	bool createMercadoLabelTable(const string& extTableName, const string& originColumn, const string& labelColumn, TeTable& labelTable);

	bool createMercadoLabelTable(const string& fileTableName, const string& layerName, const string& layerTableName, const string& linkColumn, const string& labelColumn, TeTable& labelTable);

	//! Create points representatio in current layer
	/*!
		\param layer			layer to create a point representation
		\return					Return true if a point representation was created correctly, and false otherwise
	*/
	bool createPointRepresentation(TeLayer* layer);

	//! Create a dominance mercado raster for specifics objects
	/*!
		\param vecRas			Vector of rasters used to generate mercado dominance
		\param vecObj			Vector of string with the name of each object
		\param vecColor			Vector of color that represents each object
		\param levelDom			Information about de dominance value
		\return					Return true if raster was created correctly, and false otherwise
	*/
	TeRaster* createMercadoRaster(vector<TeRaster*> vecRas, vector<string> vecObj, vector<TeColor> vecColor, int levelMin, int levelMax);

	std::vector<TeRaster*> createMercadoRaster(std::vector<TeRaster*> vecRasIn);

	bool createMercadoRelation(TeRaster* raster, TeLayer* layer, std::string columnName, vector<string> vecObj, vector<TeColor> vecColor);

	/**	Clips a raster representation using a polygon set as a mask
	\param	whole		pointer to the raster to be clipped
	\param  mask		the polygon set used as the mask to clip the raaster
	\param  geomProj	pointer the projection of the polygons used as mask
	\param  clipName	name of the file (or database table) that will contains the clipped raster
	\param  background	value to be used as the background of the clipped raster
	\param	decId		optional parameter that explicitly indicates the decoder (or format) used to generate the clipped raster
	\return a pointer to the generated clipped raster if it succeed or a null pointer otherwise.
	
	If the decId is equal to "DB" it is assumed that the clipped raster will be generated
	in the same database as the input raster. If the input raster is not in a database, the 
	routine returns a error.
*/
	TeRaster* mercadoRasterClipping(TeRaster* whole, TePolygonSet& mask, TeProjection* geomProj, const string& clipName, double background, const string& decId="");


	bool createMercadoVetorial(	TeTheme* themeIn, const std::string& mercadoTable, const std::string& outputThemeName, TeTheme*& theme,
								std::vector<std::string>& vecObj, std::vector<std::string>& vecObjLabel, std::vector<TeColor>& vecColor, 
								const int& levelMin, const int& levelMax);

	bool createFullMercadoVetorial(	TeTheme* themeIn, const std::string& mercadoTable, const std::string& outputThemeName, TeTheme*& theme,
								std::vector<std::string>& vecObj, std::vector<std::string>& vecObjLabel, std::vector<TeColor>& vecColor, 
								const int& levelMin, const int& levelMax, std::vector<std::string>& vecAllObj);

protected:

	TeTheme* createMercadoVetorialTheme(TeLayer* vecLayer, const std::string& outputThemeName, const int& viewId, const std::string& mercadoTable);

	void createVetorialThemeLegend(TeTheme* theme, std::vector<std::string>& vecObj, std::vector<TeColor>& vecColor);

	bool createVetorialMercadoObjectVisual(TeTheme* theme, const std::string& objId, TeColor cor);

protected:

	//! get map with all mercado information
	/*!
		\param tableName		Name of a external table where will be get the elements
		\param originColumn		Column name that will be used to get origin data information
		\param destinyColumn	Column name that will be used to get destiny data information
		\return					Return a map if it works, and empty map otherwise
	*/
	map<string, map<string, int> > getMercadoMap(const string& tableName, const string& originColumn, const string& destinyColumn);

  map<string, map<string, int> > getMercadoMap(const string& fileTableName);

	//! Defines a lut table in a raster parameters
	/*!
		\param params		parameters used to create a  raster
		\param colorVec		vector of color used to fill the parameters
	*/
	void createRasterDomLut(TeRasterParams& params, vector<TeColor> colorVec);

	int getClassDom(std::map<int, int> classMap, double& relation);

	std::string getClassInfo(TeColor color, vector<string> vecObj, vector<TeColor> vecColor);

	bool addClassInfo(std::string tableName, std::string colName, std::string className, double relation, std::string objId, std::string linkColumn);


protected:
	
	typedef map<string, int> OriginMap;				//! map to store origin informatin

	typedef map<string, OriginMap > MercadoMap;		//! map to store mercado information

	TeDatabase* currentDatabase_;					//! Current Database
};

#endif	// __FLOWPLUGIN_TeMercado_H