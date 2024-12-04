#include <gkg-dmri-qspace-sampling/CustomMultipleDifferentShellQSpaceSampling.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-core-io/TextFileLineCount.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-exception/Exception.h>


gkg::CustomMultipleDifferentShellQSpaceSampling::
                                     CustomMultipleDifferentShellQSpaceSampling(
                       const std::string& fileNameBValues,
                       const std::string& fileNameOrientations,
                       float deltaBValue,
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                               : gkg::MultipleDifferentShellQSpaceSampling(
                                                      transform3d,
                                                      gradientCharacteristics ),
                                 _deltaBValue( deltaBValue )
{

  try
  {

    // reading orientations
    gkg::OrientationSet orientationSet( fileNameOrientations );

    const std::vector< Vector3d< float > >& 
      orientations = orientationSet.getOrientations();
    int32_t orientationCount = orientationSet.getCount();

    // reading b-values
    int32_t lineCount = gkg::getTextFileLineCount( fileNameBValues );

    ////////////////////////////////////////////////////////////////////////////
    // in case we provide a *.bval file
    ////////////////////////////////////////////////////////////////////////////

    std::vector< float > bValues;
    if ( lineCount == 1 )
    {

      int32_t bValueCount = 
                         gkg::getScalarItemCountPerLine( fileNameBValues )[ 0 ];
      if ( bValueCount != orientationCount )
      {

        throw std::runtime_error(
                               "b-value and orientation counts are different" );

      }

      gkg::UncommentCounterInputFileStream is( fileNameBValues.c_str() );

      bValues.resize( bValueCount );
      int32_t i = 0;
      for ( i = 0; i < bValueCount; i++ )
      {

        is >> bValues[ i ];

      }
      is.close();

    }
    ////////////////////////////////////////////////////////////////////////////
    // in case we provide a GKG b-value file
    ////////////////////////////////////////////////////////////////////////////
    else
    {

      gkg::UncommentCounterInputFileStream is( fileNameBValues.c_str() );
      int32_t bValueCount = 0;
      is >> bValueCount;

      if ( bValueCount != orientationCount )
      {

        is.close();
        throw std::runtime_error(
                               "b-value and orientation counts are different" );

      }
      bValues.resize( bValueCount );
      int32_t i = 0;
      for ( i = 0; i < bValueCount; i++ )
      {

        is >> bValues[ i ];

      }
      is.close();

    }

    // extracting shell(s)
    this->setShells( bValues, orientations, deltaBValue );

  }
  GKG_CATCH( "gkg::CustomMultipleDifferentShellQSpaceSampling::"
             "CustomMultipleDifferentShellQSpaceSampling( "
             "const std::string& fileNameBValues, "
             "const std::string& fileNameOrientations, "
             "float deltaBValue, "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}


gkg::CustomMultipleDifferentShellQSpaceSampling::
                                     CustomMultipleDifferentShellQSpaceSampling(
                       const std::vector< float >& bValues,
                       const std::vector< Vector3d< float > >& orientations,
                       float deltaBValue,
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                               : gkg::MultipleDifferentShellQSpaceSampling(
                                                      transform3d,
                                                      gradientCharacteristics ),
                                 _deltaBValue( deltaBValue )
{

  try
  {

    // extracting shell(s)
    this->setShells( bValues, orientations, deltaBValue );

  }
  GKG_CATCH( "gkg::CustomMultipleDifferentShellQSpaceSampling::"
             "CustomMultipleDifferentShellQSpaceSampling( "
             "const std::vector< float >& bValues, "
             "const std::vector< Vector3d< float > >& orientations, "
             "float deltaBValue, "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}


gkg::CustomMultipleDifferentShellQSpaceSampling::
                                   ~CustomMultipleDifferentShellQSpaceSampling()
{
}


void gkg::CustomMultipleDifferentShellQSpaceSampling::addInformationToHeader(
                                     gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    // first, adding 'diffusion_gradient_orientations' and 'bvalues' for
    // backward compatibility
    this->gkg::QSpaceSampling::addInformationToHeader( headeredObject );

    // adding new diffusion sensitization header
    gkg::Dictionary diffusion_sensitization_parameters;

    diffusion_sensitization_parameters[ "b-values" ] = this->_shellBValues;

    diffusion_sensitization_parameters[ "delta-b-value" ] = this->_deltaBValue;

    int32_t shellCount = ( int32_t )this->_shellBValues.size();
    int32_t s = 0;
    std::vector< int32_t > orientationCounts( shellCount );
    for ( s = 0; s < shellCount; s++ )
    {

      orientationCounts[ s ] = this->_orientationSets[ s ].getCount();

    }

    diffusion_sensitization_parameters[ "orientation-counts" ] =
                                                             orientationCounts;

    diffusion_sensitization_parameters[ "homogeneous-transform3d" ] =
                                    this->_transform3d->getDirectCoefficients();

    int32_t motionRotation3dCount = ( int32_t )this->_motionRotation3ds.size();
    gkg::GenericObjectList golMotionRotation3ds( motionRotation3dCount );
    gkg::Vector3d< float > motionRotation3d;
    gkg::GenericObjectList golMotionRotation3d( 3 );
    int32_t r = 0;
    for ( r = 0; r < motionRotation3dCount; r++ )
    {
    
      motionRotation3d = this->_motionRotation3ds[ r ];
      golMotionRotation3d[ 0 ] = motionRotation3d.x;
      golMotionRotation3d[ 1 ] = motionRotation3d.y;
      golMotionRotation3d[ 2 ] = motionRotation3d.z;
      golMotionRotation3ds[ r ] = golMotionRotation3d;
    
    }
    diffusion_sensitization_parameters[ "motion-rotation3ds" ] =
                                                           golMotionRotation3ds;

    gkg::GenericObjectList golOrientationSets( shellCount );
    for ( s = 0; s < shellCount; s++ )
    {

      int32_t orientationCount = orientationCounts[ s ];
      gkg::GenericObjectList golOrientations( orientationCount );
      gkg::Vector3d< float > orientation;
      gkg::GenericObjectList golOrientation( 3 );
      int32_t o = 0;
      for ( o = 0; o < orientationCount; o++ )
      {
 
        orientation = this->_orientationSets[ s ].getOrientation( o );
        golOrientation[ 0 ] = orientation.x;
        golOrientation[ 1 ] = orientation.y;
        golOrientation[ 2 ] = orientation.z;
        golOrientations[ o ] = golOrientation;
 
      }
      golOrientationSets[ s ] = golOrientations;

    }

    diffusion_sensitization_parameters[ "orientation-sets" ] =
                                                             golOrientationSets;

    this->addGradientCharacteristicsToDiffusionSensitizationParameters(
                                           diffusion_sensitization_parameters );

    headeredObject.getHeader().addAttribute(
          "diffusion_sensitization_type",
          std::string(
           "spherical_multiple-shell_different-orientation-sets_custom" ) );
    headeredObject.getHeader().addAttribute(
          "diffusion_sensitization_parameters",
          diffusion_sensitization_parameters );

  }
  GKG_CATCH( "void gkg::CustomMultipleDifferentShellQSpaceSampling::"
             "addInformationToHeader( "
             "gkg::HeaderedObject& headeredObject ) const" );

}


std::vector< gkg::RCPointer< gkg::CustomSingleShellQSpaceSampling > >
gkg::CustomMultipleDifferentShellQSpaceSampling::getShells(
               std::map< float, std::list< int32_t > >& bValueAndIndices ) const
{

  try
  {

    int32_t shellCount = this->getShellCount();

    std::vector< gkg::RCPointer< gkg::CustomSingleShellQSpaceSampling > >
      customSingleShellQSpaceSamplings( shellCount );


    // building a temporary map of shells from b-value(s)
    int32_t globalOrientationCount = ( int32_t )this->_orientations.size();
    std::map< float, std::list< int32_t > > shells;

    std::map< float, std::list< float > > shellBValues;

    int32_t o = 0;
    shells[ this->_bValues[ o ] ].push_back( o );
    shellBValues[ this->_bValues[ o ] ].push_back( this->_bValues[ o ] );

    for ( o = 1; o < globalOrientationCount; o++ )
    {

      bool foundShell = false;

      float bValue = this->_bValues[ o ];

      std::map< float, std::list< int32_t > >::iterator s = shells.begin(),
                                                        se = shells.end();
      std::map< float, std::list< float > >::iterator
        sb = shellBValues.begin();
      while ( s != se )
      {

        if ( gkg::equal( bValue, s->first, this->_deltaBValue ) )
        {

           foundShell = true;
           break;

        }
        ++ s;
        ++ sb;

      }

      if ( foundShell )
      {

        s->second.push_back( o );
        sb->second.push_back( bValue );

      }
      else
      {

        shells[ bValue ].push_back( o );
        shellBValues[ bValue ].push_back( bValue );

      }

    }

    // computing the dictionaries of gradient characteristics for each shell
    gkg::Dictionary multipleShellGradientCharacteristics = 
     this->_gradientCharacteristics;
    std::vector< gkg::Dictionary > gradientCharacteristicsDictionaries( 
                                                                   shellCount );
    int32_t shellIndex = 0;
    if ( multipleShellGradientCharacteristics[ "type" ]->getString() ==
         "PGSE" )
    {

      gkg::GenericObjectList
        little_delta = multipleShellGradientCharacteristics[ "little-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        big_delta = multipleShellGradientCharacteristics[ "big-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        gradient_magnitudes = multipleShellGradientCharacteristics[
                                                         "gradient-magnitudes" ]
                                         ->getValue< gkg::GenericObjectList >();

      std::map< float, std::list< int32_t > >::const_iterator
        bi = shells.begin(),
        bie = shells.end();
      while ( bi != bie )
      {

        const std::list< int32_t >& shellIndices = bi->second;

        gkg::GenericObjectList shell_little_delta( shellIndices.size() );
        gkg::GenericObjectList shell_big_delta( shellIndices.size() );
        gkg::GenericObjectList shell_gradient_magnitudes( shellIndices.size() );

        std::list< int32_t >::const_iterator i = shellIndices.begin(),
                                             ie = shellIndices.end();
        
        gkg::GenericObjectList::iterator ld = shell_little_delta.begin();
        gkg::GenericObjectList::iterator bd = shell_big_delta.begin();
        gkg::GenericObjectList::iterator gm = shell_gradient_magnitudes.begin();

        while ( i != ie )
        {

          *ld = little_delta[ *i ]->getScalar();
          *bd = big_delta[ *i ]->getScalar();
          *gm = gradient_magnitudes[ *i ]->getScalar();

          ++ i;
          ++ ld;
          ++ bd;
          ++ gm;

        }

        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "type" ] =
                                                      std::string( "PGSE" );
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "little-delta" ] =
                                                      shell_little_delta;
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "big-delta" ] =
                                                      shell_big_delta;
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "gradient-magnitudes" ] =
                                                      shell_gradient_magnitudes;


        ++ bi;
        ++ shellIndex;

      }
 
    }
    else if ( multipleShellGradientCharacteristics[ "type" ]->getString() ==
              "Sine-OGSE" )
    {

      gkg::GenericObjectList
        little_delta = multipleShellGradientCharacteristics[ "little-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        big_delta = multipleShellGradientCharacteristics[ "big-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        period = multipleShellGradientCharacteristics[ "period" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        phase = multipleShellGradientCharacteristics[ "phase" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        gradient_magnitudes = multipleShellGradientCharacteristics[
                                                         "gradient-magnitudes" ]
                                         ->getValue< gkg::GenericObjectList >();

      std::map< float, std::list< int32_t > >::const_iterator
        bi = shells.begin(),
        bie = shells.end();
      while ( bi != bie )
      {

        const std::list< int32_t >& shellIndices = bi->second;

        gkg::GenericObjectList shell_little_delta( shellIndices.size() );
        gkg::GenericObjectList shell_big_delta( shellIndices.size() );
        gkg::GenericObjectList shell_period( shellIndices.size() );
        gkg::GenericObjectList shell_phase( shellIndices.size() );
        gkg::GenericObjectList shell_gradient_magnitudes( shellIndices.size() );

        std::list< int32_t >::const_iterator i = shellIndices.begin(),
                                             ie = shellIndices.end();
        
        gkg::GenericObjectList::iterator ld = shell_little_delta.begin();
        gkg::GenericObjectList::iterator bd = shell_big_delta.begin();
        gkg::GenericObjectList::iterator pe = shell_period.begin();
        gkg::GenericObjectList::iterator ph = shell_phase.begin();
        gkg::GenericObjectList::iterator gm = shell_gradient_magnitudes.begin();

        while ( i != ie )
        {

          *ld = little_delta[ *i ]->getScalar();
          *bd = big_delta[ *i ]->getScalar();
          *pe = period[ *i ]->getScalar();
          *ph = phase[ *i ]->getScalar();
          *gm = gradient_magnitudes[ *i ]->getScalar();

          ++ i;
          ++ ld;
          ++ bd;
          ++ pe;
          ++ ph;
          ++ gm;

        }

        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "type" ] =
                                                     std::string( "Sine-OGSE" );
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "little-delta" ] =
                                                     shell_little_delta;
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "big-delta" ] =
                                                     shell_big_delta;
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "period" ] =
                                                     shell_period;
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "phase" ] =
                                                     shell_phase;
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "gradient-magnitudes" ] =
                                                     shell_gradient_magnitudes;


        ++ bi;
        ++ shellIndex;

      }
 
    }
    else if ( multipleShellGradientCharacteristics[ "type" ]->getString() ==
              "Cosine-OGSE" )
    {

      gkg::GenericObjectList
        little_delta = multipleShellGradientCharacteristics[ "little-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        big_delta = multipleShellGradientCharacteristics[ "big-delta" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        period = multipleShellGradientCharacteristics[ "period" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        phase = multipleShellGradientCharacteristics[ "phase" ]
                                         ->getValue< gkg::GenericObjectList >();

      gkg::GenericObjectList
        gradient_magnitudes = multipleShellGradientCharacteristics[
                                                         "gradient-magnitudes" ]
                                         ->getValue< gkg::GenericObjectList >();

      std::map< float, std::list< int32_t > >::const_iterator
        bi = shells.begin(),
        bie = shells.end();
      while ( bi != bie )
      {

        const std::list< int32_t >& shellIndices = bi->second;
        
        gkg::GenericObjectList shell_little_delta( shellIndices.size() );
        gkg::GenericObjectList shell_big_delta( shellIndices.size() );
        gkg::GenericObjectList shell_period( shellIndices.size() );
        gkg::GenericObjectList shell_phase( shellIndices.size() );
        gkg::GenericObjectList shell_gradient_magnitudes( shellIndices.size() );

        std::list< int32_t >::const_iterator i = shellIndices.begin(),
                                             ie = shellIndices.end();
        
        gkg::GenericObjectList::iterator ld = shell_little_delta.begin();
        gkg::GenericObjectList::iterator bd = shell_big_delta.begin();
        gkg::GenericObjectList::iterator pe = shell_period.begin();
        gkg::GenericObjectList::iterator ph = shell_phase.begin();
        gkg::GenericObjectList::iterator gm = shell_gradient_magnitudes.begin();

        while ( i != ie )
        {

          *ld = little_delta[ *i ]->getScalar();
          *bd = big_delta[ *i ]->getScalar();
          *pe = period[ *i ]->getScalar();
          *ph = phase[ *i ]->getScalar();
          *gm = gradient_magnitudes[ *i ]->getScalar();

          ++ i;
          ++ ld;
          ++ bd;
          ++ pe;
          ++ ph;
          ++ gm;

        }

        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "type" ] =
                                                   std::string( "Cosine-OGSE" );
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "little-delta" ] =
                                                   shell_little_delta;
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "big-delta" ] =
                                                   shell_big_delta;
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "period" ] =
                                                   shell_period;
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "phase" ] =
                                                   shell_phase;
        gradientCharacteristicsDictionaries[ shellIndex ]
                                           [ "gradient-magnitudes" ] =
                                                   shell_gradient_magnitudes;


        ++ bi;
        ++ shellIndex;

      }

    }
    else if ( multipleShellGradientCharacteristics[ "type" ]->getString() ==
              "unknown" )
    {

      for ( shellIndex = 0; shellIndex < shellCount; shellIndex++ )
      {

        gradientCharacteristicsDictionaries[ shellIndex ] = 
                                           multipleShellGradientCharacteristics;

      }

    }

    std::map< float, std::list< int32_t > >::const_iterator
      bi = shells.begin(),
      bie = shells.end();
    std::map< float, std::list< float > >::const_iterator
      sb = shellBValues.begin();
    std::vector< float >::const_iterator
      b = this->_shellBValues.begin();
    shellIndex = 0;
    while ( bi != bie )
    {

      bValueAndIndices[ *b ] = bi->second;

      customSingleShellQSpaceSamplings[ shellIndex ].reset(
        new gkg::CustomSingleShellQSpaceSampling(
                    std::vector< float >( sb->second.begin(),
                                          sb->second.end() ),
                    this->_orientationSets[ shellIndex ].getOrientations(),
                    *( this->_transform3d ),
                    gradientCharacteristicsDictionaries[ shellIndex ] ) );

      ++ bi;
      ++ sb;
      ++ b;
      ++ shellIndex;

    }

    return customSingleShellQSpaceSamplings;

  }
  GKG_CATCH( "std::vector< gkg::RCPointer< "
             "gkg::CustomSingleShellQSpaceSampling > > "
             "gkg::CustomMultipleDifferentShellQSpaceSampling::getShells( "
             "std::map< float, std::list< int32_t > >& "
             "bValueAndIndices ) const" );

}


