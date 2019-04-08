
#include "mset.h"

// From previous task:

bool mequal( const std::string& s1, const std::string& s2 )
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

bool mbefore( const std::string& s1, const std::string& s2 )
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


void mprint( std::ostream& out, const mtreenode* n, size_t indent )
{

   for( size_t i = 0; i != indent; ++ i )
      out << "|  "; 
   if(n)
   {
      out << ( n -> val ) << "\n";
      mprint( out, n -> left, indent + 1 );
      mprint( out, n -> right, indent + 1 ); 
   }
   else
      out << "#\n"; 
}


const mtreenode* mfind( const mtreenode* n, const std::string& el )
{
   while( n != nullptr )
   {
      if( mequal( el, n -> val ))
         return n;

      if( mbefore( el, n -> val ))
         n = n -> left;
      else
         n = n -> right;
   }
   return nullptr; 
}


mtreenode** mfind( mtreenode** n, const std::string& el )
{
   while( *n != nullptr )
   {
      if( mequal( el, (*n) -> val ))
         return n;

      if( mbefore( el, (*n) -> val ))
         n = & ( (*n) -> left );
      else
         n = & ( (*n) -> right );
   }
   return n; 
}


// Insert n at the first treenode that has an empty right child:

void mrightinsert( mtreenode** into, mtreenode* n )
{
   while( (*into) -> right ) 
      into = & (*into) -> right; 

   (*into) -> right = n; 
}


void mchecksorted( 
          mtreenode* n, 
          const std::string* lowerbound, 
          const std::string* upperbound )
{
   while(n) 
   {
      if( lowerbound && !mbefore( *lowerbound, n -> val ))
      {
         std::cout << "value " << ( n -> val );
         std::cout << " is not above lower bound " << *lowerbound << "\n";
         std::abort( );
      }

      if( upperbound && !mbefore( n -> val, *upperbound ))
      {
         std::cout << "value " << ( n -> val );
         std::cout << "is not below upperbound " << *upperbound << "\n";
         std::abort( );
      }

      mchecksorted( n -> left, lowerbound, &( n -> val ));
      lowerbound = & ( n -> val );
      n = n -> right;
   }
} 


void mdeallocate( mtreenode* n )
{
   while(n)
   {
      mdeallocate( n -> left );
      mtreenode* bye = n;
      n = n -> right;
      delete bye;
   }
}


void mwritecopy( mtreenode** to, mtreenode* from )
{
   while( from )
   {
      *to = new mtreenode{ from -> val };

      mwritecopy( & (*to) -> left, from -> left );
      to = & (*to) -> right;
      from = from -> right;
   }
}


size_t msize( const mtreenode* n )
{
   size_t s = 0;
   while( n )
   {
      ++ s; 
      s += msize( n -> left );
      n = n -> right;
   }
   return s;
}


size_t mheight( const mtreenode* n )
{
   size_t h = 0;  // Current height.
   size_t mh = 0; // Current max height.

   while( n )
   {
      size_t m = 1 + mheight( n -> left );
      if( m + h > mh )
         mh = m + h;
      n = n -> right; 
      ++ h;          
   }

   if( h > mh )
      mh = h;
   return mh;
}


size_t mlog2( size_t s )
{
   size_t l = 0;
   while( s > 1 )
   {
      s /= 2;
      l ++;
   }
   return l;
}

 
bool mset::contains( const std::string& el ) const 
{
   return mfind( tr, el ) != nullptr; 
}


bool mset::insert( const std::string& el )
{
   mtreenode** n = mfind( &tr, el );
   if( *n )   
      return false;
   else
   {
      *n = new mtreenode( el ); 
      return true; 
   }
}


bool mset::remove( const std::string& el )
{
   mtreenode** n = mfind( &tr, el ); 
   
   if(*n)
   { 
      if( (*n) -> left == nullptr )
      {
         mtreenode* bye = *n;
         *n = (*n) -> right;
         delete bye;
         return true; 
      }

      if( (*n) -> right == nullptr )
      {
         mtreenode* bye = *n;
         *n = (*n) -> left;
         delete bye;
         return true; 
      }
      
      mtreenode* bye = *n;
      mrightinsert( & (*n) -> left, (*n) -> right );
      *n = (*n) -> left;
      delete bye; 
      return true; 
   }
   else
      return false;
}


void mset::checksorted( ) const
{
   ::mchecksorted( tr, nullptr, nullptr );
}


std::ostream& mset::print( size_t indent, std::ostream& out ) const
{
   ::mprint( out, tr, indent );
   return out;
}

