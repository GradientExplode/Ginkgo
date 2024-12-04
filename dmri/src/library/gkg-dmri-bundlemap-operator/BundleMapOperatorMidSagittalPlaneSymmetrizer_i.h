#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorMidSagittalPlaneSymmetrizer_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorMidSagittalPlaneSymmetrizer_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorMidSagittalPlaneSymmetrizer.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


template < class L, class Compare >
inline
gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::
                                   BundleMapOperatorMidSagittalPlaneSymmetrizer(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( stringParameters.size() != 1U )
    {

      throw std::runtime_error( "string parameters must have 1 argument" );

    }
    std::string fileNameMidSagittalPlaneCoefficients = stringParameters[ 0 ];

    // sanity checks for scalar parameters
    if ( !scalarParameters.empty() )
    {

      throw std::runtime_error( "no scalar string parameter required" );

    }

    // reading mid-sagittal plane coefficients
    std::ifstream is( fileNameMidSagittalPlaneCoefficients.c_str() );
    if ( is.fail() )
    {

      throw std::runtime_error( "error while opening " + 
                                         fileNameMidSagittalPlaneCoefficients );

    }
    _midSagittalPlaneCoefficients.resize( 4 );
    is >> _midSagittalPlaneCoefficients[ 0 ];
    is >> _midSagittalPlaneCoefficients[ 1 ];
    is >> _midSagittalPlaneCoefficients[ 2 ];
    is >> _midSagittalPlaneCoefficients[ 3 ];

    is.close();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::"
             "BundleMapOperatorMidSagittalPlaneSymmetrizer( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::
                                 ~BundleMapOperatorMidSagittalPlaneSymmetrizer()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::
                                                             startContribution(
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
    typename gkg::BundleMap< L, Compare >::const_iterator
      ib = inputBundleMap->begin(),
      ibe = inputBundleMap->end();
    while ( ib != ibe )
    {

      // collecting the selected fibers
      std::list< gkg::LightCurve3d< float > > fibers;
      std::vector< gkg::LightCurve3d< float > >::const_iterator
        f = ib->second.begin(),
        fe = ib->second.end();
      while ( f != fe )
      {

        std::list< gkg::Vector3d< float > > points;

        gkg::LightCurve3d< float >::const_iterator s = f->begin(),
                                                   se = f->end();
        while ( s != se )
        {

          points.push_back( this->getSymmetric( *s ) );
          ++ s;

        }
        
        fibers.push_back( gkg::LightCurve3d< float >( points ) );

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
             "void gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, "
             "Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::
                                                               addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, "
             "Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::
                                                               endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, "
             "Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::getName() const
{

  try
  {

    return 
      gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::
                                                                getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::getStaticName()
{

  try
  {

    return "mid-sagittal-plane-symmetrizer";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::
                                                        getStringParameterHelp()
{

  try
  {

    return std::string( ".  <P1>: mid-sagittal plane coefficient filename\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::
                                                        getScalarParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
gkg::Vector3d< float > 
gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::
                        getSymmetric( const gkg::Vector3d< float >& site ) const
{

  try
  {

    gkg::Vector3d< float > symmetricSite( 0, 0, 0 );

    float u = _midSagittalPlaneCoefficients[ 0 ] * site.x +
              _midSagittalPlaneCoefficients[ 1 ] * site.y +
              _midSagittalPlaneCoefficients[ 2 ] * site.z +
              _midSagittalPlaneCoefficients[ 3 ];

    float b2 = _midSagittalPlaneCoefficients[ 1 ] *
               _midSagittalPlaneCoefficients[ 1 ];
    float c2 = _midSagittalPlaneCoefficients[ 2 ] *
               _midSagittalPlaneCoefficients[ 2 ];

    symmetricSite.x = ( -u - _midSagittalPlaneCoefficients[ 3 ] -
                        ( b2 * site.x / _midSagittalPlaneCoefficients[ 0 ] ) -
                        ( c2 * site.x / _midSagittalPlaneCoefficients[ 0 ] ) -
                       ( site.y * _midSagittalPlaneCoefficients[ 1 ] ) -
                       ( site.z * _midSagittalPlaneCoefficients[ 2 ] ) ) /
                      ( _midSagittalPlaneCoefficients[ 0 ] +
                        ( b2 / _midSagittalPlaneCoefficients[ 0 ] ) +
                        ( c2 / _midSagittalPlaneCoefficients[ 0 ] ) );

    symmetricSite.y = _midSagittalPlaneCoefficients[ 1 ] /
                       _midSagittalPlaneCoefficients[ 0 ] *
                      ( symmetricSite.x - site.x ) + site.y;

    symmetricSite.z = _midSagittalPlaneCoefficients[ 2 ] /
                      _midSagittalPlaneCoefficients[ 0 ] *
                      ( symmetricSite.x - site.x ) + site.z;

    return symmetricSite;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::Vector3d< float > "
             "gkg::BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >::"
             "getSymmetric( const gkg::Vector3d< float >& site ) const" );

}


#endif
