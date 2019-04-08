
#include "set.h"

// From previous task:

bool equal( const std::string& s1, const std::string& s2 )
{
   if( s1. size( ) != s2. size( ))
      return false;
   else
   {
      for( size_t i = 0; i != s1. size( ); ++ i )
      {
         if( tolower( s1[i] ) != tolower( s2[i] ))
            return false; 
      } 
      return true;
   }
}


// Must be new written:

bool before( const std::string& s1, const std::string& s2 )
{
   for( size_t i = 0; i < s1. size( ) && i < s2. size( ); ++ i )
   {
      char c1 = tolower( s1[i] );
      char c2 = tolower( s2[i] );
      if( c1 < c2 ) return true;
      if( c1 > c2 ) return false; 
   }
   return s1.size() < s2.size(); 
}


void print( std::ostream& out, const treenode* n, size_t indent )
{

   for( size_t i = 0; i != indent; ++ i )
      out << "|  "; 
   if(n)
   {
      out << ( n -> val ) << "\n";
      print( out, n -> left, indent + 1 );
      print( out, n -> right, indent + 1 ); 
   }
   else
      out << "#\n"; 
}


const treenode* find( const treenode* n, const std::string& el )
{
   while( n != nullptr )
   {
      if( equal( el, n -> val ))
         return n;

      if( before( el, n -> val ))
         n = n -> left;
      else
         n = n -> right;
   }
   return nullptr; 
}


treenode** find( treenode** n, const std::string& el )
{
   while( *n != nullptr )
   {
      if( equal( el, (*n) -> val ))
         return n;

      if( before( el, (*n) -> val ))
         n = & ( (*n) -> left );
      else
         n = & ( (*n) -> right );
   }
   return n; 
}


// Insert n at the first treenode that has an empty right child:

void rightinsert( treenode** into, treenode* n )
{
   while( (*into) -> right ) 
      into = & (*into) -> right; 

   (*into) -> right = n; 
}


void checksorted( 
          treenode* n, 
          const std::string* lowerbound, 
          const std::string* upperbound )
{
   while(n) 
   {
      if( lowerbound && !before( *lowerbound, n -> val ))
      {
         std::cout << "value " << ( n -> val );
         std::cout << " is not above lower bound " << *lowerbound << "\n";
         std::abort( );
      }

      if( upperbound && !before( n -> val, *upperbound ))
      {
         std::cout << "value " << ( n -> val );
         std::cout << "is not below upperbound " << *upperbound << "\n";
         std::abort( );
      }

      checksorted( n -> left, lowerbound, &( n -> val ));
      lowerbound = & ( n -> val );
      n = n -> right;
   }
} 


void deallocate( treenode* n )
{
   while(n)
   {
      deallocate( n -> left );
      treenode* bye = n;
      n = n -> right;
      delete bye;
   }
}


void writecopy( treenode** to, treenode* from )
{
   while( from )
   {
      *to = new treenode{ from -> val };

      writecopy( & (*to) -> left, from -> left );
      to = & (*to) -> right;
      from = from -> right;
   }
}


size_t size( const treenode* n )
{
   size_t s = 0;
   while( n )
   {
      ++ s; 
      s += size( n -> left );
      n = n -> right;
   }
   return s;
}


size_t height( const treenode* n )
{
   size_t h = 0;  // Current height.
   size_t mh = 0; // Current max height.

   while( n )
   {
      size_t m = 1 + height( n -> left );
      if( m + h > mh )
         mh = m + h;
      n = n -> right; 
      ++ h;          
   }

   if( h > mh )
      mh = h;
   return mh;
}


size_t log2( size_t s )
{
   size_t l = 0;
   while( s > 1 )
   {
      s /= 2;
      l ++;
   }
   return l;
}

 
bool set::contains( const std::string& el ) const 
{
   return find( tr, el ) != nullptr; 
}


bool set::insert( const std::string& el )
{
   treenode** n = find( &tr, el );
   if( *n )   
      return false;
   else
   {
      *n = new treenode( el ); 
      return true; 
   }
}


bool set::remove( const std::string& el )
{
   treenode** n = find( &tr, el ); 
   
   if(*n)
   { 
      if( (*n) -> left == nullptr )
      {
         treenode* bye = *n;
         *n = (*n) -> right;
         delete bye;
         return true; 
      }

      if( (*n) -> right == nullptr )
      {
         treenode* bye = *n;
         *n = (*n) -> left;
         delete bye;
         return true; 
      }
      
      treenode* bye = *n;
      rightinsert( & (*n) -> left, (*n) -> right );
      *n = (*n) -> left;
      delete bye; 
      return true; 
   }
   else
      return false;
}


void set::checksorted( ) const
{
   ::checksorted( tr, nullptr, nullptr );
}


std::ostream& set::print( size_t indent, std::ostream& out ) const
{
   ::print( out, tr, indent );
   return out;
}

