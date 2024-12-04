#ifndef _gkg_processing_graph_EuclideanMinimumSpanningTree_h_
#define _gkg_processing_graph_EuclideanMinimumSpanningTree_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <vector>


namespace gkg
{


class EuclideanMinimumSpanningTree
{

  public:

    class Node
    {

      public:

        Node( Node* parent,
              const Vector3d< float >& position, 
              int32_t index,
              float pathDistance = 0.0f );
        Node( const Vector3d< float >& position, 
              int32_t index,
              float pathDistance = 0.0f );
        Node( const Node& other );
        virtual ~Node();

        Node& operator=( const Node& other );

        const Vector3d< float >& getPosition() const;
        int32_t getIndex() const;
        const std::vector< Node >& getChildren() const;
        float getPathDistance() const;
        bool isRoot() const; 
        bool hasChildren() const; 

        void addChild( Node* child );

      protected:

        const Node* _parent;
        Vector3d< float > _position;
        int32_t _index;
        float _pathDistance;
        bool _isRoot;
        bool _hasChildren;
        std::vector< Node > _children;

    };

    EuclideanMinimumSpanningTree(
                              const std::vector< Vector3d< float > >& positions,
                              float balancingFactor );
    virtual ~EuclideanMinimumSpanningTree();

    const std::vector< Node >& getNodes() const;

  protected:

    std::vector< Node > _nodes;


};


}


#endif
