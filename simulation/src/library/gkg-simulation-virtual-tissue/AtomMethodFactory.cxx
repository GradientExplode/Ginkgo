#include <gkg-simulation-virtual-tissue/AtomMethodFactory.h>
#include <gkg-core-exception/Exception.h>


gkg::AtomMethodFactory::AtomMethodFactory()
                       : _getAtomLutVoxelMethods( 16U, 0 ),
                         _getAtomLutVoxelSpanMethods( 16U, 0 ),
                         _belongToMethods( 16U, 0 ),
                         _distanceToMembraneMethods( 16U, 0 ),
                         _outwardNormalToMembraneMethods( 16U, 0 ),
                         _isRayCollidingWithAtomMethods( 16U, 0 )
                         
{
}


gkg::AtomMethodFactory::~AtomMethodFactory()
{
}


////////////////////////////////////////////////////////////////////////////////
// get atom lut voxel method 
////////////////////////////////////////////////////////////////////////////////

bool gkg::AtomMethodFactory::registerGetAtomLutVoxelMethod(
     uint8_t atomType,
     gkg::AtomMethodFactory::GetAtomLutVoxelMethod getAtomLutVoxelMethod )
{

  try
  {

    if ( atomType && getAtomLutVoxelMethod )
    {

      if ( _getAtomLutVoxelMethods[ ( size_t )atomType ] )
      {

        std::cerr << "gkg::AtomMethodFactory::"
                  << "registerGetAtomLutVoxelMethod for type "
                  << ( int32_t )atomType
                  << " already exists"
                  << std::endl;
        return false;

      }
      else
      {

        _getAtomLutVoxelMethods[ ( size_t )atomType ] = getAtomLutVoxelMethod;
        return true;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AtomMethodFactory::"
             "registerGetAtomLutVoxelMethod( "
             "uint8_t atomType, "
             "gkg::AtomMethodFactory::GetAtomLutVoxelMethod "
             "getAtomLutVoxelMethod )" );

}


gkg::AtomMethodFactory::GetAtomLutVoxelMethod 
gkg::AtomMethodFactory::getGetAtomLutVoxelMethod( uint8_t atomType ) const
{

  try
  {

    if ( atomType && ( ( size_t )atomType < _getAtomLutVoxelMethods.size() ) )
    {

      return _getAtomLutVoxelMethods[ ( size_t )atomType ];

    }
    else
    {

      throw std::runtime_error( "bad atom type" );

    }
    return 0;

  }
  GKG_CATCH( "gkg::AtomMethodFactory::GetAtomLutVoxelMethod "
             "gkg::AtomMethodFactory::getGetAtomLutVoxelMethod( "
             "uint8_t atomType ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// get atom lut voxel span method 
////////////////////////////////////////////////////////////////////////////////

bool gkg::AtomMethodFactory::registerGetAtomLutVoxelSpanMethod(
     uint8_t atomType,
     gkg::AtomMethodFactory::GetAtomLutVoxelSpanMethod
                                                     getAtomLutVoxelSpanMethod )
{

  try
  {

    if ( atomType && getAtomLutVoxelSpanMethod )
    {

      if ( _getAtomLutVoxelSpanMethods[ ( size_t )atomType ] )
      {

        std::cerr << "gkg::AtomMethodFactory::"
                  << "registerGetAtomLutVoxelSpanMethod for type "
                  << ( int32_t )atomType
                  << " already exists"
                  << std::endl;
        return false;

      }
      else
      {

        _getAtomLutVoxelSpanMethods[ ( size_t )atomType ] =
                                                      getAtomLutVoxelSpanMethod;
        return true;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AtomMethodFactory::"
             "registerGetAtomLutVoxelSpanMethod( "
             "uint8_t atomType, "
             "gkg::AtomMethodFactory::GetAtomLutVoxelSpanMethod "
             "getAtomLutVoxelSpanMethod )" );

}


gkg::AtomMethodFactory::GetAtomLutVoxelSpanMethod 
gkg::AtomMethodFactory::getGetAtomLutVoxelSpanMethod( uint8_t atomType ) const
{

  try
  {

    if ( atomType &&
         ( ( size_t )atomType < _getAtomLutVoxelSpanMethods.size() ) )
    {

      return _getAtomLutVoxelSpanMethods[ ( size_t )atomType ];

    }
    else
    {

      throw std::runtime_error( "bad atom type" );

    }
    return 0;

  }
  GKG_CATCH( "gkg::AtomMethodFactory::GetAtomLutVoxelSpanMethod "
             "gkg::AtomMethodFactory::getGetAtomLutVoxelSpanMethod( "
             "uint8_t atomType ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// belong to method 
////////////////////////////////////////////////////////////////////////////////

bool gkg::AtomMethodFactory::registerBelongToMethod(
     uint8_t atomType,
     gkg::AtomMethodFactory::BelongToMethod belongToMethod )
{

  try
  {

    if ( atomType && belongToMethod )
    {

      if ( _belongToMethods[ ( size_t )atomType ] )
      {

        std::cerr << "gkg::AtomMethodFactory::"
                  << "registerBelongToMethod for type "
                  << ( int32_t )atomType
                  << " already exists"
                  << std::endl;
        return false;

      }
      else
      {

        _belongToMethods[ ( size_t )atomType ] = belongToMethod;
        return true;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AtomMethodFactory::"
             "registerBelongToMethod( "
             "uint8_t atomType, "
             "gkg::AtomMethodFactory::BelongToMethod "
             "belongToMethod )" );

}


gkg::AtomMethodFactory::BelongToMethod 
gkg::AtomMethodFactory::getBelongToMethod( uint8_t atomType ) const
{

  try
  {

    if ( atomType && ( ( size_t )atomType < _belongToMethods.size() ) )
    {

      return _belongToMethods[ ( size_t )atomType ];

    }
    else
    {

      throw std::runtime_error( "bad atom type" );

    }
    return 0;

  }
  GKG_CATCH( "gkg::AtomMethodFactory::BelongToMethod "
             "gkg::AtomMethodFactory::getBelongToMethod( "
             "uint8_t atomType ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// distance to membrane method 
////////////////////////////////////////////////////////////////////////////////

bool gkg::AtomMethodFactory::registerDistanceToMembraneMethod(
     uint8_t atomType,
     gkg::AtomMethodFactory::DistanceToMembraneMethod distanceToMembraneMethod )
{

  try
  {

    if ( atomType && distanceToMembraneMethod )
    {

      if ( _distanceToMembraneMethods[ ( size_t )atomType ] )
      {

        std::cerr << "gkg::AtomMethodFactory::"
                  << "registerDistanceToMembraneMethod for type "
                  << ( int32_t )atomType
                  << " already exists"
                  << std::endl;
        return false;

      }
      else
      {

        _distanceToMembraneMethods[ ( size_t )atomType ] =
                                                       distanceToMembraneMethod;
        return true;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AtomMethodFactory::"
             "registerDistanceToMembraneMethod( "
             "uint8_t atomType, "
             "gkg::AtomMethodFactory::DistanceToMembraneMethod "
             "distanceToMembraneMethod )" );

}


gkg::AtomMethodFactory::DistanceToMembraneMethod 
gkg::AtomMethodFactory::getDistanceToMembraneMethod( uint8_t atomType ) const
{

  try
  {

    if ( atomType &&
         ( ( size_t )atomType < _distanceToMembraneMethods.size() ) )
    {

      return _distanceToMembraneMethods[ ( size_t )atomType ];

    }
    else
    {

      throw std::runtime_error( "bad atom type" );

    }
    return 0;

  }
  GKG_CATCH( "gkg::AtomMethodFactory::DistanceToMembraneMethod "
             "gkg::AtomMethodFactory::getDistanceToMembraneMethod( "
             "uint8_t atomType ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// outward normal to membrane method 
////////////////////////////////////////////////////////////////////////////////

bool gkg::AtomMethodFactory::registerOutwardNormalToMembraneMethod(
     uint8_t atomType,
     gkg::AtomMethodFactory::OutwardNormalToMembraneMethod 
                                                 outwardNormalToMembraneMethod )
{

  try
  {

    if ( atomType && outwardNormalToMembraneMethod )
    {

      if ( _outwardNormalToMembraneMethods[ ( size_t )atomType ] )
      {

        std::cerr << "gkg::AtomMethodFactory::"
                  << "registerOutwardNormalToMembraneMethod for type "
                  << ( int32_t )atomType
                  << " already exists"
                  << std::endl;
        return false;

      }
      else
      {

        _outwardNormalToMembraneMethods[ ( size_t )atomType ] =
                                                  outwardNormalToMembraneMethod;
        return true;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AtomMethodFactory::"
             "registerOutwardNormalToMembraneMethod( "
             "uint8_t atomType, "
             "gkg::AtomMethodFactory::OutwardNormalToMembraneMethod "
             "outwardNormalToMembraneMethod )" );

}


gkg::AtomMethodFactory::OutwardNormalToMembraneMethod 
gkg::AtomMethodFactory::getOutwardNormalToMembraneMethod(
                                                        uint8_t atomType ) const
{

  try
  {

    if ( atomType &&
         ( ( size_t )atomType < _outwardNormalToMembraneMethods.size() ) )
    {

      return _outwardNormalToMembraneMethods[ ( size_t )atomType ];

    }
    else
    {

      throw std::runtime_error( "bad atom type" );

    }
    return 0;

  }
  GKG_CATCH( "gkg::AtomMethodFactory::OutwardNormalToMembraneMethod "
             "gkg::AtomMethodFactory::getOutwardNormalToMembraneMethod( "
             "uint8_t atomType ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// is ray colliding atom method 
////////////////////////////////////////////////////////////////////////////////

bool gkg::AtomMethodFactory::registerIsRayCollidingWithAtomMethod(
     uint8_t atomType,
     gkg::AtomMethodFactory::IsRayCollidingWithAtomMethod
                                                  isRayCollidingWithAtomMethod )
{

  try
  {

    if ( atomType && isRayCollidingWithAtomMethod )
    {

      if ( _isRayCollidingWithAtomMethods[ ( size_t )atomType ] )
      {

        std::cerr << "gkg::AtomMethodFactory::"
                  << "registerIsRayCollidingWithAtomMethod for type "
                  << ( int32_t )atomType
                  << " already exists"
                  << std::endl;
        return false;

      }
      else
      {

        _isRayCollidingWithAtomMethods[ ( size_t )atomType ] =
                                                   isRayCollidingWithAtomMethod;
        return true;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::AtomMethodFactory::"
             "registerIsRayCollidingWithAtomMethod( "
             "uint8_t atomType, "
             "gkg::AtomMethodFactory::IsRayCollidingWithAtomMethod "
             "isRayCollidingWithAtomMethod )" );

}


gkg::AtomMethodFactory::IsRayCollidingWithAtomMethod 
gkg::AtomMethodFactory::getIsRayCollidingWithAtomMethod(
                                                        uint8_t atomType ) const
{

  try
  {

    if ( atomType &&
         ( ( size_t )atomType < _isRayCollidingWithAtomMethods.size() ) )
    {

      return _isRayCollidingWithAtomMethods[ ( size_t )atomType ];

    }
    else
    {

      throw std::runtime_error( "bad atom type" );

    }
    return 0;

  }
  GKG_CATCH( "gkg::AtomMethodFactory::IsRayCollidingWithAtomMethod "
             "gkg::AtomMethodFactory::getIsRayCollidingWithAtomMethod( "
             "uint8_t atomType ) const" );

}
