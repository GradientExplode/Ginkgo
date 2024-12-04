#include <gkg-fmri-contrast/ContrastEffects.h>
#include <gkg-fmri-contrast/ContrastEffectsContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


//
// class ContrastEffects::Information
//

gkg::ContrastEffects::Information::Information()
{
}


gkg::ContrastEffects::Information::Information(
                            const std::string& theName,
                            const std::vector< std::string >& theRegressorNames,
                            const gkg::Vector& theWeights )
                                  : name( theName ),
                                    regressorNames( theRegressorNames ),
                                    weights( theWeights )
{
}

                                    
gkg::ContrastEffects::Information::Information(
                                const gkg::ContrastEffects::Information& other )
                                  : name( other.name ),
                                    regressorNames( other.regressorNames ),
                                    weights( other.weights )
{
}


gkg::ContrastEffects::Information& 
gkg::ContrastEffects::Information::operator = (
                                const gkg::ContrastEffects::Information& other )
{

  try
  {

    name = other.name;
    regressorNames = other.regressorNames;
    weights = other.weights;

    return *this;

  }
  GKG_CATCH( "gkg::ContrastEffects::Information&  "
             "gkg::ContrastEffects::Information::operator = ( "
             "const gkg::ContrastEffects::Information& other )" );

}


//
// class ContrastEffects
//


gkg::ContrastEffects::ContrastEffects()
                     : gkg::HeaderedObject(),
                       _regressorCount( 0 )
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "ContrastEffects" ) );

}


gkg::ContrastEffects::~ContrastEffects()
{
}


void gkg::ContrastEffects::setParadigm( const gkg::Paradigm& paradigm )
{

  try
  {

    // getting global regressor count
    _regressorCount = paradigm.getRegressorCount();

    // building regressor name lookup table
    int32_t index = 0;
    std::list< Stimulus* >::const_iterator
      s = paradigm.getStimuli().begin(),
      se = paradigm.getStimuli().end();
    while ( s != se )
    {

      _stimuliNameToIndexLut[ ( *s )->getName() ] = index;
      _stimuliIndexToNameLut[ index ] = ( *s )->getName();
      ++ s;
      ++ index;

    }

  }
  GKG_CATCH( "void gkg::ContrastEffects::setParadigm( "
             "const gkg::Paradigm& paradigm )" );

}


int32_t gkg::ContrastEffects::getRegressorCount() const
{

  try
  {

    return _regressorCount;

  }
  GKG_CATCH( "int32_t gkg::ContrastEffects::getRegressorCount() const" );

}


std::string gkg::ContrastEffects::getRegressorName( int32_t index ) const
{

  try
  {

    if ( _regressorCount <= 0 )
    {

      throw std::runtime_error( "_regressorCount must be greater than zero" );

    }

    // remember that _regressorCount is equal to the stimuli count, +1
    // for the constant regressor, and +driftPolynomOrder for the drift
    // correction; so we need to do the following tests
    std::string name = "";
    int32_t stimuliCount = ( int32_t )_stimuliIndexToNameLut.size();

    if ( index < stimuliCount )
    {

      std::map< int32_t, std::string >::const_iterator
        s = _stimuliIndexToNameLut.find( index );
      name = s->second;

    }
    else if ( index == stimuliCount )
    {

      name = "constant";

    }
    else if ( index < _regressorCount )
    {

      name = "drift_order_" + 
             gkg::StringConverter::toString( index - stimuliCount );

    }
    else
    {

      throw std::runtime_error( "bad regressor index" );

    }
    return name;

  }
  GKG_CATCH( "std::string gkg::ContrastEffects::getRegressorName( "
             "int32_t index ) const" );

}


int32_t gkg::ContrastEffects::getRegressorIndex( const std::string& name ) const
{

  try
  {

    int32_t index = -1;

    if ( _stimuliNameToIndexLut.find( name ) != _stimuliNameToIndexLut.end() )
    {

      std::map< std::string, int32_t >::const_iterator
        s = _stimuliNameToIndexLut.find( name );
      index = s->second;

    }
    else
    {

      throw std::runtime_error( "invalid regressor name" );

    }
    return index;

  }
  GKG_CATCH( "int32_t gkg::ContrastEffects::getRegressorIndex( "
             "const std::string& name ) const" );

}


