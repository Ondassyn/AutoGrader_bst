
#include "set.h"



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


// Used by copy constructor and assignment:

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

// Used by destructor:

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



size_t log_base2( size_t s ) {
    size_t i = 0;
    if (s == 0) {
        return i;
    } else {
        while (s != 1) {
            s = s / 2;
            i++;
        }
        return i;
    }
}
bool equal( const std::string& s1, const std::string& s2 ) {
    if (s1.length() != s2.length()) {
        return false;
    } else {
        size_t i = 0;
        for (char s : s1) {
            if (tolower(s) != tolower(s2[i])) {
                return false;
            }
            i++;
        }
        return true;
    }
}

bool before( const std::string& s1, const std::string& s2 )
{
    size_t i=0;
    while(tolower(s1[i])==tolower(s2[i]))
    {
        ++i;
    }
    if(tolower(s1[i]) < tolower(s2[i]))
    {
        return true;
    }
    else
    {
        return false;
    }

}

const treenode* find( const treenode* n, const std::string& el){
    if(n==NULL)
    {
        return nullptr;
    }
    else if (equal(n ->val,el))
    {
        return n;
    }
    else if (before(n ->val, el))
    {
        return find(n ->right,el);
    }
    else if (!before(n ->val,el))
    {
        return find(n ->left, el);
    }
}

treenode** find( treenode** n, const std::string& el ){
    if(*n== nullptr||equal((*n)->val,el))
    {
        return n ;

    }else if (before((*n)->val,el))
    {
        return find( &((*n)->right),el);
    }
    else if (!before((*n)->val,el))
    {
        return find( &((*n)->left),el);
    }
}

// Insert n at the right most position in into:

void rightinsert( treenode** into, treenode* n )
{
    if(((*into)->right)!=NULL)
    {
        rightinsert(&(*into)->right, n);
    } else (*into)->right=n;
}

//How to find size was understood from:
//https://www.geeksforgeeks.org/write-a-c-program-to-calculate-size-of-a-tree/
size_t size( const treenode* n )
{
    if(n==NULL)
    {
        return 0;
    }
    else
    {
        return (1+size(n->left)+size(n->right));
    }
}
//How to find height was understood from:
// https://www.geeksforgeeks.org/write-a-c-program-to-find-the-maximum-depth-or-height-of-a-tree/
size_t height( const treenode* n )
{
    if(n==NULL)
    {
        return 0;
    }
    else
    {
        size_t right_height=height(n->right);
        size_t left_height=height(n->left);
        if(right_height>left_height)
        {
            return 1+right_height;
        } else
        {
            return  1+left_height;
        }

    }
}


bool set::insert( const std::string& el )
{
    if(find(tr,el))
    {
        //std::cout<<"false"<<std::endl;
        return false;
    }else if (!find(tr,el))
    {
        //std::cout<<"True"<<std::endl;
        treenode **NewNode=find(&tr,el);
        * NewNode = new treenode(el);

        return true;
    }
}

bool set::contains( const std::string& el ) const
{
    return find( tr, el ) != nullptr;
}

bool set::remove( const std::string& el )
{
    if(!contains(el))
    {
        return false;
    }
    else{
        treenode**N=find(&tr,el);
        if(&((*N)->right)==NULL&&&((*N)->left)==NULL)
        {
            delete *N;
            *N = nullptr;

        }
        else if((*N)->left==NULL)
        {
            treenode *temp = *N;
            (*N) = (*N)->right;
            delete temp;
        }
        else if(((*N)->right)==NULL)
        {
            treenode *temp = *N;
            (*N) = (*N)->left;
            delete temp;
        }
        else{
            rightinsert( &((*N)->left), (*N)->right );
            treenode *temp = (*N);
            (*N) = (*N)->left;
            delete temp;

        }
        return true;
    }



}


void set::checksorted( ) const
{
    ::checksorted( tr, nullptr, nullptr );
    // :: are needed because the other checksorted is not in the class.
}


std::ostream& set::print( size_t indent, std::ostream& out ) const
{
    ::print( out, tr, indent );
    return out;
}

