#include <gkg-dmri-microstructure-plot/MeasurementVsModelAttenuation.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>
#include <map>
#include <list>


gkg::MeasurementVsModelAttenuation::MeasurementVsModelAttenuation()
{
}


gkg::MeasurementVsModelAttenuation::MeasurementVsModelAttenuation(
          const std::vector< float >& measuredAttenuationValues,
          const gkg::OrientationSet& inputOrientationSet,
          const gkg::Vector3d< float >& principalOrientation,
          const std::vector< std::vector< float > >& modelededAttenuationValues,
          const std::vector< float >& multipleShellBValues,
          const std::vector< std::vector< int32_t > >&
                                              multipleShellOrientationIndexSets,
          const std::vector< float >& coordinates )
{

  try
  {

    int32_t shellCount = ( int32_t )multipleShellBValues.size();

    // now creating graph points
    _labels.resize( shellCount );

    _measuredXs.resize( shellCount );
    _measuredYs.resize( shellCount );

    _modeledXs.resize( shellCount );
    _modeledYs.resize( shellCount );

    int32_t shellIndex = 0;
    int32_t index = 0;
    int32_t o = 0;
    for ( shellIndex = 0; shellIndex < shellCount; shellIndex++ )
    {

      _labels[ shellIndex ] = gkg::StringConverter::toString( 
                                        multipleShellBValues[ shellIndex ],
                                        'f', 10, 2 ) + " s/mm2";
      int32_t shellOrientationCount = 
              ( int32_t )multipleShellOrientationIndexSets[ shellIndex ].size();


      _measuredXs[ shellIndex ].resize( shellOrientationCount );
      _measuredYs[ shellIndex ].resize( shellOrientationCount );

      _modeledXs[ shellIndex ].resize( coordinates.size() );
      _modeledYs[ shellIndex ].resize( coordinates.size() );

      for ( o = 0; o < shellOrientationCount; o++ )
      {

        index = multipleShellOrientationIndexSets[ shellIndex ][ o ];

        const gkg::Vector3d< float >& diffusionGradientOrientation = 
                                    inputOrientationSet.getOrientation( index );
        _measuredXs[ shellIndex ][ o ] =
           std::abs( diffusionGradientOrientation.dot( principalOrientation ) );
        _measuredYs[ shellIndex ][ o ] = measuredAttenuationValues[ index ];


      }

      _modeledXs[ shellIndex ] = coordinates;
      _modeledYs[ shellIndex ] = modelededAttenuationValues[ shellIndex ];


    }

  }
  GKG_CATCH( "gkg::MeasurementVsModelAttenuation::"
             "MeasurementVsModelAttenuation( "
             "const std::vector< float >& measuredAttenuationValues, "
             "const std::vector< float >& modeledAttenuationValues, "
             "const std::vector< float >& bValues, "
             "float deltaBValue, "
             "const gkg::OrientationSet& inputOrientationSet, "
             "const gkg::Vector3d< float >& principalOrientation )" );

}


gkg::MeasurementVsModelAttenuation::MeasurementVsModelAttenuation(
                              const gkg::MeasurementVsModelAttenuation& other )
                                   : _labels( other._labels ),
                                     _measuredXs( other._measuredXs ),
                                     _measuredYs( other._measuredYs ),
                                     _modeledXs( other._modeledXs ),
                                     _modeledYs( other._modeledYs )
{
}


gkg::MeasurementVsModelAttenuation::~MeasurementVsModelAttenuation()
{
}


gkg::MeasurementVsModelAttenuation&
gkg::MeasurementVsModelAttenuation::operator=(
                              const gkg::MeasurementVsModelAttenuation& other )
{

  try
  {

    _labels = other._labels;

    _measuredXs = other._measuredXs;
    _measuredYs = other._measuredYs;

    _modeledXs = other._modeledXs;
    _modeledYs = other._modeledYs;

    return *this;

  }
  GKG_CATCH( "gkg::MeasurementVsModelAttenuation& "
             "gkg::MeasurementVsModelAttenuation::operator=( "
             "const gkg::MeasurementVsModelAttenuation& other )" );

}


