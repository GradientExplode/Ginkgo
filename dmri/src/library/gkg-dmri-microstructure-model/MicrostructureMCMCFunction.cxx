#include <gkg-dmri-microstructure-model/MicrostructureMCMCFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>
#include <map>
#include <list>


gkg::MicrostructureMCMCFunction::MicrostructureMCMCFunction( 
                             gkg::RCPointer< gkg::Volume< float > > dw,
                             const gkg::OrientationSet& outputOrientationSet,
                             double noiseStandardDeviation )
                       : gkg::MonteCarloMarkovChainFunction()
{

  try
  {

    // getting input orientation count
    _inputOrientationCount = dw->getSizeT();

    // collecting output orientation set
    _outputOrientationCount = outputOrientationSet.getCount();
    _outputOrientations = outputOrientationSet.getOrientations();

    // collecting b-values
    _bValues.resize( _inputOrientationCount );
    if ( dw->getHeader().hasAttribute( "bvalues" ) )
    {

      std::vector< double > bValues;
      dw->getHeader().getAttribute( "bvalues", bValues );

      // resizing the b-value vector
      _bValues.resize( bValues.size() );

      // rescaling b-values to s/m^2 instead of s/mm^2
      std::vector< float >::iterator ob = _bValues.begin(),
                                      obe = _bValues.end();
      std::vector< double >::const_iterator ib = bValues.begin();
      while ( ob != obe )
      {

        *ob = *ib * 1e6;   
        ++ ob;
        ++ ib;

      } 

    }
    else
    {

      throw std::runtime_error( "missing attribute 'bvalues'" );

    }

    // collecting orientations
    gkg::GenericObjectList 
     genericObjectListOrientations( _inputOrientationCount );
    if ( dw->getHeader().hasAttribute( "diffusion_gradient_orientations" ) )
    {

      dw->getHeader().getAttribute( "diffusion_gradient_orientations",
                                    genericObjectListOrientations );

    }
    else
    {

      throw std::runtime_error(
                        "missing attribute 'diffusion_gradient_orientations'" );

    }
    _inputOrientations.resize( _inputOrientationCount );
    int32_t i = 0;
    for ( i = 0; i < _inputOrientationCount; i++ )
    {

      gkg::GenericObjectList orientation =
       genericObjectListOrientations[ i ]->getValue< gkg::GenericObjectList >();
      _inputOrientations[ i ].x = ( float )orientation[ 0 ]->getScalar();
      _inputOrientations[ i ].y = ( float )orientation[ 1 ]->getScalar();
      _inputOrientations[ i ].z = ( float )orientation[ 2 ]->getScalar();

    }

    // collecting dictionary of gradient characteristics
    if ( dw->getHeader().hasAttribute( "diffusion_sensitization_parameters" ) )
    {

      gkg::Dictionary diffusion_sensitization_parameters;
      dw->getHeader().getAttribute( "diffusion_sensitization_parameters",
                                    diffusion_sensitization_parameters );

      if ( diffusion_sensitization_parameters.find( "gradient-characteristics" )
           != diffusion_sensitization_parameters.end() )
      {

        // getting access to the dictionray of gradient characteristics
        this->_gradientCharacteristics =
          diffusion_sensitization_parameters[ "gradient-characteristics"
                                        ]->getValue< gkg::Dictionary >();

      }

    }

    // computing noise variance
    _noiseStandardDeviation = noiseStandardDeviation;
    _noiseVariance = _noiseStandardDeviation * _noiseStandardDeviation;

    // getting a pointer to the numerical analysis factory
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();


  }
  GKG_CATCH( "gkg::MicrostructureMCMCFunction::MicrostructureMCMCFunction( "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::OrientationSet* samplerOrientationSet )" );

}


gkg::MicrostructureMCMCFunction::~MicrostructureMCMCFunction()
{
}


