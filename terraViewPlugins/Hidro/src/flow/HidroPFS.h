#ifndef HIDRO_PFS_H
#define HIDRO_PFS_H

// Melhor usar o define que cada caminho guardar um Double
#define BORDER_PATH_GRADIENT_ 0.0001

#include <HidroMatrix.hpp>

#include<vector>
#include<queue>

class PathTree
{
public:
  // Contrutor padrão  
  PathTree( unsigned int x, unsigned int y );

  // destrutor
  ~PathTree();

  // Coordenadas do No
  unsigned int x_;
  unsigned int y_;

  // Ponteiros para Pai e filhos
  PathTree* father_;
  std::vector<PathTree*> children_; // usei vector porque na maioria das vezes vai ter um filho só  
};

class Path
{
public:
  // Contrutor padrão
  Path();

  // Contrutor
  Path( unsigned int x, unsigned int y, PathTree* father, double altimetria, unsigned int length );

  // Sobrecarga do operador < (less than) para utilizar a priority_queue
  bool operator<( const Path &rightSide ) const;   

  // Coordenadas atuais
  unsigned int x_;
  unsigned int y_;

  // Quem é o pai. Precisa saber isso para colocar na árvore de caminhos.
  PathTree* father_;  

  // Altimetria atual
  double altimetria_;

  // Tamanho do caminho
  unsigned int length_;  
};

class HidroPFS
{
public:
  HidroPFS( HidroMatrix<double>& dem );

  // Soluciona um fosso
  bool HidroPFS::solvePit( unsigned int x, unsigned int y );

  // Adiciona um no na árvore de caminhos
  PathTree* treeAdd( PathTree* father, unsigned int x, unsigned int y );

  // Visita um no
  void visit( unsigned int x, unsigned int y );  

  // Ajusta o DEM
  void fitDEM();

  // Verrifica se é borda, considera celulas vizinhas a Dummy como borda
  bool isBorder( unsigned int x, unsigned int y );
  
  // -- Membros
  Path priority_path_;  
  priority_queue< Path > paths_;
  HidroMatrix<double>& dem_;  
  vector< pair<unsigned int, unsigned int> > visited_;
  deque< pair<unsigned int, unsigned int> > solution_path_;
  double pit_altimetria_;

  // Priority Tree (Árvore de caminhos)
  PathTree* tree_root_;
  PathTree* tree_leaf_;  

  // -- Raster Limits
  unsigned int dem_columns_;
  unsigned int dem_lines_;  
};

#endif // HIDRO_PFS_H