void 
gkg::MeasurementVsModelAttenuation::write( const std::string& fileName ) const
{

  try
  {

    std::ofstream os( fileName.c_str() );

    os << "import numpy as np" << std::endl;
    os << "import matplotlib.pyplot as plt" << std::endl;
    os << std::endl;

    int32_t shellCount = ( int32_t )_measuredXs.size();
    int32_t shellIndex = 0;
    int32_t o = 0;

    // displaying measuredXs ///////////////////////////////////////////////////
    os << "measuredXs = [ " << std::flush;
    for ( shellIndex = 0; shellIndex < shellCount; shellIndex++ )
    {

      os << "[ " << std::flush;

      int32_t shellOrientationCount = 
                                    ( int32_t )_measuredXs[ shellIndex ].size();

      for ( o = 0; o < shellOrientationCount - 1; o++ )
      {

        os << _measuredXs[ shellIndex ][ o ] << ", ";

      }
      os << _measuredXs[ shellIndex ][ o ] << " ]";

      if ( shellIndex == shellCount - 1 )
      {

        os << " ]" << std::endl;

      }
      else
      {

        os << ", " << std::endl;

      }

    }

    // displaying measuredYs //////////////////////////////////////////////////
    os << "measuredYs = [ " << std::flush;
    for ( shellIndex = 0; shellIndex < shellCount; shellIndex++ )
    {

      os << "[ " << std::flush;

      int32_t shellOrientationCount =
                                   ( int32_t )_measuredYs[ shellIndex ].size();

      for ( o = 0; o < shellOrientationCount - 1; o++ )
      {

        os << _measuredYs[ shellIndex ][ o ] << ", ";

      }
      os << _measuredYs[ shellIndex ][ o ] << " ]";

      if ( shellIndex == shellCount - 1 )
      {

        os << " ]" << std::endl;

      }
      else
      {

        os << ", " << std::endl;

      }

    }

    // displaying modeledXs ////////////////////////////////////////////////////
    os << "modeledXs = [ " << std::flush;
    for ( shellIndex = 0; shellIndex < shellCount; shellIndex++ )
    {

      os << "[ " << std::flush;

      int32_t shellOrientationCount = 
                                     ( int32_t )_modeledXs[ shellIndex ].size();

      for ( o = 0; o < shellOrientationCount - 1; o++ )
      {

        os << _modeledXs[ shellIndex ][ o ] << ", ";

      }
      os << _modeledXs[ shellIndex ][ o ] << " ]";

      if ( shellIndex == shellCount - 1 )
      {

        os << " ]" << std::endl;

      }
      else
      {

        os << ", " << std::endl;

      }

    }

    // displaying modeledYs ///////////////////////////////////////////////////
    os << "modeledYs = [ " << std::flush;
    for ( shellIndex = 0; shellIndex < shellCount; shellIndex++ )
    {

      os << "[ " << std::flush;

      int32_t shellOrientationCount =
                                    ( int32_t )_modeledYs[ shellIndex ].size();

      for ( o = 0; o < shellOrientationCount - 1; o++ )
      {

        os << _modeledYs[ shellIndex ][ o ] << ", ";

      }
      os << _modeledYs[ shellIndex ][ o ] << " ]";

      if ( shellIndex == shellCount - 1 )
      {

        os << " ]" << std::endl;

      }
      else
      {

        os << ", " << std::endl;

      }

    }

    // displaying labels ///////////////////////////////////////////////////////
    os << "labels = [ " << std::flush;
    for ( shellIndex = 0; shellIndex < shellCount; shellIndex++ )
    {

      os << "\'" << _labels[ shellIndex ] << "\'" << std::flush;
      if ( shellIndex == shellCount - 1 )
      {

        os << " ]" << std::endl;

      }
      else
      {

        os << ", " << std::endl;

      }

    }


    // loopig over shells and plotting point(s)
    os << "fig = plt.figure()" << std::endl;
    os << "fig.set_size_inches( 5, 5 )" << std::endl;
    os << "ax = fig.add_subplot(1, 1, 1)" << std::endl;
    os << std::endl;
    os << "ax.set_xlim( 0.0, 1.0)" << std::endl;
    os << "ax.set_ylim( 0.0, 1.0)" << std::endl;
    os << std::endl;

    os << "shellCount = len( measuredXs )" << std::endl;
    os << std::endl;

    os << "markers = [ 'o', '^' ]" << std::endl;
    os << "colors = [ 'r', 'g', 'b', 'c', 'm', 'y', 'k', 'w' ]" << std::endl;
    os << std::endl;
    os << "for shell in range( 0, shellCount ):" << std::endl;
    os << std::endl;
    os << "  plt.plot( measuredXs[ shell ], " << std::endl;
    os << "            measuredYs[ shell ], " << std::endl;
    os << "            marker=markers[ 0 ], " << std::endl;
    os << "            color=colors[ shell ], " << std::endl;
    os << "            linestyle='None', " << std::endl;
    os << "            label='measurement @b='+labels[ shell ] ) " << std::endl;
    os << "  plt.plot( modeledXs[ shell ], " << std::endl;
    os << "            modeledYs[ shell ], " << std::endl;
    os << "            color=colors[ shell ], " << std::endl;
    os << "            linestyle='-', " << std::endl; 
    os << "            label='model @b='+labels[ shell ] ) " << std::endl;
    os << std::endl;
    os << "plt.grid( True )" << std::endl;
    os << "plt.legend()" << std::endl;
    os << std::endl;
    os << "plt.xlabel( 'cosinus principal orientation "
       << "/ diffusion direction' )" << std::endl;
    os << "plt.ylabel( 'attenuation' )" << std::endl;
    os << std::endl;
    os << "plt.show()" << std::endl;
    os << std::endl;

    os.close();


  }
  GKG_CATCH( "void "
             "gkg::MeasurementVsModelAttenuation::write( "
             "const std::string& fileName ) const" );

}


