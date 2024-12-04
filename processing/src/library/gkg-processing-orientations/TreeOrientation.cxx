#include <gkg-processing-orientations/TreeOrientation.h>
#include <gkg-processing-orientations/Node.h>


gkg::TreeOrientation::TreeOrientation( bool avoidSymmetricOrientations )
                     : gkg::Orientation( avoidSymmetricOrientations )
{
}


gkg::TreeOrientation::~TreeOrientation()
{
}


void gkg::TreeOrientation::addForce( const gkg::Node& node, float weight )
{

  std::list< gkg::Node* >::const_iterator c = node.getChildren().begin(),
                                          ce = node.getChildren().end();
  while ( c != ce )
  {

    addForce( **c, weight );
    ++ c;

  }

  std::list< gkg::TreeOrientation* >::const_iterator
    o = node.getOrientations().begin(),
    oe = node.getOrientations().end();
  while ( o != oe )
  {

    this->gkg::Orientation::addForce( **o, weight * weight );
    ++ o;

  }

}

