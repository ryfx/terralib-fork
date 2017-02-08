#include <TVPLGCodeGenerator.h>

//TerraLib include files
#include <TeException.h>

//Qt include files
#include <qdir.h>
#include <qobject.h>

TVPLGCodeGenerator::TVPLGCodeGenerator()
{
}

TVPLGCodeGenerator::~TVPLGCodeGenerator()
{
}

void TVPLGCodeGenerator::generatePlgCode(const string& name, const string& className, const string& description, const string& location, const string& vendorName, 
			const string& vendorMail, const string& vendorHP)
{
	QDir dir(location.c_str());

	if(!dir.mkdir(name.c_str()))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Error creating project folder.").latin1());

	if(!dir.cd(name.c_str()))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to enter created folder.").latin1());

	generateFolders(dir);

	vector<string> headers;
	vector<string> sources;

	generatePluginHeader(dir, name, className, headers);
	generatePluginSource(dir, name, className, sources);
	generatePluginCodeHeader(dir, name, description, vendorName, vendorMail, vendorHP, headers);
	generatePluginCodeSrc(dir, name, className, sources);

	generatePro(dir, name, headers, sources);
}

void TVPLGCodeGenerator::generateFolders(QDir& projectLocation)
{
	if(!projectLocation.mkdir("include"))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Error creating include folder.").latin1());

	if(!projectLocation.mkdir("src"))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Error creating source folder.").latin1());
}

void TVPLGCodeGenerator::generatePro(QDir& projectLocation, const string& name, const vector<string>& headers, const vector<string>& srcs)
{
	QString fileName = string(name+".pro").c_str();
	QFileInfo fInfo(projectLocation, fileName);

	QFile file(fInfo.filePath());

	if(!file.open(IO_WriteOnly))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to create project file(.pro).").latin1());

	QTextStream stream(&file);

	stream	<<"#--------------------------------------------------------------------------------------------------------" <<endl
			<<"#If you want to create a visual studio project, use the commmand \"qmake -t vclib -spec win32-msvc.net\"." <<endl
			<<"#For debug version add the expression \"TE_PROJECT_TYPE=DEBUG\" to previous command." <<endl
			<<"#--------------------------------------------------------------------------------------------------------" <<endl
			<<"TARGET = " <<name.c_str() <<endl
			<<"include( ../base/base.pro )" <<endl
			<<"TRANSLATIONS = " + QString(name.c_str()).lower() + "_pt.ts \\" <<endl
			<<"		" + QString(name.c_str()).lower() + "_sp.ts" <<endl
			<<"INCLUDEPATH += ./include" <<endl
			<<"HEADERS += ";

	for(unsigned int i = 0; i < headers.size(); i++)
	{
		(i > 0) ? stream <<"	" : stream <<" ";

		stream <<"./include/" <<headers[i].c_str();
		
		if(i != headers.size()-1)
			stream <<" \\";

		stream <<endl;
	}

	stream <<"SOURCES += "; 

	for(unsigned int i = 0; i < srcs.size(); i++)
	{
		(i > 0) ? stream <<"	" : stream <<" ";

		stream <<"./src/" <<srcs[i].c_str();
		
		if(i != srcs.size()-1)
			stream <<" \\";

		stream <<endl;
	}

	stream <<"FORMS += ";

	file.close();
}

