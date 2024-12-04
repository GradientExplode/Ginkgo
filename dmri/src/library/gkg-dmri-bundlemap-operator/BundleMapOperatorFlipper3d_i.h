#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorFlipper3d_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorFlipper3d_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFlipper3d.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


template < class L, class Compare >
inline
gkg::BundleMapOperatorFlipper3d< L, Compare >::BundleMapOperatorFlipper3d(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( stringParameters.empty() )
    {

      throw std::runtime_error(
                            "string parameters must have at least 1 argument" );

    }
    int32_t f = 0;
    int32_t flippingTypeCount = ( int32_t )stringParameters.size();
    std::string flippingType;
    for ( f = 0; f < flippingTypeCount; f++ )
    {

      flippingType = stringParameters[ f ];
      if ( ( flippingType != "x" ) &&
           ( flippingType != "y" ) &&
           ( flippingType != "z" ) &&
           ( flippingType != "xy" ) &&
           ( flippingType != "xz" ) &&
           ( flippingType != "yz" ) )
      {

        throw std::runtime_error( "unknown flipping type" );

      }

    }

    _flippingTypes = stringParameters;

    // sanity checks for scalar parameters
    if ( !scalarParameters.empty() )
    {

      throw std::runtime_error( "no scalar string parameter required" );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorFlipper3d< L, Compare >::"
             "BundleMapOperatorFlipper3d( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorFlipper3d< L, Compare >::~BundleMapOperatorFlipper3d()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorFlipper3d< L, Compare >::startContribution(
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



    gkg::RCPointer< gkg::BundleMap< L, Compare > > 
      tmpInputBundleMap = inputBundleMap;
    gkg::RCPointer< gkg::BundleMap< L, Compare > > tmpOutputBundleMap;

    int32_t flippingTypeCount = ( int32_t )_flippingTypes.size();
    int32_t flippingTypeIndex = 0; 

    std::vector< std::string >::const_iterator
      ft = _flippingTypes.begin(),
      fte = _flippingTypes.end();

    while ( ft != fte )
    {

      // collecting information of tmp input bundle map
      int32_t sizeX = 1;
      if ( tmpInputBundleMap->getHeader().hasAttribute( "sizeX" ) )
      {

        try
        {

          tmpInputBundleMap->getHeader().getAttribute( "sizeX", sizeX );

        }
        catch ( std::exception& )
        {

          double tmpSizeX = 1.0;
          tmpInputBundleMap->getHeader().getAttribute( "sizeX", tmpSizeX );

          sizeX = ( int32_t )( tmpSizeX + 0.5 );

        }

      }

      int32_t sizeY = 1;
      if ( tmpInputBundleMap->getHeader().hasAttribute( "sizeY" ) )
      {

        try
        {

          tmpInputBundleMap->getHeader().getAttribute( "sizeY", sizeY );

        }
        catch ( std::exception& )
        {

          double tmpSizeY = 1.0;
          tmpInputBundleMap->getHeader().getAttribute( "sizeY", tmpSizeY );

          sizeY = ( int32_t )( tmpSizeY + 0.5 );

        }

      }

      int32_t sizeZ = 1;
      if ( tmpInputBundleMap->getHeader().hasAttribute( "sizeZ" ) )
      {

        try
        {

          tmpInputBundleMap->getHeader().getAttribute( "sizeZ", sizeZ );

        }
        catch ( std::exception& )
        {

          double tmpSizeZ = 1.0;
          tmpInputBundleMap->getHeader().getAttribute( "sizeZ", tmpSizeZ );

          sizeZ = ( int32_t )( tmpSizeZ + 0.5 );

        }

      }

      double resolutionX = 1.0;
      if ( tmpInputBundleMap->getHeader().hasAttribute( "resolutionX" ) )
      {

        tmpInputBundleMap->getHeader().getAttribute( "resolutionX",
                                                     resolutionX );

      }

      double resolutionY = 1.0;
      if ( tmpInputBundleMap->getHeader().hasAttribute( "resolutionY" ) )
      {

        tmpInputBundleMap->getHeader().getAttribute( "resolutionY",
                                                     resolutionY );

      }

      double resolutionZ = 1.0;
      if ( tmpInputBundleMap->getHeader().hasAttribute( "resolutionZ" ) )
      {

        tmpInputBundleMap->getHeader().getAttribute( "resolutionZ",
                                                     resolutionZ );

      }

      // allocating tmp output bundle map
      if ( flippingTypeIndex == flippingTypeCount - 1 )
      {

        tmpOutputBundleMap = outputBundleMap;

      }
      else
      {

        tmpOutputBundleMap.reset( new gkg::BundleMap< L, Compare > );

      }

      if ( *ft == "x" )
      {

        // looping over input bundles
        int32_t fiberIndex = 0;
        typename gkg::BundleMap< L, Compare >::const_iterator
          ib = tmpInputBundleMap->begin(),
          ibe = tmpInputBundleMap->end();
        while ( ib != ibe )
        {

          fiberIndex = 0;

          // collecting the selected fibers
          gkg::Vector3d< float > tp;
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

              tp.x = ( float )sizeX * ( float )resolutionX - p->x;
              tp.y = p->y;
              tp.z = p->z;

              points.push_back( tp );
              ++ p;

            }

            fibers.push_back( gkg::LightCurve3d< float >( points ) );

            ++ fiberIndex;
            ++ f;

          }

          if ( !fibers.empty() )
          {

            // adding the kept fibers to the output bundle map
            tmpOutputBundleMap->addCurve3ds( ib->first, fibers );

          }

          ++ ib;

        }

      }
      else if ( *ft == "y" )
      {

        // looping over input bundles
        int32_t fiberIndex = 0;
        typename gkg::BundleMap< L, Compare >::const_iterator
          ib = tmpInputBundleMap->begin(),
          ibe = tmpInputBundleMap->end();
        while ( ib != ibe )
        {

          fiberIndex = 0;

          // collecting the selected fibers
          gkg::Vector3d< float > tp;
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

              tp.x = p->x;
              tp.y = ( float )sizeY * ( float )resolutionY - p->y;
              tp.z = p->z;

              points.push_back( tp );
              ++ p;

            }

            fibers.push_back( gkg::LightCurve3d< float >( points ) );

            ++ fiberIndex;
            ++ f;

          }

          if ( !fibers.empty() )
          {

            // adding the kept fibers to the output bundle map
            tmpOutputBundleMap->addCurve3ds( ib->first, fibers );

          }

          ++ ib;

        }

      }
      else if ( *ft == "z" )
      {

        // looping over input bundles
        int32_t fiberIndex = 0;
        typename gkg::BundleMap< L, Compare >::const_iterator
          ib = tmpInputBundleMap->begin(),
          ibe = tmpInputBundleMap->end();
        while ( ib != ibe )
        {

          fiberIndex = 0;

          // collecting the selected fibers
          gkg::Vector3d< float > tp;
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

              tp.x = p->x;
              tp.y = p->y;
              tp.z = ( float )sizeZ * ( float )resolutionZ - p->z;

              points.push_back( tp );
              ++ p;

            }

            fibers.push_back( gkg::LightCurve3d< float >( points ) );

            ++ fiberIndex;
            ++ f;

          }

          if ( !fibers.empty() )
          {

            // adding the kept fibers to the output bundle map
            tmpOutputBundleMap->addCurve3ds( ib->first, fibers );

          }

          ++ ib;

        }

      }
      else if ( *ft == "xy" )
      {

        // looping over input bundles
        int32_t fiberIndex = 0;
        typename gkg::BundleMap< L, Compare >::const_iterator
          ib = tmpInputBundleMap->begin(),
          ibe = tmpInputBundleMap->end();
        while ( ib != ibe )
        {

          fiberIndex = 0;

          // collecting the selected fibers
          gkg::Vector3d< float > tp;
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

              tp.x = p->y;
              tp.y = p->x;
              tp.z = p->z;

              points.push_back( tp );
              ++ p;

            }

            fibers.push_back( gkg::LightCurve3d< float >( points ) );

            ++ fiberIndex;
            ++ f;

          }

          if ( !fibers.empty() )
          {

            // adding the kept fibers to the output bundle map
            tmpOutputBundleMap->addCurve3ds( ib->first, fibers );

          }

          ++ ib;

        }

        std::swap( sizeX, sizeY );
        std::swap( resolutionX, resolutionY );

      }
      else if ( *ft == "xz" )
      {

        // looping over input bundles
        int32_t fiberIndex = 0;
        typename gkg::BundleMap< L, Compare >::const_iterator
          ib = tmpInputBundleMap->begin(),
          ibe = tmpInputBundleMap->end();
        while ( ib != ibe )
        {

          fiberIndex = 0;

          // collecting the selected fibers
          gkg::Vector3d< float > tp;
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

              tp.x = p->z;
              tp.y = p->y;
              tp.z = p->x;

              points.push_back( tp );
              ++ p;

            }

            fibers.push_back( gkg::LightCurve3d< float >( points ) );

            ++ fiberIndex;
            ++ f;

          }

          if ( !fibers.empty() )
          {

            // adding the kept fibers to the output bundle map
            tmpOutputBundleMap->addCurve3ds( ib->first, fibers );

          }

          ++ ib;

        }

        std::swap( sizeX, sizeZ );
        std::swap( resolutionX, resolutionZ );

      }
      else if ( *ft == "yz" )
      {

        // looping over input bundles
        int32_t fiberIndex = 0;
        typename gkg::BundleMap< L, Compare >::const_iterator
          ib = tmpInputBundleMap->begin(),
          ibe = tmpInputBundleMap->end();
        while ( ib != ibe )
        {

          fiberIndex = 0;

          // collecting the selected fibers
          gkg::Vector3d< float > tp;
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

              tp.x = p->x;
              tp.y = p->z;
              tp.z = p->y;

              points.push_back( tp );
              ++ p;

            }

            fibers.push_back( gkg::LightCurve3d< float >( points ) );

            ++ fiberIndex;
            ++ f;

          }

          if ( !fibers.empty() )
          {

            // adding the kept fibers to the output bundle map
            tmpOutputBundleMap->addCurve3ds( ib->first, fibers );

          }

          ++ ib;

        }

        std::swap( sizeY, sizeZ );
        std::swap( resolutionY, resolutionZ );

      }

      tmpOutputBundleMap->getHeader().addAttribute( "sizeX", sizeX );
      tmpOutputBundleMap->getHeader().addAttribute( "sizeY", sizeY );
      tmpOutputBundleMap->getHeader().addAttribute( "sizeZ", sizeZ );

      tmpOutputBundleMap->getHeader().addAttribute( "resolutionX",
                                                    resolutionX );
      tmpOutputBundleMap->getHeader().addAttribute( "resolutionY",
                                                    resolutionY );
      tmpOutputBundleMap->getHeader().addAttribute( "resolutionZ",
                                                    resolutionZ );


      tmpInputBundleMap = tmpOutputBundleMap;
      ++ ft;
      ++ flippingTypeIndex;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorFlipper3d< L, "
             "Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorFlipper3d< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorFlipper3d< L, "
             "Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorFlipper3d< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorFlipper3d< L, "
             "Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorFlipper3d< L, Compare >::getName() const
{

  try
  {

    return  gkg::BundleMapOperatorFlipper3d< L, Compare >:: getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorFlipper3d< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorFlipper3d< L, Compare >::getStaticName()
{

  try
  {

    return "flipper3d";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorFlipper3d< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorFlipper3d< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string( ".  <P1...N>: flipping mode, one of\n"
                        ".             - x\n"
                        ".             - y\n"
                        ".             - z\n"
                        ".             - xy\n"
                        ".             - xz\n"
                        ".             - yz\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorFlipper3d< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorFlipper3d< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorFlipper3d< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
