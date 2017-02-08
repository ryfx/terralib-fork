
#ifndef KFUNCTIONPLUGINWINDOW_H
#define KFUNCTIONPLUGINWINDOW_H

///Include necessarior
#include <KFunctionForm.h>
#include <k.h>
#include <k12.h>
#include <klabel.h>
#include <envelope.h>
#include <LoadParams.h>
#include <xyplotgraphic.h>
#include <PluginParameters.h>
#include <TeSharedPtr.h>

///Classe KFunctionPluginWindow extensao da interface que possui as variaveis e
///funcoes contidas para o funcionamento da interface
class KFunctionPluginWindow : public kfunction
{

Q_OBJECT

public:
	///Funcoes de criacao e destruicao da classe
	KFunctionPluginWindow(QWidget* parent);
	~KFunctionPluginWindow();
	///Variavel do grafico
	XYPlotGraphic		*gra_;

protected:
	///Variavel de cores para o grafico
	int ncols_;
	std::vector<QColor> _colors;
	///Variavel para inicializacao do plugin, contem os dados emitidos pelo terraview ao abrir o plugin
	PluginParameters	plugIngParams_;
	///Variavel para conexcao do plugin com o banco de dados
	TeDatabase*			localDb_;
	///Vetor de saida
	vector<string>		result_;
	///Variavel de controle para testar se uma vista ja foi selecionada ao abrir o plugin
	bool				control_;
	///Variavel com o id da vista selecionada inicialmente
	int					ViewId_;

public:	
	//Metodo para inicializacao da interface
	void init();
	//Metodo para inicializacao do combobox com as funcoes K existentes
	void InitializefunctionComboBox();
	//Metodo para criacao da conexcao do plugin com o banco e outras inicializacoes necessarias
	void updateForm(PluginParameters* pluginParams);
	//Metodo para esconder a interface
	void hideEvent(QHideEvent* );


public slots:
	//Slot para abilitar/desabilitar os combos de tema acordo com a escolha do usuario
	void Enable_Theme1Slot();
	void Enable_Theme2Slot();
	//Slot para executar a funcao K escolhida quando o botao ok for acionado
	void kfunctionButtonClickSlot();
	void helpButtonSlot();
};

#endif 


