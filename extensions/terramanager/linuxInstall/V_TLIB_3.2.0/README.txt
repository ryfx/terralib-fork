Nota de Instala��o no Linux para a Vers�o 3.2.0 da TerraLib
===========================================================



1. Descompacte o arquivo (.tar.gz) contendo
os scripts de instala��o em um diret�rio apropriado,
como por exemplo:
/root/terraphpinstall.

2. Os seguintes arquivos (scripts) dever�o
ser encontrados neste diret�rio:
- te_main_install;
- te_install_terralib;
- te_install_terramanager;
- te_install_terraphp;


3. Ajuste o privil�gio de execu��o para
os arquivos de script se necess�rio (chmod 700 te_*).

4. Voc� dever� estar logado como usu�rio
root para realizar a instala��o.

5. Executando o comando:
./te_main_install

ser� exibida a seguinte tela:

"
Selecione uma da opcoes de instalacao abaixo (ou digite 0 para sair):

1 => Instalar TerraLib
2 => Instalar TerraManager
3 => Instalar TerraPHP
0 => Sair 
"

6. Este script permite que voc� instale a TerraLib
e a extens�o TerraPHP.

7. Para instalar a extens�o TerraPHP,
voc� precisar� ter instalado os seguintes
softwares em seu sistema:
- biblioteca gd (gd-2.0.33)
- Apache (1.3.X ou superior)
- PHP 4.3.11 (ou qualquer outro da s�rie 4.4.x) 
- Biblioteca MySQL client (libmysqlclient) se for instalar com
suporte a MySQL

8. caso da instala��o do TerraManager,
que � utilizado pela extens�o TerraPHP,
� necess�rio indicar quais os drivers de bancos
de dados dever�o estar presentes na extens�o TerraPHP.
Edite o arquivo te_install_terramanager e
configure a seguinte vari�vel:
DATABASE_2_USE="HAS_POSTGRESQL"

Caso deseje instalar outros drivers,
pode-se usar espa�o para separar uma
das seguintes op��es:
HAS_POSTGRESQL, HAS_OCI, HAS_MYSQL.

Ex: DATABASE_2_USE="HAS_POSTGRESQL HAS MYSQL"

8. Ap�s configurar o script de instala��o
do TerraManager (te_install_terramanager),
podemos iniciar a instala��o dos m�dulos TerraLib.

9. Possivelmente, voc� vai querer gerar
a biblioteca TerraLib em modo release
para obter o maior desempenho poss�vel no TerraPHP.
Sendo assim, edite os arquivos ".pro"
dentro da pasta terralibx e substitua
os modificadores "debug" por "release"
nestes arquivos. Depois, utilize o qmake
para gerar os novos Makefiles.
Ex:
qmake terralib.pro
qmake shapelib.pro

10. Inicie a instala��o (./te_main_install).

11. D�vidas ou sugest�es, entre em contato com a equipe TerraLib (http://www.terralib.org).


Obs.:

1. N�s recomendamos que a instala��o do TerraPHP seja
feita como um m�dulo externo. Isso facilita a
manuten��o e upgrade da extens�o.

2. Durante a instala��o do TerraPHP, se voc� optou
por instalar o PHP e o script de configura��o do PHP
n�o foi executado com sucesso, verifique
se o pacote do lex (ou flex) est� instalado no seu sistema.
