#include <HidroPFS.h>

PathTree::PathTree(  unsigned int x, unsigned int y ) : 
  x_(x),
  y_(y),
  father_(0)
{}

PathTree::~PathTree()
{
  // Nao precisa apagar o pai so os filhos
  std::vector<PathTree*>::iterator i;
  for( i=children_.begin(); i!=children_.end(); ++i )
  {
    delete *i;
  }
}

Path::Path() :
  x_(0),
  y_(0),
  father_(0),
  altimetria_(0),
  length_(0)
{}

Path::Path( unsigned int x, unsigned int y, PathTree* father, double altimetria, unsigned int length ) : 
  x_(x),
  y_(y),
  father_(father),
  altimetria_(altimetria),
  length_(length)
{}

bool
Path::operator<( const Path &rightSide ) const
{
  if( this->altimetria_ == rightSide.altimetria_ )
    return this->length_ > rightSide.length_;

  return  this->altimetria_ > rightSide.altimetria_;
}

HidroPFS::HidroPFS( HidroMatrix<double>& dem ) : 
  dem_(dem),
  tree_root_(0),
  tree_leaf_(0)
{
  dem_columns_ = dem_.GetColumns() - 2;
  dem_lines_ = dem_.GetLines() - 2;
}

bool
HidroPFS::solvePit( unsigned int x, unsigned int y )
{
  // Limpa o caminho resolvido
  solution_path_.clear();

  // Define o root da árvore de caminhos
  tree_root_ = new PathTree( x, y );

  // Define o primeiro caminho
  priority_path_ = Path( x, y, tree_root_, dem_[y][x], 1 );  
  
  // O tree_leaf_ é o ultimo no adicionado na árvore de caminhos
  tree_leaf_ = tree_root_;
  
  // Adiciona o no visitado  
  visited_.push_back( pair<unsigned int,unsigned int>( x, y ) );

  // Guarda a altimetria do fosso para calcular as novas altimetrias do caminho
  pit_altimetria_ = dem_[y][x];  
  
  do
  {
    // Up    
    visit( x, y-1 );        
    
    // Up Right
    visit( x+1, y-1 );        
    
    // Right
    visit( x+1, y );
    
    // Down Right
    visit( x+1, y+1 );
    
    // Down
    visit( x, y+1 );
    
    // Down Left
    visit( x-1, y+1 );

    // Left
    visit( x-1, y );

    // Up Left
    visit( x-1, y-1 );

    // Melhor caminho
    priority_path_ = paths_.top();
    paths_.pop();
    x = priority_path_.x_;
    y = priority_path_.y_;    

    // adiciona na árvore de caminhos
    tree_leaf_ = treeAdd( priority_path_.father_, x, y );    
    
  }
  while( priority_path_.altimetria_ >= pit_altimetria_ && !isBorder( x, y ) );    

  // Esvazia os caminhos (libera memória)
  while( !paths_.empty() )
    paths_.pop();  

  // Limpa os nos visitados  
  visited_.clear();

  // Guarda o caminho solução em uma pilha para ficar na ordem correta
  tree_leaf_;
  while( tree_leaf_ != 0 )
  {
    solution_path_.push_front( pair<unsigned int,unsigned int>( tree_leaf_->x_, tree_leaf_->y_ ) );    
    tree_leaf_ = tree_leaf_->father_;
  }  
  
  // Libera a memória utilizada pela árvore de caminhos
  delete tree_root_;
  
  // fit the DEM
  fitDEM();

  return true;
}

void
HidroPFS::visit( unsigned int x, unsigned int y )
{
  // Verifica se ja foi visitado  
  for( unsigned int i=0; i<visited_.size(); i++ )
  {
    if( visited_[i].first == x )
      if( visited_[i].second == y )
        return;
  }
  
  // Marca o no como visitado  
  visited_.push_back( pair<unsigned int,unsigned int>( x, y ) );

  // Precisa usar o getElement para verificar valores dummy no DEM.
  double altimetria;
  if( dem_.getElement( x, y, altimetria ) )
  {     
    // Cria o novo caminho
    Path new_path( x, y, tree_leaf_, altimetria, priority_path_.length_ + 1 );    
    paths_.push( new_path );
  }
}

void
HidroPFS::fitDEM()
{
  // 1- Calular gradiente
  
  // gradiente quando o fosso atinge a borda do DEM
  double gradient = BORDER_PATH_GRADIENT_;

  double first_altimetria = pit_altimetria_;
  double last_altimetria = priority_path_.altimetria_;  
  if( first_altimetria > last_altimetria )
  {
    // gradiente quando achou o pixel de saída
    gradient = (first_altimetria - last_altimetria) / (solution_path_.size() - 1);    
  }

  // 2- Alterar o DEM com as novas altimetrias
  unsigned int x;
  unsigned int y;  
  double altimetria = pit_altimetria_;
  for( unsigned int i=1; i < solution_path_.size(); i++ )
  {
    x = solution_path_[i].first;
    y = solution_path_[i].second;

    // calulo a nova altimetria
    altimetria = altimetria - gradient;
    dem_[y][x] = altimetria;    
  } 
}

bool
HidroPFS::isBorder( unsigned int x, unsigned int y )
{
  if( x < 1 || x > (dem_columns_) ||
    y < 1 || y > (dem_lines_) )
  {
    return true;
  }
  else if( dem_.hasDummy() )
  {
    // Compute the start and end of window (3x3)
    unsigned int startColumn = x - 1;
    unsigned int endColumn = x + 2;
    unsigned int startLine = y - 1;
    unsigned int endLine = y + 2;
  	
    double altimetria;

	  for (unsigned int l = startLine; l < endLine; l++)
	  {				
		  for (unsigned int c = startColumn; c < endColumn; c++)
		  {
        if( !dem_.getElement(c, l, altimetria) )
        {
          // is dummy.
          return true;
        }
      }
    }
  }

  return false;
}

PathTree*
HidroPFS::treeAdd( PathTree* father, unsigned int x, unsigned int y )
{
  // Adiciona mais um no na arvore
  father->children_.push_back( new PathTree( x, y ) );
  father->children_.back()->father_ = father; 

  // ultimo no adicionado
  return father->children_.back();
}