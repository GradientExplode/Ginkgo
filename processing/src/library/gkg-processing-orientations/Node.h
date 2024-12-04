#ifndef _gkg_processing_orientations_Node_h_
#define _gkg_processing_orientations_Node_h_


#include <gkg-core-cppext/StdInt.h>
#include <list>
#include <string>
#include <iostream>


namespace gkg
{


class TreeOrientation;


class Node
{

  public:

    Node( int32_t orientationCount,
          bool avoidSymmetricOrientations = true );
    Node( const std::string& pattern,
          bool avoidSymmetricOrientations = true );
    ~Node();

    const std::list< Node* >& getChildren() const;
    const std::list< TreeOrientation* >& getOrientations() const;
  
    void addChild( Node* node );
    void addOrientation( TreeOrientation* orientation );
 
    int32_t getOrientationCount() const;
    int32_t setWeight( int32_t depth, int32_t probabilityFactor );
    int32_t getWeight() const;

    float getGlobalEnergy( float multiplicator ) const;
    float getEnergy( const Node& other ) const;
    float getEnergy( const TreeOrientation& orientation ) const;
  
    void setForces( float mult, float coef = 1 );
    void addForces( const Node& other, float coef );
    void addForces( const TreeOrientation& orientation, float coef );
    void resetForce();

    Node* getRandomNode();

    void move( float step );
    void undo();

  private:

    void analyzePattern( const std::string& pattern,
                         bool avoidSymmetricOrientations );
    bool firstElementIsNode( std::string& pattern,
                             std::string& subPattern ) const;
    bool firstElementIsOrientation( std::string& pattern ) const;
    bool firstElementIsDigit( std::string& pattern,
                              int32_t& orientationCount ) const;


    std::list< TreeOrientation* > _orientations;
    std::list< Node* > _children;
    int32_t _ownWeight;
    int32_t _childrenWeight;

};


std::ostream& printCpp( std::ostream& os, const gkg::Node& thing );

}


namespace std
{


ostream& operator << ( ostream& os, const gkg::Node& thing );


}


#endif