double gkg::MicrostructureMCMCFunction::getLikelihoodRatio(
                                      const gkg::Vector& realMeasurements,
                                      const gkg::Vector& currentMeasurements,
                                      const gkg::Vector& newMeasurements ) const
{

  try
  {

    // Gaussian case
    // 
    // double logLikelihood1 = 0.0;
    // double logLikelihood2 = 0.0;
    // 
    // int32_t m = 0;
    // int32_t measurementCount = realMeasurements.getSize();
    // for ( m = 0; m < measurementCount; m++ )
    // {
    // 
    //   logLikelihood1 += -std::log( _noiseStandardDeviation *
    //                                std::sqrt( 2 * M_PI ) ) -
    //                     ( ( currentMeasurements( m ) -
    //                         realMeasurements( m ) ) *
    //                       ( currentMeasurements( m ) -
    //                         realMeasurements( m ) ) ) / 
    //                     ( 2.0 * _noiseVariance );
    // 
    //   logLikelihood2 += -std::log( _noiseStandardDeviation *
    //                                std::sqrt( 2 * M_PI ) ) -
    //                     ( ( newMeasurements( m ) -
    //                         realMeasurements( m ) ) *
    //                       ( newMeasurements( m ) -
    //                         realMeasurements( m ) ) ) / 
    //                     ( 2.0 * _noiseVariance );
    // 
    // 
    // 
    // }
    // 
    // return std::exp( logLikelihood2 - logLikelihood1 );

    // Rician case
    double logLikelihood1 = 0.0;
    double logLikelihood2 = 0.0;

    int32_t m = 0;
    int32_t measurementCount = realMeasurements.getSize();
    double argument1 = 0.0;
    double argument2 = 0.0;
    for ( m = 0; m < measurementCount; m++ )
    {

      argument1 = ( realMeasurements( m ) * currentMeasurements( m ) ) /
                 _noiseVariance;
      if ( argument1 < 700.0 )
      {

        logLikelihood1 += std::log( 
	                    currentMeasurements( m ) *
	                    _factory->getRegularBesselFunctionI0( argument1 ) / 
                            _noiseVariance ) -
                        ( currentMeasurements( m ) * currentMeasurements( m ) +
                          realMeasurements( m ) * realMeasurements( m ) ) /
                        ( 2.0 * _noiseVariance );
      }
      else
      {

        logLikelihood1 += std::log( currentMeasurements( m ) / 
                                  _noiseVariance /
				  std::sqrt( 2.0 * M_PI * argument1 ) ) -
                        ( currentMeasurements( m ) * currentMeasurements( m ) +
                          realMeasurements( m ) * realMeasurements( m ) ) /
                        ( 2.0 * _noiseVariance ) + argument1;
      }


      argument2 = ( realMeasurements( m ) * newMeasurements( m ) ) /
                 _noiseVariance;
      if ( argument2 < 700.0 )
      {

        logLikelihood2 += std::log( 
	                    newMeasurements( m ) *
	                    _factory->getRegularBesselFunctionI0( argument2 ) / 
                            _noiseVariance ) -
                        ( newMeasurements( m ) * newMeasurements( m ) +
                          realMeasurements( m ) * realMeasurements( m ) ) /
                        ( 2.0 * _noiseVariance );

      }
      else
      {

        logLikelihood2 += std::log( newMeasurements( m ) / 
                                  _noiseVariance /
				  std::sqrt( 2.0 * M_PI * argument2 ) ) -
                        ( newMeasurements( m ) * newMeasurements( m ) +
                          realMeasurements( m ) * realMeasurements( m ) ) /
                        ( 2.0 * _noiseVariance ) + argument2;

      }

    }

    return std::exp( logLikelihood2 - logLikelihood1 );

  }
  GKG_CATCH( "double gkg::MicrostructureMCMCFunction::getLikelihoodRatio( "
             "const gkg::Vector& realMeasurements, "
             "const gkg::Vector& currentMeasurements, "
             "const gkg::Vector& newMeasurements ) const" );

}


void gkg::MicrostructureMCMCFunction::getModelAt(
       const gkg::Vector& /* parameters */,
       const std::vector< float >& /* bValues */,
       const std::vector< std::vector< float > >& /* gradientCharacteristics */,
       int32_t /* inputOrientationCount */,
       std::vector< std::vector< float > >& /* values */,
       std::vector< float >& /* coordinates */ ) const
{

  try
  {

    throw std::runtime_error( "should not pass through this method" );

  }
  GKG_CATCH(
           "void gkg::MicrostructureMCMCFunction::getModelAt( "
           "const gkg::Vector& parameters, "
           "const std::vector< float >& bValues, "
           "const std::vector< std::vector< float > >& gradientCharacteristics,"
           "int32_t inputOrientationCount, "
           "std::vector< std::vector< float > >& values, "
           "std::vector< float >& coordinates ) const" );

}