float gkg::CustomMultipleDifferentShellQSpaceSampling::getDeltaBValue() const
{

  try
  {

    return _deltaBValue;

  }
  GKG_CATCH( "float gkg::CustomMultipleDifferentShellQSpaceSampling::"
             "getDeltaBValue() const" );

}


void gkg::CustomMultipleDifferentShellQSpaceSampling::setShells(
                     const std::vector< float >& bValues,
                     const std::vector< gkg::Vector3d< float > >& orientations,
                     float deltaBValue )
{

  try
  {

    if ( bValues.size() != orientations.size() )
    {

      throw std::runtime_error(
                               "inconsistent b-value and orientation vectors" );

    }

    int32_t globalOrientationCount = ( int32_t )orientations.size();

    this->_bValues = bValues;
    this->_orientations = orientations;
    this->_motionRotation3ds = std::vector< Vector3d< float > >(
                                                      globalOrientationCount );

    // building a temporary map of shells from b-value(s)
    std::map< float, std::list< int32_t > > shells;

    int32_t o = 0;
    shells[ bValues[ o ] ].push_back( o );

    for ( o = 1; o < globalOrientationCount; o++ )
    {

      bool foundShell = false;

      float bValue = bValues[ o ];

      std::map< float, std::list< int32_t > >::iterator s = shells.begin(),
                                                        se = shells.end();
      while ( s != se )
      {

        if ( gkg::equal( bValue, s->first, deltaBValue ) )
        {

           foundShell = true;
           break;

        }
        ++ s;

      }

      if ( foundShell )
      {

        s->second.push_back( o );

      }
      else
      {

        shells[ bValue ].push_back( o );

      }

    }

    // collecting shell count
    int32_t shellCount = ( int32_t )shells.size();

    // resizing vector of average b-values
    this->_shellBValues.resize( shellCount );

    // resizing vector of orientation sets
    this->_orientationSets.resize( shellCount );

    // replacing temporary b-value(s) by average b-value computed over all
    // orientation(s)
    std::map< float, std::list< int32_t > >::const_iterator s = shells.begin(),
                                                            se = shells.end();
    std::vector< float >::iterator 
      b = this->_shellBValues.begin();
    std::vector< gkg::OrientationSet >::iterator 
      os = this->_orientationSets.begin();
    while ( s != se )
    {

      const std::list< int32_t >& orientationIndices = s->second;
      int32_t orientationCount = ( int32_t )orientationIndices.size();
      std::vector< gkg::Vector3d< float > >
        shellOrientations( orientationCount );

      *b = 0.0f;
      std::list< int32_t >::const_iterator oi = orientationIndices.begin();
      for ( o = 0; o < orientationCount; o++, oi++ )
      {

        *b += bValues[ *oi ];
        shellOrientations[ o ] = orientations[ *oi ];

      }
      *b /= ( float )orientationCount;
      *os = gkg::OrientationSet( shellOrientations );

      ++ s;
      ++ b;
      ++ os;

    }

    this->extractDiffusionTimes();

    this->_shellDiffusionTimes.resize( shellCount );

    int32_t orientationIndex = 0;
    int32_t shell = 0;
    for ( shell = 0; shell < shellCount; shell++ )
    {

      this->_shellDiffusionTimes[ shell ] =
                                      this->_diffusionTimes[ orientationIndex ];
      orientationIndex += this->_orientationSets[ shell ].getCount();

    }

  }
  GKG_CATCH( "void gkg::CustomMultipleDifferentShellQSpaceSampling::setShells( "
             "const std::vector< float >& bValues, "
             "const std::vector< Vector3d< float > >& orientations, "
             "float deltaBValue )" );

}

