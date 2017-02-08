#include <geodma_colors.h>

geodma_colors::geodma_colors()
{
  /* Colors imported from http://colorbrewer2.org/ */

  string colors = "0 0 0 ; 240 240 240 ; 84 48 5 ; 140 81 10 ; 191 129 45 ; 223 129 125 ; 246 232 195 ; 199 234 229 ; 128 205 193 ; 53 151 143 ; 1 102 94 ; 0 60 48 ; 165 0 38 ; 215 48 39 ; 244 109 67 ; 253 174 97 ; 254 224 144 ; 255 255 191 ; 224 243 248 ; 171 217 233 ; 116 173 209 ; 69 117 180 ; 49 54 149 ; 255 0 0 ; 0 255 0 ;  0 0 255 ; 0 255 255 ; 255 255 0 ; 255 0 255 ; 184 115 51 ; 217 135 25 ; 133 99 99 ; 217 217 25 ; 207 181 59 ; 205 127 50 ; 230 232 250 ; 192 192 192 ; 181 166 66 ; 140 120 83 ; 166 125 61 ; 84 84 84 ; 35 107 142 ; 237 248 251 ; 178 226 226 ; 102 194 164 ; 44 162 95 ; 0 109 44 ; 254 240 217 ; 253 204 138 ; 252 141 89 ; 227 74 51 ; 179 0 0 ; 242 240 247 ; 203 201 226 ; 158 154 200 ; 117 107 177 ; 84 39 143 ; 237 248 233 ; 186 228 179 ; 116 196 118 ; 49 163 84 ; 0 109 44 ;";
  istringstream color_stream( colors );
  int r, g, b;
  string none;
  while( color_stream ) 
  { 
    color_stream >> r; 
    color_stream >> g; 
    color_stream >> b;
    color_stream >> none;
    R.push_back( r );
    G.push_back( g );
    B.push_back( b );
  }
}

geodma_colors::~geodma_colors()
{
}

unsigned geodma_colors::get_size()
{
  return R.size();
}

int geodma_colors::get_R(unsigned i)
{
  if ( i < R.size() )
    return R[i];
  return 0;
}

int geodma_colors::get_G(unsigned i)
{
  if ( i < G.size() )
    return G[i];
  return 0;
}

int geodma_colors::get_B(unsigned i)
{
  if ( i < B.size() )
    return B[i];
  return 0;
}