int32_t gkg::ContrastEffects::getContrastCount() const
{

  try
  {

    return ( int32_t )_informationLut.size();

  }
  GKG_CATCH( "int32_t gkg::ContrastEffects::getContrastCount() const" );

}


std::string gkg::ContrastEffects::getContrastName( int32_t index ) const
{

  try
  {

    std::string name = "";
    if ( index > getContrastCount() )
    {

      throw std::runtime_error( "not a valid contrast index" );

    }
    else
    {

      std::map< int32_t, std::string >::const_iterator
        c = _contrastIndexToNameLut.find( index );
      name = c->second;

    }
    return name;

  }
  GKG_CATCH( "std::string gkg::ContrastEffects::getContrastName( "
             "int32_t index ) const" );

}


int32_t gkg::ContrastEffects::getContrastIndex( const std::string& name ) const
{

  try
  {

    int32_t index = -1;

    std::map< std::string, int32_t >::const_iterator
      c = _contrastNameToIndexLut.find( name );
    if ( c != _contrastNameToIndexLut.end() )
    {

      
      index = c->second;

    }
    else
    {

      throw std::runtime_error( "invalid contrast name" );

    }
    return index;

  }
  GKG_CATCH( "int32_t gkg::ContrastEffects::getContrastIndex( "
             "const std::string& name ) const" );

}


void gkg::ContrastEffects::addContrastEffect(
                          const gkg::ContrastEffects::Information& information )
{

  try
  {

    // sanity check
    if ( information.weights.getSize() !=
         ( int32_t )information.regressorNames.size() )
    {

      throw std::runtime_error(
                           "inconsistent regressor names and weights size(s)" );

    }

    // building lut(s) and storing information
    int32_t lastIndex = ( int32_t )_contrastNameToIndexLut.size();

    _contrastNameToIndexLut[ information.name ] = lastIndex;
    _contrastIndexToNameLut[ lastIndex ] = information.name;

    _informationLut[ lastIndex ] = information;


  }
  GKG_CATCH( "void gkg::ContrastEffects::addContrastEffect( "
             "const gkg::ContrastEffects::Information& information )" );


}


const gkg::ContrastEffects::Information& 
gkg::ContrastEffects::getInformation( int32_t index ) const
{

  try
  {

    std::map< int32_t, gkg::ContrastEffects::Information >::const_iterator
      i = _informationLut.find( index );

    if ( i == _informationLut.end() )
    {

      throw std::runtime_error( " not a valid information index" );

    }

    return i->second;

  }
  GKG_CATCH( "const gkg::ContrastEffects::Information& "
             "gkg::ContrastEffects::getInformation( int32_t index ) const" );

}