void TVPLGCodeGenerator::generatePluginHeader(QDir projectLocation, const string& name, const string& className, vector<string>& headerNames)
{
	QString fileName = QString(className.c_str()).lower();
	fileName += ".h";

	headerNames.push_back(fileName.latin1());

	if(!projectLocation.cd("include"))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to enter include folder.").latin1());

	QFileInfo fInfo(projectLocation, fileName);

	QFile file(fInfo.filePath());

	if(!file.open(IO_WriteOnly))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to plugin header file(.h).").latin1());

	QTextStream stream(&file);

	QString pluginMacro(name.c_str());
	pluginMacro = "__" + pluginMacro.upper() + "_PLUGIN_H_";

	stream	<<"#ifndef " <<pluginMacro <<endl
		<<"#define " <<pluginMacro <<endl
		<<endl
		<<"#include <TViewAbstractPlugin.h>" <<endl
		<<endl
		<<"//Qt include files" <<endl
		<<"#include <qobject.h>" <<endl
		<<endl
		<<"class " <<className.c_str() <<": " <<"public QObject, public TViewAbstractPlugin" <<endl
		<<"{" <<endl
		<<"	Q_OBJECT" <<endl
		<<endl
		<<"public:" <<endl
		<<"	/**	\\brief Constructor." <<endl
		<<"	 *	\\param params Plugin parameters." <<endl
		<<"	 */" <<endl
		<<"	" <<className.c_str() <<"(PluginParameters* params);" <<endl 
		<<endl 
		<<"	/**	\\brief Destructor." <<endl
		<<"	 */" <<endl
		<<"	~" <<className.c_str() <<"();" <<endl
		<<endl
		<<"	/**	\\brief Initializer method." <<endl
		<<"	 */" <<endl
		<<"	void init();" <<endl
		<<endl
		<<"	/**	\\brief Finalizer method." <<endl
		<<"	 */" <<endl
		<<"	void end();" <<endl
		<<"};" <<endl
		<<endl
		<<"#endif" <<" //" <<pluginMacro;

	file.close();
}

void TVPLGCodeGenerator::generatePluginCodeHeader(QDir projectLocation, const string& name, const string& description, const string& vendorName, 
			const string& vendorMail, const string& vendorHP, vector<string>& headerNames)
{
	QString fileName = QString(name.c_str()).lower();
	fileName += "PluginCode.h";

	headerNames.push_back(fileName.latin1());

	if(!projectLocation.cd("include"))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to enter include folder.").latin1());

	QFileInfo fInfo(projectLocation, fileName);

	QFile file(fInfo.filePath());

	if(!file.open(IO_WriteOnly))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to plugin code header file(.h).").latin1());

	QTextStream stream(&file);

	stream <<endl <<"#include <spl.h>" <<endl
		<<endl
		<<"#if SPL_PLATFORM == SPL_PLATFORM_WIN32" <<endl
		<<"	// Include windows.h - Needed for Windows projects." <<endl
		<<"	#include <windows.h>" <<endl
		<<"#endif" <<endl
		<<endl
		<<"#include <PluginParameters.h>" <<endl
		<<endl
		<<"//" <<endl
		<<"// Define standard export symbols for Win32." <<endl
		<<"// Note: You don't have to remove this macro for other platforms." <<endl
		<<"//" <<endl
		<<"SPL_DEFINE_PLUGIN_EXPORTS();" <<endl
		<<endl
		<<"SPL_DEFINE_PLUGIN_INFO( " <<endl
		<<"1,	///< The build number. "<<endl
		<<"1,	///< The major version (e.g. 1.xx)." <<endl
		<<"0,	///< The minor version (e.g. 0.10)." <<endl
		<<"true,	///< Does this plugin show its arguments to the public?" <<endl
		<<"\"" <<name.c_str() <<"\",	///< The plugin's name." <<endl
		<<"\"" <<vendorName.c_str() <<"\",	///< The plugin's vendor." <<endl
		<<"\"" <<description.c_str() <<"\",	///< The plugin's general description." <<endl
		<<"PLUGINPARAMETERS_VERSION,	///< The expected plugin parameters version." <<endl
		<<"\"" <<vendorHP.c_str() <<"\",	///< The plugin vendor's homepage." <<endl
		<<"\"" <<vendorMail.c_str() <<"\",	///< The plugin vendor's email address."<<endl
		<<"\"TerraViewPlugin\" );	///< The plugin's UUID, must be \"TerraViewPlugin\"." <<endl
		<<endl
		<<"//" <<endl
		<<"// Define a standard DllMain() function for Win32 plugin DLLs." <<endl
		<<"// Note: You haven't to delete or comment out this function for other platforms." <<endl
		<<"// SPL automatically detects the current platform and removes unused/unwanted code" <<endl
		<<"// behind the macros." <<endl
		<<"//" <<endl
		<<"SPL_DEFINE_PLUGIN_DLLMAIN();" <<endl
		<<endl
		<<"//" <<endl
		<<"// We don't want to change the default get information method, so simply" <<endl
		<<"// use the default implementation." <<endl
		<<"//" <<endl
		<<"SPL_IMPLEMENT_PLUGIN_GETINFO();"<<endl;

	file.close();
}

