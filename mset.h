
#ifndef MSET_INCLUDED
#define MSET_INCLUDED    1 

#include <iostream>
#include <string>
#include <vector>


bool mequal( const std::string& s1, const std::string& s2 );
bool mbefore( const std::string& s1, const std::string& s2 );


// It is good to have a look at the list-based implementation
// of stack again. This implementation is very similar.

struct mtreenode
{
   std::string val;
   mtreenode* left;
   mtreenode* right;

   mtreenode( const std::string& val )
      : val{ val },
        left{ nullptr },
        right{ nullptr }  
   { }

   mtreenode( std::string&& val )
      : val{ std::move(val) },
        left{ nullptr },
        right{ nullptr } 
   { }
};


void mdeallocate( mtreenode* n ); 
   // Deallocate complete tree n.

void mwritecopy( mtreenode** to, mtreenode* from );

const mtreenode* mfind( const mtreenode* n, const std::string& el );
mtreenode** mfind( mtreenode** n, const std::string& el );

void mrightinsert( mtreenode** into, mtreenode* n );

void mprint( std::ostream& out, const mtreenode* n, size_t indent );
   // Print the tree as tree.

size_t msize( const mtreenode* n ); 
   // Returns the total size of tree.

size_t mheight( const mtreenode* n );
   // Returns the height of the tree starting at n.

size_t mlog2( size_t s );

class mset
{
   mtreenode* tr;

public: 
   mset( ) 
      : tr{ nullptr } 
   { } 

   mset( const mset& s ) :mset()
   {
      mwritecopy( &tr, s. tr );
   }

   mset& operator = ( const mset& s ) 
   {
      if( tr != s. tr )
      {
         mdeallocate( tr );
         mwritecopy( &tr, s. tr );
      }
      return *this;
   }

   ~mset( ) { mdeallocate( tr ); }


   mset( std::initializer_list< std::string > init ):mset()
   {
      for( const auto& s : init ) 
         insert( s );
   } 
  
   const std::string& top( ) const { return tr -> val; }
      // Return element on top. The tree must be non empty.
 
   bool contains( const std::string& s ) const;

   bool insert( const std::string& s );
      // Returns true if the insertion took place.

   bool remove( const std::string& s );
      // Returns true if s was present. 

   size_t remove( const mset& s );
      // Returns number of elements removed. (that were present).
 
   size_t size( ) const { return ::msize( tr ); }
   size_t height( ) const { return ::mheight( tr ); }

   bool isempty( ) const { return ! tr; }

   void clear( ) { mdeallocate( tr ); tr = nullptr; }

   void checksorted( ) const;
      // Check if the tree is sorted, and throw an exception
      // if it is not. Call this function often to check your
      // trees. 

   std::ostream& print( size_t indent, std::ostream& out ) const;
};


inline std::ostream& operator << ( std::ostream& out, const mset& s )
{
   return s. print( 0, out );
}


#endif