void gkg::ContrastEffects::process(
                 const gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut,
                 gkg::Volume< float >& volumeOfEffects,
                 gkg::Volume< float >& volumeOfEffectVariances ) const
{

  try
  {

    if ( _regressorCount <= 0 )
    {

      throw std::runtime_error( "_regressorCount must be greater than zero" );

    }

    // collecting size(s)
    int32_t sizeX = kalmanLut.getSizeX();
    int32_t sizeY = kalmanLut.getSizeY();
    int32_t sizeZ = kalmanLut.getSizeZ();
    int32_t contrastCount = getContrastCount();
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    kalmanLut.getResolution( resolution );

    // allocating effect volume
    volumeOfEffects.reallocate( sizeX, sizeY, sizeZ, contrastCount );
    volumeOfEffects.setResolution( resolution );
    volumeOfEffects.fill( 0.0 );

    // allocating variance of effect volume
    volumeOfEffectVariances.reallocate( sizeX, sizeY, sizeZ,
                                        contrastCount * contrastCount );
    volumeOfEffectVariances.setResolution( resolution );
    volumeOfEffectVariances.fill( 0.0 );

    // building contrast matrix and its transpose matrix
    gkg::Matrix C( _regressorCount, contrastCount );
    C.setZero();

    int32_t c, w, weightCount;
    for ( c = 0; c < contrastCount; c++ )
    {

      std::map< int32_t, gkg::ContrastEffects::Information >::const_iterator
        i = _informationLut.find( c );

      if ( i == _informationLut.end() )
      {

        throw std::runtime_error( "informationLut not found!" );

      }

      const gkg::ContrastEffects::Information& information = i->second;

      weightCount = information.weights.getSize();

      for ( w = 0; w < weightCount; w++ )
      {

        std::map< std::string, int32_t >::const_iterator
          s = _stimuliNameToIndexLut.find( information.regressorNames[ w ] );

        if ( s == _stimuliNameToIndexLut.end() )
        {

          throw std::runtime_error( "stimuli name not found for regressor " +
                                    information.regressorNames[ w ] );

        }

        C( s->second, c ) = information.weights( w );

      }

    }

    gkg::Matrix Ct = C.getTransposition();

    // looping over voxel(s)
    int32_t x, y, z, contrast, contrast1, contrast2;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          gkg::Kalman::Parameters* parameters = kalmanLut( x, y, z );
          if ( parameters )
          {

            // calculating effects
            gkg::Vector effects = Ct.getComposition( parameters->X );

            // calculating variance of effects
            gkg::Matrix variances = 
               Ct.getComposition( parameters->varianceOfX ).getComposition( C );

            for ( contrast = 0, contrast1 = 0; 
                  contrast1 < contrastCount;
                  contrast1++ )
            {

              volumeOfEffects( x, y, z, contrast1 ) =
                                                  ( float )effects( contrast1 );

              for ( contrast2 = 0;
                    contrast2 < contrastCount;
                    contrast2++, contrast++ )
              {

                volumeOfEffectVariances( x, y, z, contrast ) =
                                  ( float )( variances( contrast1, contrast2 ) *
                                             parameters->squareScaleParameter );

              }

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::ContrastEffects::process( "
             "const gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut, "
             "gkg::Volume< float >& effects, "
             "gkg::Volume< float >& effectVariances ) const" );

}


void gkg::ContrastEffects::process(
                 const gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut,
                 const std::vector< gkg::Vector3d< int32_t > >& sites,
                 gkg::Volume< float >& volumeOfEffects,
                 gkg::Volume< float >& volumeOfEffectVariances ) const
{

  try
  {

    if ( _regressorCount <= 0 )
    {

      throw std::runtime_error( "_regressorCount must be greater than zero" );

    }

    // collecting size(s)
    int32_t sizeX = kalmanLut.getSizeX();
    int32_t sizeY = kalmanLut.getSizeY();
    int32_t sizeZ = kalmanLut.getSizeZ();
    int32_t contrastCount = getContrastCount();
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    kalmanLut.getResolution( resolution );

    // allocating effect volume
    volumeOfEffects.reallocate( sizeX, sizeY, sizeZ, contrastCount );
    volumeOfEffects.setResolution( resolution );
    volumeOfEffects.fill( 0.0 );

    // allocating variance of effect volume
    volumeOfEffectVariances.reallocate( sizeX, sizeY, sizeZ,
                                        contrastCount * contrastCount );
    volumeOfEffectVariances.setResolution( resolution );
    volumeOfEffectVariances.fill( 0.0 );

    // building contrast matrix and its transpose matrix
    gkg::Matrix C( _regressorCount, contrastCount );
    C.setZero();

    int32_t c, w, weightCount;
    for ( c = 0; c < contrastCount; c++ )
    {

      std::map< int32_t, gkg::ContrastEffects::Information >::const_iterator
        i = _informationLut.find( c );

      if ( i == _informationLut.end() )
      {

        throw std::runtime_error( "informationLut not found!" );

      }

      const gkg::ContrastEffects::Information& information = i->second;

      weightCount = information.weights.getSize();

      for ( w = 0; w < weightCount; w++ )
      {

        std::map< std::string, int32_t >::const_iterator
          s = _stimuliNameToIndexLut.find( information.regressorNames[ w ] );

        if ( s == _stimuliNameToIndexLut.end() )
        {

          throw std::runtime_error( "stimuli name not found for regressor " +
                                    information.regressorNames[ w ] );

        }

        C( s->second, c ) = information.weights( w );

      }

    }

    gkg::ContrastEffectsContext context( kalmanLut,
                                         sites,
                                         C,
                                         volumeOfEffects,
                                         volumeOfEffectVariances );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, sites.size() );

    threadedLoop.launch();

  }
  GKG_CATCH( "void gkg::ContrastEffects::process( "
             "const gkg::Volume< gkg::Kalman::Parameters* >& kalmanLut, "
             "const std::vector< Vector3d< int32_t > >& sites, "
             "gkg::Volume< float >& effects, "
             "gkg::Volume< float >& effectVariances ) const" );

}