int32_t gkg::MicrostructureMCMCFunction::getInputOrientationCount() const
{

  try
  {
  
    return _inputOrientationCount;
  
  }
  GKG_CATCH( "int32_t "
             "gkg::MicrostructureMCMCFunction::getInputOrientationCount() "
             "const" );

}


void gkg::MicrostructureMCMCFunction::getMultipleShellBValues(
                   std::string& sequenceType,
                   std::vector< std::vector< float > >& gradientCharacteristics,
                   std::vector< float >& multipleShellBValues,
                   std::vector< std::vector< int32_t > >& orientationIndexSets,
                   float deltaBValue ) const
{

  try
  {

    int32_t globalOrientationCount = ( int32_t )_inputOrientations.size();

    // building a temporary map of shells from b-value(s)
    std::map< float, std::list< int32_t > > shells;

    int32_t o = 0;
    shells[ _bValues[ o ] ].push_back( o );

    for ( o = 1; o < globalOrientationCount; o++ )
    {

      bool foundShell = false;

      float bValue = _bValues[ o ];

      std::map< float, std::list< int32_t > >::iterator s = shells.begin(),
                                                        se = shells.end();
      while ( s != se )
      {

        if ( gkg::equal( bValue, s->first, deltaBValue * 1e6f ) )
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

    // creating vector of average b-values
    multipleShellBValues.resize( shellCount );

    // creating vector of orientation index sets
    orientationIndexSets.resize( shellCount );

    // replacing temporary b-value(s) by average b-value computed over all
    // orientation(s)
    std::map< float, std::list< int32_t > >::const_iterator s = shells.begin(),
                                                            se = shells.end();
    std::vector< float >::iterator 
      b = multipleShellBValues.begin();
    std::vector< std::vector< int32_t > >::iterator 
      ois = orientationIndexSets.begin();
    while ( s != se )
    {

      const std::list< int32_t >& orientationIndices = s->second;
      int32_t orientationCount = ( int32_t )orientationIndices.size();

      *b = 0.0f;
      ois->resize( orientationCount );

      std::list< int32_t >::const_iterator oi = orientationIndices.begin();
      for ( o = 0; o < orientationCount; o++, oi++ )
      {

        *b += _bValues[ *oi ];
        ( *ois )[ o ] = *oi;

      }
      *b /= 1e6f * ( float )orientationCount;

      ++ s;
      ++ b;
      ++ ois;

    }

    // building the gradientCharacteristics vector for getModelAt
    gkg::Dictionary::const_iterator 
      c = this->_gradientCharacteristics.find( "type" );
    if ( c != this->_gradientCharacteristics.end() )
    {

      sequenceType = c->second->getString();


      if ( sequenceType == "PGSE" )
      {

        // collecting gradient magnitude(s) ////////////////////////////////////
        gkg::GenericObjectList golGradientMagnitudes;
        if ( _gradientCharacteristics.find( "gradient-magnitudes" )
             != _gradientCharacteristics.end() )
        {

          gkg::Dictionary::const_iterator 
            c = _gradientCharacteristics.find( "gradient-magnitudes" );
  
          golGradientMagnitudes = 
                                c->second->getValue< gkg::GenericObjectList >();

        }
        else
        {

          throw std::runtime_error( "missing attribute 'gradient-magnitudes'" );

        }

        // collecting little delta(s) //////////////////////////////////////////
        gkg::GenericObjectList golLittleDeltas;
        if ( _gradientCharacteristics.find( "little-delta" )
             != _gradientCharacteristics.end() )
        {

          gkg::Dictionary::const_iterator 
            c = _gradientCharacteristics.find( "little-delta" );

          golLittleDeltas = c->second->getValue< gkg::GenericObjectList >();

        }
        else
        {

          throw std::runtime_error( "missing attribute 'little-delta'" );

        }

        // collecting big delta(s) /////////////////////////////////////////////
        gkg::GenericObjectList golBigDeltas;
        if ( _gradientCharacteristics.find( "big-delta" )
             != _gradientCharacteristics.end() )
        {

          gkg::Dictionary::const_iterator 
            c = _gradientCharacteristics.find( "big-delta" );

          golBigDeltas = c->second->getValue< gkg::GenericObjectList >();

        }
        else
        {

          throw std::runtime_error( "missing attribute 'big-delta'" );

        }

        // collecting rise time(s) //////////////////////////////////////////
        gkg::GenericObjectList golRiseTimes;
        if ( _gradientCharacteristics.find( "rise-time" )
             != _gradientCharacteristics.end() )
        {

          gkg::Dictionary::const_iterator 
            c = _gradientCharacteristics.find( "rise-time" );

          golRiseTimes = c->second->getValue< gkg::GenericObjectList >();

        }
        else
        {

          throw std::runtime_error( "missing attribute 'rise-time'" );

        }



        gradientCharacteristics.resize( shellCount );

        int32_t shell = 0;
        for ( shell = 0; shell < shellCount; shell++ )
        {

          gradientCharacteristics[ shell ].resize( 4 );

          gradientCharacteristics[ shell ][ 0 ] = 
          golGradientMagnitudes[ orientationIndexSets[ shell ].front() ]
                                                                  ->getScalar();

          gradientCharacteristics[ shell ][ 1 ] = 
          golLittleDeltas[ orientationIndexSets[ shell ].front() ]->getScalar();

          gradientCharacteristics[ shell ][ 2 ] = 
          golBigDeltas[ orientationIndexSets[ shell ].front() ]->getScalar();

          gradientCharacteristics[ shell ][ 3 ] = 
          golRiseTimes[ orientationIndexSets[ shell ].front() ]->getScalar();

        }

      }

    }

  }
  GKG_CATCH( "void gkg::MicrostructureMCMCFunction::getMultipleShellBValues( "
             "std::vector< float >& multipleShellBValues, "
             "std::vector< std::vector< float > >& gradientCharacteristics, "
             "std::string& sequenceType, "
             "std::vector< std::vector< int32_t > >& orientationIndexSets, "
             "float deltaBValue ) const" );

}


float gkg::MicrostructureMCMCFunction::getBValue( int32_t index ) const
{

  try
  {
  
    return _bValues[ index ];
  
  }
  GKG_CATCH( 
          "double "
          "gkg::MicrostructureMCMCFunction::getBValue( int32_t index ) const" );

}


const gkg::Vector3d< float >&  
gkg::MicrostructureMCMCFunction::getInputOrientation( int32_t index ) const
{

  try
  {
  
    return _inputOrientations[ index ];
  
  }
  GKG_CATCH( "const gkg::Vector3d< float >&  "
             "gkg::MicrostructureMCMCFunction::getInputOrientation( "
	     "int32_t index ) const" );

}


const gkg::Dictionary& 
gkg::MicrostructureMCMCFunction::getGradientCharacteristics() const
{

  try
  {

    return _gradientCharacteristics;

  }
  GKG_CATCH( "const gkg::Dictionary& "
             "gkg::MicrostructureMCMCFunction::"
             "getGradientCharacteristics() const" );

}


int32_t gkg::MicrostructureMCMCFunction::getOutputOrientationCount() const
{

  try
  {
  
    return _outputOrientationCount;
  
  }
  GKG_CATCH(
         "int32_t "
         "gkg::MicrostructureMCMCFunction::getOutputOrientationCount() const" );

}


const gkg::Vector3d< float >&  
gkg::MicrostructureMCMCFunction::getOutputOrientation( int32_t index ) const
{

  try
  {
  
    return _outputOrientations[ index ];
  
  }
  GKG_CATCH( "const gkg::Vector3d< float >& " 
             "gkg::MicrostructureMCMCFunction::getOutputOrientation( "
	     "int32_t index ) const" );

}


double gkg::MicrostructureMCMCFunction::getNoiseStandardDeviation() const
{

  try
  {

    return _noiseStandardDeviation;

  }
  GKG_CATCH( 
         "double "
         "gkg::MicrostructureMCMCFunction::getNoiseStandardDeviation() const" );

}


double gkg::MicrostructureMCMCFunction::getNoiseVariance() const
{

  try
  {

    return _noiseVariance;

  }
  GKG_CATCH( "double "
             "gkg::MicrostructureMCMCFunction::getNoiseVariance() const" );

}