void TVPLGCodeGenerator::generatePluginSource(QDir projectLocation, const string& name, const string& className, vector<string>& srcNames)
{
	QString fileName = QString(className.c_str()).lower();
	fileName += ".cpp";

	srcNames.push_back(fileName.latin1());

	if(!projectLocation.cd("src"))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to enter source folder.").latin1());

	QFileInfo fInfo(projectLocation, fileName);

	QFile file(fInfo.filePath());

	if(!file.open(IO_WriteOnly))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to plugin source file(.cpp).").latin1());

	QTextStream stream(&file);

	stream <<"#include <" <<QString(className.c_str()).lower() <<".h>" <<endl
		<<endl
		<<className.c_str() <<"::" <<className.c_str() <<"(PluginParameters* params):" <<endl
		<<"QObject()," <<endl
		<<"TViewAbstractPlugin(params)" <<endl
		<<"{" <<endl
		<<"	loadTranslationFiles(\"" + QString(name.c_str()).lower() + "_\");" <<endl
		<<"}" <<endl
		<<endl
		<<className.c_str() <<"::~" <<className.c_str() <<"()" <<endl
		<<"{" <<endl
		<<"	end();" <<endl
		<<"}" <<endl
		<<endl
		<<"void " <<className.c_str() <<"::" <<"init()" <<endl
		<<"{" <<endl
		<<"	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!" <<endl
		<<"}" <<endl
		<<endl
		<<"void " <<className.c_str() <<"::" <<"end()" <<endl
		<<"{" <<endl
		<<"}" <<endl;

	file.close();
}

void TVPLGCodeGenerator::generatePluginCodeSrc(QDir projectLocation, const string& name, const string& className, vector<string>& srcNames)
{
	QString fileName = QString(name.c_str()).lower();
	fileName += "PluginCode.cpp";

	srcNames.push_back(fileName.latin1());

	if(!projectLocation.cd("src"))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to enter src folder.").latin1());

	QFileInfo fInfo(projectLocation, fileName);

	QFile file(fInfo.filePath());

	if(!file.open(IO_WriteOnly))
		throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to plugin code source file(.h).").latin1());

	QTextStream stream(&file);

	stream <<"#include <" << QString(name.c_str()).lower() <<"PluginCode.h>" <<endl
		<<endl
		<<"#include <" <<QString(className.c_str()).lower() <<".h>" <<endl
		<<endl
		<<"static " <<className.c_str() <<"* "<< QString(className.c_str()).lower() <<"_plg = 0;" <<endl
		<<endl
		<<"// This function will be executed at plugin's loading time." <<endl
		<<"SPL_PLUGIN_API bool SPL_INIT_NAME_CODE( slcPluginArgs* a_pPluginArgs )" <<endl
		<<"{" <<endl
		<<"	// Extracts plugin parameters" <<endl
		<<"	void* arg_ptrs[ 1 ];" <<endl
		<<endl
		<<"	if( a_pPluginArgs->GetArg( 0, arg_ptrs ) == 0 )" <<endl
		<<"	{" <<endl
		<<"		PluginParameters* params = ( PluginParameters* ) arg_ptrs[ 0 ];" <<endl
		<<endl
		<<"		// Checking the TerraView plugin interface compatibility" <<endl
		<<"		if( params->getVersion() == PLUGINPARAMETERS_VERSION )" <<endl
		<<"		{" <<endl
		<<"			// Loads the plugin translation file" <<endl
		<<"			"<<QString(className.c_str()).lower() <<"_plg = new " << className.c_str() <<"(params);" <<endl
		<<endl
		<<"			"<<QString(className.c_str()).lower() <<"_plg->init();" <<endl
		<<"		}" <<endl
		<<"	}" <<endl
		<<endl
		<<"	return true;" <<endl
		<<"}" <<endl   
		<<endl
		<<"// This function will be executed at plugin's unloading time." <<endl
		<<"SPL_PLUGIN_API bool SPL_SHUTDOWN_NAME_CODE( slcPluginArgs* a_pPluginArgs )" <<endl
		<<"{" <<endl
		<<"	delete " <<QString(className.c_str()).lower() <<"_plg;" <<endl
		<<"	" <<QString(className.c_str()).lower() <<"_plg = 0;" <<endl
		<<endl
		<<"	return true;" <<endl
		<<"}";
}
