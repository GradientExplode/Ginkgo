#ifndef _gkg_processing_orientations_TreeOrientation_h_
#define _gkg_processing_orientations_TreeOrientation_h_


#include "Orientation.h"


namespace gkg
{


class Node;


class TreeOrientation : public Orientation
{

  public:

    TreeOrientation( bool avoidSymmetricOrientations = true );
    virtual ~TreeOrientation();

    void addForce( const Node& node, float weight );

};


}


#endif
