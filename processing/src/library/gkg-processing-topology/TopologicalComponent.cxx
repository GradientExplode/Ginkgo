#include <gkg-processing-topology/TopologicalComponent.h>
#include <gkg-core-exception/Exception.h>


#define CCExist  101
#define CCTooFar 102


gkg::TopologicalComponent::TopologicalComponent( 
                          gkg::Neighborhood3d::Type type,
                          gkg::TopologicalComponent::Adjacency adjacency,
                          gkg::TopologicalComponent::Connectivity connectivity )
{

  try
  {

    switch ( adjacency )
    {

      case gkg::TopologicalComponent::Adjacency3d_6:
        _adjacency = 6;
        break;

      case gkg::TopologicalComponent::Adjacency3d_18:
        _adjacency = 18;
        break;

      case gkg::TopologicalComponent::Adjacency3d_26:
        _adjacency = 26;
        break;

    }

    switch ( connectivity )
    {

      case gkg::TopologicalComponent::Connectivity3d_6:
        _connectivity = 6;
        break;

      case gkg::TopologicalComponent::Connectivity3d_18:
        _connectivity = 18;
        break;

      case gkg::TopologicalComponent::Connectivity3d_26:
        _connectivity = 26;
        break;

    }

    std::list< gkg::Vector3d< int32_t > > neighbors = 
      gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                       gkg::Neighborhood3d::Neighborhood3d_26 );

    int32_t x, y, z;

    for ( y = 0; y < 27; y++ )
    {

      for ( x = 0; x < 26; x++ )
      {

        _relation[ y ][ x ] = 0;

      }

    }

    neighbors.push_front( gkg::Vector3d< int32_t >( 0, 0, 0 ) );

    int32_t r = 0;
    int32_t* neighborCountPtr = _neighborCount;
    std::list< gkg::Vector3d< int32_t > >::iterator
      n1 = neighbors.begin(),
      n1e = neighbors.end();

    while ( n1 != n1e )
    {

      x = n1->x;
      y = n1->y;
      z = n1->z;

      int32_t j = 0;
      int32_t* relation = _relation[ r ];

      *neighborCountPtr = 0;

      std::list< gkg::Vector3d< int32_t > >::iterator
        n2 = neighbors.begin(),
        n2e = neighbors.end();

      while ( n2 != n2e )
      {

        if ( ( std::abs( x - n2->x ) <= 1 ) &&
             ( std::abs( y - n2->y ) <= 1 ) &&
             ( std::abs( z - n2->z ) <= 1 ) )
        {

          if ( ( std::abs( x - n2->x ) +
                 std::abs( y - n2->y ) +
                 std::abs( z - n2->z ) ) == 1 )
          {

            *relation++ = j;
            *neighborCountPtr += 1;

          }

        }

        j++;
        ++n2;

      }

      if ( type == gkg::Neighborhood3d::Neighborhood3d_18 ||
           type == gkg::Neighborhood3d::Neighborhood3d_26 )
      {

        int32_t j = 0;
        n2 = neighbors.begin();
        n2e = neighbors.end();

        while ( n2 != n2e )
        {

          if ( ( std::abs( x - n2->x ) <= 1 ) &&
               ( std::abs( y - n2->y ) <= 1 ) &&
               ( std::abs( z - n2->z ) <= 1 ) )
          {

            if ( ( std::abs( x - n2->x ) +
                   std::abs( y - n2->y ) +
                   std::abs( z - n2->z ) ) == 2 )
            {

              *relation++ = j;
              *neighborCountPtr += 1;

            }

          }

          j++;
          ++n2;

        }

      }

      if ( type == gkg::Neighborhood3d::Neighborhood3d_26 )
      {

        int32_t j = 0;
        n2 = neighbors.begin();
        n2e = neighbors.end();

        while ( n2 != n2e )
        {

          if ( ( std::abs( x - n2->x ) <= 1 ) &&
               ( std::abs( y - n2->y ) <= 1 ) &&
               ( std::abs( z - n2->z ) <= 1 ) )
          {

            if ( ( std::abs( x - n2->x ) +
                   std::abs( y - n2->y ) +
                   std::abs( z - n2->z ) ) == 3 )
            {

              *relation++ = j;
              *neighborCountPtr += 1;

            }

          }

          j++;
          ++n2;

        }

      }

      neighborCountPtr++;
      r++;
      ++n1;

    }

  }
  GKG_CATCH( "gkg::TopologicalComponent::TopologicalComponent("
             "gkg::Neighborhood3d::Type type,"
             "gkg::TopologicalComponent::Adjacency adjacency,"
             "gkg::TopologicalComponent::Connectivity connectivity )" );


}


gkg::TopologicalComponent::~TopologicalComponent()
{
}


int32_t
gkg::TopologicalComponent::getNumberOfComponents( int32_t* neighborhood )
{

  int32_t point;
  int32_t* XPtr = neighborhood;

  for ( point = _connectivity + 1; point--; XPtr++ )
  {

    if ( *XPtr )
    {

      *XPtr = CCExist;

    }

  }

  for ( point = 26 - _connectivity; point--; XPtr++ )
  {

    if ( *XPtr )
    {

      *XPtr = CCTooFar;

    }

  }

  return getNumber( neighborhood );

}


int32_t gkg::TopologicalComponent::getNumberOfComponentsForComplement( 
                                                         int32_t* neighborhood )
{

  int32_t point;
  int32_t* XPtr = neighborhood;

  if ( *XPtr )
  {

    *XPtr = CCExist;

  }

  for ( XPtr++, point = _connectivity; point--; XPtr++ )
  {

    if ( !*XPtr )
    {

      *XPtr = CCExist;

    }

  }

  for ( point = 26 - _connectivity; point--; XPtr++ )
  {

    if ( !*XPtr )
    {

      *XPtr = CCTooFar;

    }

  }

  return getNumber( neighborhood );

}


int32_t 
gkg::TopologicalComponent::getNumber( int32_t* neighborhood )
{

  int32_t point, componentCount = 0;
  int32_t list[ 27 ];
  int32_t* listPtr = list;
  int32_t* XPtr = neighborhood + 1;

  for ( point = 1; point <= _adjacency; point++ )
  {

    if ( (*XPtr++) == CCExist )
    {

      componentCount++;
      *listPtr++ = point;
      neighborhood[ point ] = componentCount;

      do
      {

        int32_t p = *(--listPtr);
        int32_t n, nMax = _neighborCount[ p ];

        for ( n = 0; n < nMax; n++ )
        {

          int32_t currentNeighbor = _relation[ p ][ n ];

          if ( neighborhood[ currentNeighbor ] == CCExist )
          {

            *listPtr++ = currentNeighbor;
            neighborhood[ currentNeighbor ] = componentCount;
            
          }

        }

      }
      while ( listPtr != list );

    }

  }

  return componentCount;

}


#undef CCExist
#undef CCTooFar
