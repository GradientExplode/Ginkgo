#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorTransform3d_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorTransform3d_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorTransform3d.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


template < class L, class Compare >
inline
gkg::BundleMapOperatorTransform3d< L, Compare >::
                                   BundleMapOperatorTransform3d(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( ( stringParameters.size() != 1U ) &&
         ( stringParameters.size() != 3U ) )
    {

      throw std::runtime_error(
                               "string parameters must have 1 or 3 arguments" );

    }

    // sanity checks for scalar parameters
    if ( !scalarParameters.empty() )
    {

      throw std::runtime_error( "no scalar parameter required" );

    }

    // reading rigid or affine 3D transform
    std::string fileNameTransform3d = stringParameters[ 0 ];
    try
    {

      gkg::RigidTransform3d< float >*
        rigidTransform3d = new gkg::RigidTransform3d< float >;
      std::ifstream is( fileNameTransform3d.c_str() );
      if ( is.fail() )
      {

        throw std::runtime_error( "error while opening " +
                                  fileNameTransform3d );

      }
      rigidTransform3d->readTrm( is );
      is.close();
      _transform3d.reset( rigidTransform3d );

    }
    catch ( std::exception& )
    {

      try
      {

        gkg::AffineTransform3d< float >*
          affineTransform3d =
            new gkg::AffineTransform3d< float >;
        std::ifstream is( fileNameTransform3d.c_str() );
        if ( is.fail() )
        {

          throw std::runtime_error( "error while opening " +
                                    fileNameTransform3d );

        }
        affineTransform3d->readTrm( is );
        is.close();
        _transform3d.reset( affineTransform3d );

      }
      catch ( std::exception& )
      {

        throw std::runtime_error(
                                "unable to find an adequate linear transform" );

      }

    }

    // reading vector field 3D transform
    if ( stringParameters.size() == 3U )
    {

      std::string fileNameDirectVectorFieldTransform3d = stringParameters[ 1 ];
      gkg::Volume< float > directVectorField;
      try
      {

        gkg::Reader::getInstance().read( fileNameDirectVectorFieldTransform3d,
                                         directVectorField );
        if ( directVectorField.getSizeT() != 3 )
        {

          throw std::runtime_error( "direct vector field T size must be 3" );

        }

      }
      catch ( std::exception& )
      {
        gkg::Volume< gkg::Vector3d< float > > directVectorFieldVector3d;

        gkg::Reader::getInstance().read( fileNameDirectVectorFieldTransform3d,
                                         directVectorFieldVector3d );

        gkg::Vector3d< int32_t > sizes;
        directVectorFieldVector3d.getSize( sizes );

        gkg::Vector3d< double > resolutions;
        directVectorFieldVector3d.getResolution( resolutions );

        directVectorField.reallocate( sizes, 3 );
        directVectorField.setResolution( resolutions );

        int32_t x = 0;
        int32_t y = 0;
        int32_t z = 0;

        for ( z = 0; z < sizes.z; z++ )
        {

          for ( y = 0; y < sizes.y; y++ )
          {

            for ( x = 0; x < sizes.x; x++ )
            {

              directVectorField( x, y, z, 0 ) =
                                         directVectorFieldVector3d( x, y, z ).x;
              directVectorField( x, y, z, 1 ) =
                                         directVectorFieldVector3d( x, y, z ).y;
              directVectorField( x, y, z, 2 ) =
                                         directVectorFieldVector3d( x, y, z ).z;

            }

          }

        }

      }


      std::string fileNameInverseVectorFieldTransform3d = stringParameters[ 2 ];
      gkg::Volume< float > inverseVectorField;
      try
      {

        gkg::Reader::getInstance().read( fileNameInverseVectorFieldTransform3d,
                                         inverseVectorField );
        if ( inverseVectorField.getSizeT() != 3 )
        {

          throw std::runtime_error( "inverse vector field T size must be 3" );

        }

      }
      catch ( std::exception& )
      {

        gkg::Volume< gkg::Vector3d< float > > inverseVectorFieldVector3d;

        gkg::Reader::getInstance().read( fileNameInverseVectorFieldTransform3d,
                                         inverseVectorFieldVector3d );

        gkg::Vector3d< int32_t > sizes;
        inverseVectorFieldVector3d.getSize( sizes );

        gkg::Vector3d< double > resolutions;
        inverseVectorFieldVector3d.getResolution( resolutions );

        inverseVectorField.reallocate( sizes, 3 );
        inverseVectorField.setResolution( resolutions );

        int32_t x = 0;
        int32_t y = 0;
        int32_t z = 0;

        for ( z = 0; z < sizes.z; z++ )
        {

          for ( y = 0; y < sizes.y; y++ )
          {

            for ( x = 0; x < sizes.x; x++ )
            {

              inverseVectorField( x, y, z, 0 ) =
                                        inverseVectorFieldVector3d( x, y, z ).x;
              inverseVectorField( x, y, z, 1 ) =
                                        inverseVectorFieldVector3d( x, y, z ).y;
              inverseVectorField( x, y, z, 2 ) =
                                        inverseVectorFieldVector3d( x, y, z ).z;

            }

          }

        }

      }

      _vectorFieldTransform3d.reset(
                      new gkg::VectorFieldTransform3d< float >(
                                                         directVectorField,
                                                         inverseVectorField ) );

    }



  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorTransform3d< L, Compare >::"
             "BundleMapOperatorTransform3d( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorTransform3d< L, Compare >::
                                 ~BundleMapOperatorTransform3d()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorTransform3d< L, Compare >::startContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    // sanity checks
    if ( inputBundleMap.isNull() )
    {

      throw std::runtime_error( "input pointer is empty" );

    }
    if ( outputBundleMap.isNull() )
    {

      throw std::runtime_error( "output pointer is empty" );

    }


    // looping over input bundles
    int32_t fiberIndex = 0;
    typename gkg::BundleMap< L, Compare >::const_iterator
      ib = inputBundleMap->begin(),
      ibe = inputBundleMap->end();
    while ( ib != ibe )
    {

      fiberIndex = 0;

      // collecting the selected fibers
      gkg::Vector3d< float > tp;
      gkg::Vector3d< float > tp1;
      gkg::Vector3d< float > tp2;
      std::list< gkg::LightCurve3d< float > > fibers;
      std::vector< gkg::LightCurve3d< float > >::const_iterator
        f = ib->second.begin(),
        fe = ib->second.end();
      while ( f != fe )
      {

        std::list< gkg::Vector3d< float > > points;

        const gkg::LightCurve3d< float >& fiber = *f;
        gkg::LightCurve3d< float >::const_iterator p = fiber.begin(),
                                                   pe = fiber.end();
        while ( p != pe )
        {

          if ( _vectorFieldTransform3d.isNull() )
          {

            _transform3d->getDirect( *p, tp );
            points.push_back( tp );

          }
          else
          {

            _transform3d->getDirect( *p, tp1 );
            _vectorFieldTransform3d->getDirect( tp1, tp2 );
            points.push_back( tp2 );

          }

          ++ p;

        }

        fibers.push_back( gkg::LightCurve3d< float >( points ) );

        ++ fiberIndex;
        ++ f;

      }

      if ( !fibers.empty() )
      {

        // adding the kept fibers to the output bundle map
        outputBundleMap->addCurve3ds( ib->first, fibers );

      }

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorTransform3d< L, "
             "Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorTransform3d< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorTransform3d< L, "
             "Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorTransform3d< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorTransform3d< L, "
             "Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorTransform3d< L, Compare >::getName() const
{

  try
  {

    return  gkg::BundleMapOperatorTransform3d< L, Compare >:: getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorTransform3d< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorTransform3d< L, Compare >::getStaticName()
{

  try
  {

    return "transform3d";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorTransform3d< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorTransform3d< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string( ".  <P1>: 3D affine or rigid transform filename\n"
                        ".  <P2>: 3D direct vector field transform filename "
                        "(optional)\n"
                        ".  <P3>: 3D inverse vector field transform filename "
                        "(optional)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorTransform3d< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorTransform3d< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorTransform3d< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
