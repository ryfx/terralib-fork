
#ifndef IPOPPLUGINWINDOW_H
#define IPOPPLUGINWINDOW_H

///Include necessarior
#include <IpopForm.h>
#include <PluginParameters.h>
#include <TeSharedPtr.h>

//STL include files
#include <vector>

///Classe IpopPluginWindow extensao da interface que possui as variaveis e
///funcoes contidas para o funcionamento da interface
class IpopPluginWindow : public ipop
{

Q_OBJECT

public:
	//Funcoes de criacao e destruicao da classe
	IpopPluginWindow(QWidget* parent);
	~IpopPluginWindow();
	//Funcao para preenchimento do combo de matrizes com as matrizes de proximidade existente
	bool fillProxMatrixCombo(const QString& temaName);
    //Funcao para definir como padrao a matriz de proximidade escolhida pelo usuario
	void setDefaultMatrix(const QString& temaName);

protected:
	//Variavel para inicializacao do plugin, contem os dados emitidos pelo terraview ao abrir o plugin
	PluginParameters	plugIngParams_;
	//Variavel para conexcao do plugin com o banco de dados
	TeDatabase*			localDb_;
	//Vetor de saida
	vector<string>		result_;
	//Mapa que contem os indexes das matrizes de proximidade existentes no banco
	map<int, int>		comboIndex_;
	//Variavel de controle para testar se uma vista ja foi selecionada ao abrir o plugin
	bool				control_;
	//Variavel com o id da vista selecionada inicialmente
	int					ViewId_;



public slots:
	//Slot para carregar no combo de tabela as tabelas de um determinado tema
	void temaComboBoxClickSlot(const QString&);
	//Slot para sair da interface
	void CloseSlot();
	//Slot para abilitar/desabilitar o combo de matrizes acordo com a escolha do usuario
	void Enable_matrixSlot();
	//Slot para executar a funcao Ipop quando o butao ok for acionado
    void okButtonClickSlot();
	void helpButtonSlot();
    //Slot para carregar no combo de atribuitos as colunas de um determinado tabela
    void tableComboBoxClickSlot(const QString&);
	//Metodo para criacao da conexcao do plugin com o banco e outras inicializacoes necessarias
	void updateForm(PluginParameters* pluginParams);
	//Metodo para esconder a interface
	void hideEvent(QHideEvent* );
	
};

#endif 


