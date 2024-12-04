#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-dmri-sh-basis/SymmetricalSphericalHarmonicsCache.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-coordinates/OrientationSetCache.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/MaximumIndexFilter_i.h>
#include <gkg-processing-algobase/MinMaxFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::OrientationDistributionFunction::OrientationDistributionFunction(
                      gkg::OrientationDistributionFunction::BasisType basisType,
                      const gkg::OrientationSet* orientationSet )
                                     : gkg::DiffusionFeature(),
                                       _basisType( basisType ),
                                       _orientationSet( orientationSet )
{

  if ( orientationSet )
  {

    if ( basisType == gkg::OrientationDistributionFunction::Standard )
    {

      this->_values.resize( orientationSet->getCount() );

    }
    else if ( basisType == 
                    gkg::OrientationDistributionFunction::UnnormalizedStandard )
    {

      this->_values.resize( orientationSet->getCount() );

    }
    else if ( basisType == 
                    gkg::OrientationDistributionFunction::Shore )
    {

      this->_values.resize( orientationSet->getCount() );

    }

  }

}


gkg::OrientationDistributionFunction::OrientationDistributionFunction(
                             const gkg::OrientationDistributionFunction& other )
                                     : gkg::DiffusionFeature( other ),
                                       _basisType( other._basisType ),
                                       _orientationSet( other._orientationSet )
{
}


gkg::OrientationDistributionFunction::~OrientationDistributionFunction()
{
}


gkg::OrientationDistributionFunction& 
gkg::OrientationDistributionFunction::operator=(
                             const gkg::OrientationDistributionFunction& other )
{

  try
  {

    this->gkg::DiffusionFeature::operator=( other );
    _basisType = other._basisType;
    _orientationSet = other._orientationSet;

    return *this;

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction& "
             "gkg::OrientationDistributionFunction::operator=( "
             "const gkg::OrientationDistributionFunction& other )" );

}


gkg::OrientationDistributionFunction::BasisType
gkg::OrientationDistributionFunction::getBasisType() const
{

  try
  {

    return _basisType;

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction::BasisType "
             "gkg::OrientationDistributionFunction::getBasisType() const" );

}


std::string gkg::OrientationDistributionFunction::getBasisTypeName() const
{

  try
  {

    std::string basisTypeName = "";

    if ( _basisType == gkg::OrientationDistributionFunction::Standard )
    {

      basisTypeName = "std";

    }
    else if ( _basisType ==
                    gkg::OrientationDistributionFunction::UnnormalizedStandard )
    {

      basisTypeName = "ustd";

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      basisTypeName = "sh";

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      basisTypeName = "mtsh";

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiWatson )
    {

      basisTypeName = "nw";

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiBingham )
    {

      basisTypeName = "nb";

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::PGSEAxonMapping )
    {

      basisTypeName = "pa";

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
    {

      basisTypeName = "ctoa";

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Shore )
    {

      basisTypeName = "ph";

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Ivim )
    {

      basisTypeName = "iv";

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::AxCaliber )
    {

      basisTypeName = "ac";

    }
    return basisTypeName;

  }
  GKG_CATCH( "std::string "
             "gkg::OrientationDistributionFunction::getBasisTypeName() const" );


}


void gkg::OrientationDistributionFunction::setBasisType(
                     gkg::OrientationDistributionFunction::BasisType basisType )
{

  try
  {

    _basisType = basisType;

  }
  GKG_CATCH( "void gkg::OrientationDistributionFunction::setBasisType( "
             "gkg::OrientationDistributionFunction::BasisType basisType )" );

}


void gkg::OrientationDistributionFunction::setBasisType(
                                              const std::string& basisTypeName )
{

  try
  {

    if ( basisTypeName == "std" )
    {

      _basisType = gkg::OrientationDistributionFunction::Standard;

    }
    else if ( basisTypeName == "ustd" )
    {

      _basisType = gkg::OrientationDistributionFunction::UnnormalizedStandard;

    }
    else if ( basisTypeName == "sh" )
    {

      _basisType = gkg::OrientationDistributionFunction::SphericalHarmonics;

    }
    else if ( basisTypeName == "mtsh" )
    {

      _basisType = gkg::OrientationDistributionFunction::
                                                  MultiTissueSphericalHarmonics;

    }
    else if ( basisTypeName == "nw" )
    {

      _basisType = gkg::OrientationDistributionFunction::NoddiWatson;

    }
    else if ( basisTypeName == "nb" )
    {

      _basisType = gkg::OrientationDistributionFunction::NoddiBingham;

    }
    else if ( basisTypeName == "pa" )
    {

      _basisType = gkg::OrientationDistributionFunction::PGSEAxonMapping;

    }
    else if ( basisTypeName == "ctoa" )
    {

      _basisType = gkg::OrientationDistributionFunction::CTOGSEAxonMapping;

    }
    else if ( basisTypeName == "ph" )
    {

      _basisType = gkg::OrientationDistributionFunction::Shore;

    }
    else if ( basisTypeName == "iv" )
    {

      _basisType = gkg::OrientationDistributionFunction::Ivim;

    }
    else if ( basisTypeName == "ax" )
    {

      _basisType = gkg::OrientationDistributionFunction::AxCaliber;

    }
    else
    {

      throw std::runtime_error( "not a valid basis type name" );

    }

  }
  GKG_CATCH( "void "
             "gkg::OrientationDistributionFunction::setBasisType( "
             "const std::string& name )" );

}

void gkg::OrientationDistributionFunction::setStandardBasisType()
{

  try
  {

    if ( _basisType == gkg::OrientationDistributionFunction::Standard )
    {

      // nothing special to do

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::UnnormalizedStandard )
    {

      _values = getOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      _values = getOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      _values = getOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiWatson )
    {

      _values = getOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiBingham )
    {

      _values = getOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::PGSEAxonMapping )
    {

      _values = getOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
    {

      _values = getOrientationProbabilities();

    }
    else if ( _basisType == gkg::OrientationDistributionFunction::Shore )
    {

      // nothing special to do

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Ivim )
    {

      _values = getOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::AxCaliber )
    {

      _values = getOrientationProbabilities();

    }
    _basisType = gkg::OrientationDistributionFunction::Standard;

  }
  GKG_CATCH( "void "
             "gkg::OrientationDistributionFunction::setStandardBasisType()" );

}

void gkg::OrientationDistributionFunction::setUnnormalizedStandardBasisType()
{

  try
  {

    if ( _basisType == gkg::OrientationDistributionFunction::Standard )
    {

      // nothing special to do

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::UnnormalizedStandard )
    {

      // nothing special to do

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      _values = getUnnormalizedOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      _values = getUnnormalizedOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiWatson )
    {

      _values = getUnnormalizedOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiBingham )
    {

      _values = getUnnormalizedOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::PGSEAxonMapping )
    {

      _values = getUnnormalizedOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
    {

      _values = getUnnormalizedOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Shore )
    {

      // nothing special to do

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Ivim )
    {

      _values = getUnnormalizedOrientationProbabilities();

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::AxCaliber )
    {

      _values = getUnnormalizedOrientationProbabilities();

    }
    _basisType = gkg::OrientationDistributionFunction::UnnormalizedStandard;

  }
  GKG_CATCH( "void "
             "gkg::OrientationDistributionFunction::setStandardBasisType()" );

}


void gkg::OrientationDistributionFunction::updateCache( 
                                      const gkg::OrientationSet& orientationSet,
                                      float apertureAngle ) const
{

  try
  {

    // updating orientation set cache for speeding-up calculation
    gkg::OrientationSetCache::getInstance().update( orientationSet,
                                                    apertureAngle );

    // using a cache system for speeding SH coefficient calculation when
    // used in field(s)
    if ( _basisType ==
         gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                    orientationSet,
                                                    ( int32_t )_values.size() );

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      // in case of multi-tissue spherical harmonics, _values[] contains
      //  <tissueCount> <shCoefficientCount1 > <series of SH coefficients> ...
      //  ............. <shCoefficientCountN > <series of SH coefficients>
      // and it is assumed that the first tissue class is the one of interest
      // (eg white matter in our case)
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                            orientationSet,
                                            ( int32_t )( _values[ 1 ] + 0.5 ) );

    }

  }
  GKG_CATCH( "void gkg::OrientationDistributionFunction::updateCache( "
             "const gkg::OrientationSet& orientationSet, "
             "float apertureAngle )" );

}


void gkg::OrientationDistributionFunction::setOrientationSet(
                                     const gkg::OrientationSet* orientationSet )
{

  try
  {

    if ( orientationSet == 0 )
    {

      throw std::runtime_error( "nul orientation set pointer" );

    }
    _orientationSet = orientationSet;

  }
  GKG_CATCH( "void gkg::OrientationDistributionFunction::setOrientationSet( "
             "const gkg::OrientationSet* orientationSet )" );

}


void
gkg::OrientationDistributionFunction::getOrientationProbabilities(
                                     std::vector< float >& probabilities ) const
{


  try
  {

    if ( _basisType == gkg::OrientationDistributionFunction::Standard )
    {

      probabilities.resize( this->_values.size(), 0.0f );
      std::vector< float >::const_iterator
        v = this->_values.begin(),
        ve = this->_values.end();
      std::vector< float >::iterator p = probabilities.begin();
      while ( v != ve )
      {

        if ( *v > 0.0f )
        {

          *p = *v;

        }
        ++ v;
        ++ p;

      }

    }
    else if ( _basisType == 
              gkg::OrientationDistributionFunction::UnnormalizedStandard )
    {

      // collecting unnormalized probabilities
      probabilities.resize( this->_values.size(), 0.0f );
      std::vector< float >::const_iterator
        v = this->_values.begin(),
        ve = this->_values.end();
      std::vector< float >::iterator p = probabilities.begin();
      float sum = 0.0;
      while ( v != ve )
      {

        if ( *v > 0.0f )
        {

          *p = *v;
          sum += *v;

        }
        ++ v;
        ++ p;

      }

      if ( sum > 0.0 )
      {

        std::vector< float >::iterator
          p = probabilities.begin(),
          pe = probabilities.end();
        while ( p != pe )
        {

          *p /= sum;
          ++ p;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      int32_t shCoefficientCount = ( int32_t )this->_values.size();
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o = 0;
      int32_t s = 0;
      float sum = 0.0;

      // using a cache system for speeding SH coefficient calculation when
      // used in field(s)
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );

      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      // processing probabilities
      probabilities.resize( orientationCount, 0.0f );
      for ( o = 0; o < orientationCount; o++ )
      {

        float p = 0.0f;
        float* val = ( float* )&this->_values[ 0 ];
        float* shCoef = ( float* )&shCoefficients[ o ][ 0 ];
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          p += *val++ * *shCoef++;

        }
        if ( p > 0.0 )
        {

          sum += p;
          probabilities[ o ] = p;

        }

      }

      // normalizing
      if ( sum > 0.0 )
      {

        for ( o = 0; o < orientationCount; o++ )
        {

          probabilities[ o ] /= sum;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      // in case of multi-tissue spherical harmonics, _values[] contains
      //  <tissueCount> <shCoefficientCount1 > <series of SH coefficients> ...
      //  ............. <shCoefficientCountN > <series of SH coefficients>
      // and it is assumed that the first tissue class is the one of interest
      // (eg white matter in our case)
      int32_t shCoefficientCount = ( int32_t )( this->_values[ 1 ] + 0.5 );
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o;
      int32_t s;
      float sum = 0.0;

      // using a cache system for speeding SH coefficient calculation when
      // used in field(s)
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      // processing probabilities
      probabilities.resize( orientationCount, 0.0f );
      for ( o = 0; o < orientationCount; o++ )
      {

        float p = 0.0f;
        float* val = ( float* )&( this->_values[ 2 ] );
        float* shCoef = ( float* )&shCoefficients[ o ][ 0 ];
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          p += *val++ * *shCoef++;

        }
        if ( p > 0.0 )
        {

          sum += p;
          probabilities[ o ] = p;

        }

      }

      // normalizing
      if ( sum > 0.0 )
      {

        for ( o = 0; o < orientationCount; o++ )
        {

          probabilities[ o ] /= sum;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiWatson )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 3 ],
                                                   this->_values[ 4 ],
                                                   this->_values[ 5 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 2 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;
      float sum = 0.0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )factory->getWatsonDistribution(
                                          kappa,
                                          principalOrientation,
                                          _orientationSet->getOrientation( o ) );
        sum += probabilities[ o ];

      }

      // normalizing
      if ( sum > 0.0 )
      {

        for ( o = 0; o < orientationCount; o++ )
        {

          probabilities[ o ] /= sum;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiBingham )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 4 ],
                                                   this->_values[ 5 ],
                                                   this->_values[ 6 ] );

      // extracting secondary orientation
      gkg::Vector3d< float > secondaryOrientation( this->_values[ 7 ],
                                                   this->_values[ 8 ],
                                                   this->_values[ 9 ] );

      // extracting kappa1 and kappa2
      double kappa1 = ( double )this->_values[ 2 ];
      double kappa2 = ( double )this->_values[ 3 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;
      float sum = 0.0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )factory->getBinghamDistribution(
                                          kappa1,
                                          kappa2,
                                          principalOrientation,
                                          secondaryOrientation,
                                          _orientationSet->getOrientation( o ) );
        sum += probabilities[ o ];

      }

      // normalizing
      if ( sum > 0.0 )
      {

        for ( o = 0; o < orientationCount; o++ )
        {

          probabilities[ o ] /= sum;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::PGSEAxonMapping )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;
      float sum = 0.0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )factory->getWatsonDistribution(
                                          kappa,
                                          principalOrientation,
                                          _orientationSet->getOrientation( o ) );
        sum += probabilities[ o ];

      }

      // normalizing
      if ( sum > 0.0 )
      {

        for ( o = 0; o < orientationCount; o++ )
        {

          probabilities[ o ] /= sum;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;
      float sum = 0.0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )factory->getWatsonDistribution(
                                          kappa,
                                          principalOrientation,
                                          _orientationSet->getOrientation( o ) );
        sum += probabilities[ o ];

      }

      // normalizing
      if ( sum > 0.0 )
      {

        for ( o = 0; o < orientationCount; o++ )
        {

          probabilities[ o ] /= sum;

        }

      }

    }
    else if ( _basisType == gkg::OrientationDistributionFunction::Shore )
    {

      probabilities = this->_values;

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Ivim )
    {

      gkg::Tensor tensor( this->_values[ 3 ],
                          this->_values[ 4 ],
                          this->_values[ 5 ],
                          this->_values[ 6 ],
                          this->_values[ 7 ],
                          this->_values[ 8 ] );

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;
      float sum = 0.0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )tensor.getDisplacementProbability(
                                         _orientationSet->getOrientation( o ) );
        sum += probabilities[ o ];

      }

      // normalizing
      if ( sum > 0.0 )
      {

        for ( o = 0; o < orientationCount; o++ )
        {

          probabilities[ o ] /= sum;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::AxCaliber )
    {

      // we are just representing a sphere of diameter equal to the mean axon
      // diameter stemming from the gamma distribution 

      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;
      float sum = 0.0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )this->_values[ 5 ];
        sum += probabilities[ o ];

      }

      // normalizing
      if ( sum > 0.0 )
      {

        for ( o = 0; o < orientationCount; o++ )
        {

          probabilities[ o ] /= sum;

        }

      }

    }

  }
  GKG_CATCH( "void "
             "gkg::OrientationDistributionFunction::"
             "getOrientationProbabilities( "
             "std::vector< float >& probabilities ) const" );

}


std::vector< float > 
gkg::OrientationDistributionFunction::getOrientationProbabilities() const
{


  try
  {

    std::vector< float > orientationProbabilities;

    this->getOrientationProbabilities( orientationProbabilities );

    return orientationProbabilities;

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::OrientationDistributionFunction::"
             "getOrientationProbabilities() const" );

}


void 
gkg::OrientationDistributionFunction::
                           getOrientationProbabilitiesWithSpecificNormalization(
                                 std::vector< float >& orientationProbabilities,
                                 float minimumProbability,
                                 float maximumProbability ) const
{


  try
  {

    orientationProbabilities = this->getOrientationProbabilities();

    gkg::MinMaxFilter< std::vector< float >, float > minMaxFilter;
    std::pair< float, float > localMinMaxProbability;

    minMaxFilter.filter( orientationProbabilities,
                         localMinMaxProbability );


    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( localMinMaxProbability.first,
                localMinMaxProbability.second,
                minimumProbability,
                maximumProbability );
    rescaler.rescale( orientationProbabilities, orientationProbabilities );

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::OrientationDistributionFunction::"
             "getOrientationProbabilitiesWithSpecificNormalization( "
             "float minimumProbability, "
             "float maximumProbability ) const" );

}


std::vector< float > 
gkg::OrientationDistributionFunction::getUnnormalizedOrientationProbabilities() 
                                                                           const
{


  try
  {

    std::vector< float > probabilities;

    if ( _basisType == gkg::OrientationDistributionFunction::Standard )
    {

      probabilities = this->_values;

    }
    else if ( _basisType == 
              gkg::OrientationDistributionFunction::UnnormalizedStandard )
    {

      probabilities = this->_values;

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      int32_t shCoefficientCount = ( int32_t )this->_values.size();
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o;
      int32_t s;

      // using a cache system for speeding SH coefficient calculation when
      // used in field(s)
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      // processing probabilities
      probabilities.resize( orientationCount );
      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = 0.0;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          probabilities[ o ] += ( float )this->_values[ s ] *
                                         shCoefficients[ o ][ s ];

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      // in case of multi-tissue spherical harmonics, _values[] contains
      //  <tissueCount> <shCoefficientCount1 > <series of SH coefficients> ...
      //  ............. <shCoefficientCountN > <series of SH coefficients>
      // and it is assumed that the first tissue class is the one of interest
      // (eg white matter in our case)

      int32_t shCoefficientCount = ( int32_t )( this->_values[ 1 ] + 0.5 );
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o;
      int32_t s;

      // using a cache system for speeding SH coefficient calculation when
      // used in field(s)
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      // processing probabilities
      probabilities.resize( orientationCount );
      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = 0.0;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          probabilities[ o ] += ( float )this->_values[ 2 + s ] *
                                         shCoefficients[ o ][ s ];

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiWatson )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 3 ],
                                                   this->_values[ 4 ],
                                                   this->_values[ 5 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 2 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation( o ) );

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiBingham )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 4 ],
                                                   this->_values[ 5 ],
                                                   this->_values[ 6 ] );

      // extracting secondary orientation
      gkg::Vector3d< float > secondaryOrientation( this->_values[ 7 ],
                                                   this->_values[ 8 ],
                                                   this->_values[ 9 ] );

      // extracting kappa1 and kappa2
      double kappa1 = ( double )this->_values[ 2 ];
      double kappa2 = ( double )this->_values[ 3 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )factory->getBinghamDistribution(
                                          kappa1,
                                          kappa2,
                                          principalOrientation,
                                          secondaryOrientation,
                                          _orientationSet->getOrientation( o ) );

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::PGSEAxonMapping )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation( o ) );

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation( o ) );

      }

    }
    else if ( _basisType == gkg::OrientationDistributionFunction::Shore )
    {

      probabilities = this->_values;

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Ivim )
    {

      gkg::Tensor tensor( this->_values[ 3 ],
                          this->_values[ 4 ],
                          this->_values[ 5 ],
                          this->_values[ 6 ],
                          this->_values[ 7 ],
                          this->_values[ 8 ] );

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      probabilities.resize( orientationCount );
      int32_t o = 0;

      for ( o = 0; o < orientationCount; o++ )
      {

        probabilities[ o ] = ( float )tensor.getDisplacementProbability(
                                         _orientationSet->getOrientation( o ) );

      }

    }

    return probabilities;

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::OrientationDistributionFunction::"
             "getOrientationProbabilities() const" );

}


std::vector< float > 
gkg::OrientationDistributionFunction::getUnnormalizedOrientationProbabilities(
                           const gkg::Vector3d< float >& coneUnitaryOrientation,
                           float apertureAngle,
                           float* minimumProbability,
                           float* maximumProbability ) const
{


  try
  {

    int32_t coneUnitaryOrientationIndex =
      _orientationSet->getNearestOrientationIndex( coneUnitaryOrientation );
    return getUnnormalizedOrientationProbabilities( coneUnitaryOrientationIndex,
                                                    apertureAngle,
                                                    minimumProbability,
                                                    maximumProbability );

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::OrientationDistributionFunction::"
             "getUnnormalizedOrientationProbabilities( "
             "const gkg::Vector3d< float >& coneUnitaryOrientation, "
             "float apertureAngle, "
             "float* minimumProbability, "
             "float* maximumProbability ) const" );

}


std::vector< float > 
gkg::OrientationDistributionFunction::getUnnormalizedOrientationProbabilities(
                                            int32_t coneUnitaryOrientationIndex,
                                            float /* apertureAngle */,
                                            float* minimumProbability,
                                            float* maximumProbability ) const
{


  try
  {


    // updating orientation set cache for speeding-up calculation
    //gkg::OrientationSetCache::getInstance().update( *_orientationSet,
    //                                                apertureAngle );
    if ( gkg::OrientationSetCache::getInstance().getOrientationSet() !=
         _orientationSet )
    {

      throw std::runtime_error( "Cache error : bad orientation sets." );

    }

    // collecting the orientation indices contained in the solid angle
    const std::vector< int32_t >& solidAngleOrientationIndices =
      gkg::OrientationSetCache::getInstance().getSolidAngleOrientationIndices(
                                                  coneUnitaryOrientationIndex );

    // collecting solid angle orientation count
    int32_t solidAngleOrientationIndexCount =
      ( int32_t )solidAngleOrientationIndices.size();

    // allocating unnormalized probability vector
    std::vector< float >
      unnormalizedProbabilities( solidAngleOrientationIndexCount );

    // initializing minimum and maximum probabilities if required
    if ( minimumProbability )
    {

      *minimumProbability = +1e38;

    }
    if ( maximumProbability )
    {

      *maximumProbability = -1e38;

    }

    // processing probabilities
    if ( ( _basisType == gkg::OrientationDistributionFunction::Standard ) ||
         ( _basisType ==
           gkg::OrientationDistributionFunction::UnnormalizedStandard ) ||
         ( _basisType ==
           gkg::OrientationDistributionFunction::Shore ) )
    {

      int32_t i = 0;
      float probability = 0.0;
      float minProba = 1e38;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = this->_values[ solidAngleOrientationIndices[ i ] ];
        unnormalizedProbabilities[ i ] = probability;

        if ( probability < minProba )
        {

          minProba = probability;

        }
        if ( probability > maxProba )
        {

          maxProba = probability;

        }

      }

      if ( minimumProbability )
      {

        *minimumProbability = minProba;

      }
      if ( maximumProbability )
      {

        *maximumProbability = maxProba;

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      int32_t shCoefficientCount = ( int32_t )this->_values.size();
      int32_t i;
      int32_t s;

      // using a cache system for speeding SH coefficient calculation when
      // used in field(s)
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      // processing probabilities
      float probability = 0.0;
      float minProba = 1e38;
      float maxProba = -1e38;

      int32_t* saIdx = (int32_t*)&solidAngleOrientationIndices[ 0 ];

      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        float* val = ( float* )&this->_values[ 0 ];
        float* shCoef = ( float* )&shCoefficients[ *saIdx++ ][ 0 ];
        probability = 0.0;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          probability += *val++ * *shCoef++;

        }
        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minProba )
        {

          minProba = probability;

        }
        if ( probability > maxProba )
        {

          maxProba = probability;

        }

      }

      if ( minimumProbability )
      {

        *minimumProbability = minProba;

      }
      if ( maximumProbability )
      {

        *maximumProbability = maxProba;

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      // in case of multi-tissue spherical harmonics, _values[] contains
      //  <tissueCount> <shCoefficientCount1 > <series of SH coefficients> ...
      //  ............. <shCoefficientCountN > <series of SH coefficients>
      // and it is assumed that the first tissue class is the one of interest
      // (eg white matter in our case)

      int32_t shCoefficientCount = ( int32_t )( this->_values[ 1 ] + 0.5 );
      int32_t i;
      int32_t s;

      // using a cache system for speeding SH coefficient calculation when
      // used in field(s)
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      // processing probabilities
      float probability = 0.0;
      float minProba = 1e38;
      float maxProba = -1e38;

      int32_t* saIdx = (int32_t*)&solidAngleOrientationIndices[ 0 ];

      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        float* val = ( float* )&( this->_values[ 2 ] );
        float* shCoef = ( float* )&shCoefficients[ *saIdx++ ][ 0 ];
        probability = 0.0;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          probability += *val++ * *shCoef++;

        }
        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minProba )
        {

          minProba = probability;

        }
        if ( probability > maxProba )
        {

          maxProba = probability;

        }

      }

      if ( minimumProbability )
      {

        *minimumProbability = minProba;

      }
      if ( maximumProbability )
      {

        *maximumProbability = maxProba;

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiWatson )
    {


      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 4 ],
                                                   this->_values[ 5 ],
                                                   this->_values[ 6 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 2 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      float minProba = 1e38;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minProba )
        {

          minProba = probability;

        }
        if ( probability > maxProba )
        {

          maxProba = probability;

        }

      }

      if ( minimumProbability )
      {

        *minimumProbability = minProba;

      }
      if ( maximumProbability )
      {

        *maximumProbability = maxProba;

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiBingham )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 4 ],
                                                   this->_values[ 5 ],
                                                   this->_values[ 6 ] );

      // extracting secondary orientation
      gkg::Vector3d< float > secondaryOrientation( this->_values[ 7 ],
                                                   this->_values[ 8 ],
                                                   this->_values[ 9 ] );

      // extracting kappa1 and kappa2
      double kappa1 = ( double )this->_values[ 2 ];
      double kappa2 = ( double )this->_values[ 3 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      float minProba = 1e38;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getBinghamDistribution(
                                         kappa1,
                                         kappa2,
                                         principalOrientation,
                                         secondaryOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minProba )
        {

          minProba = probability;

        }
        if ( probability > maxProba )
        {

          maxProba = probability;

        }

      }

      if ( minimumProbability )
      {

        *minimumProbability = minProba;

      }
      if ( maximumProbability )
      {

        *maximumProbability = maxProba;

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::PGSEAxonMapping )
    {


      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      float minProba = 1e38;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minProba )
        {

          minProba = probability;

        }
        if ( probability > maxProba )
        {

          maxProba = probability;

        }

      }

      if ( minimumProbability )
      {

        *minimumProbability = minProba;

      }
      if ( maximumProbability )
      {

        *maximumProbability = maxProba;

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
    {


      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      float minProba = 1e38;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minProba )
        {

          minProba = probability;

        }
        if ( probability > maxProba )
        {

          maxProba = probability;

        }

      }

      if ( minimumProbability )
      {

        *minimumProbability = minProba;

      }
      if ( maximumProbability )
      {

        *maximumProbability = maxProba;

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Ivim )
    {

      gkg::Tensor tensor( this->_values[ 3 ],
                          this->_values[ 4 ],
                          this->_values[ 5 ],
                          this->_values[ 6 ],
                          this->_values[ 7 ],
                          this->_values[ 8 ] );

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      float minProba = 1e38;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )tensor.getDisplacementProbability(
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minProba )
        {

          minProba = probability;

        }
        if ( probability > maxProba )
        {

          maxProba = probability;

        }

      }

      if ( minimumProbability )
      {

        *minimumProbability = minProba;

      }
      if ( maximumProbability )
      {

        *maximumProbability = maxProba;

      }

    }

    return unnormalizedProbabilities;

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::OrientationDistributionFunction::"
             "getUnnormalizedOrientationProbabilities( "
             "int32_t coneUnitaryOrientationIndex, "
             "float apertureAngle, "
             "float* minimumProbability, "
             "float* maximumProbability ) const" );

}


std::vector< float > 
gkg::OrientationDistributionFunction::
     getUnnormalizedOrientationProbabilitiesFast(
                                            int32_t coneUnitaryOrientationIndex,
                                            float& minimumProbability,
                                            float& maximumProbability ) const
{


  try
  {


    // collecting the orientation indices contained in the solid angle
    const std::vector< int32_t >& solidAngleOrientationIndices =
      gkg::OrientationSetCache::getInstance().getSolidAngleOrientationIndices(
                                                  coneUnitaryOrientationIndex );

    // collecting solid angle orientation count
    int32_t solidAngleOrientationIndexCount =
                                 ( int32_t )solidAngleOrientationIndices.size();

    // allocating unnormalized probability vector
    std::vector< float > unnormalizedProbabilities(
                                              solidAngleOrientationIndexCount );

    // initializing minimum and maximum probabilities if required
    minimumProbability = +1e38;
    maximumProbability = -1e38;

    // processing probabilities
    if ( ( _basisType == gkg::OrientationDistributionFunction::Standard ) ||
         ( _basisType ==
           gkg::OrientationDistributionFunction::UnnormalizedStandard ) ||
         ( _basisType ==
           gkg::OrientationDistributionFunction::Shore ) )
    {

      int32_t i = 0;
      float probability = 0.0;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = this->_values[ solidAngleOrientationIndices[ i ] ];
        if ( probability < 0.0f )
        {

          probability = 0.0f;

        }
        unnormalizedProbabilities[ i ] = probability;

        if ( probability < minimumProbability )
        {

          minimumProbability = probability;

        }
        if ( probability > maximumProbability )
        {

          maximumProbability = probability;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      int32_t shCoefficientCount = ( int32_t )this->_values.size();
      int32_t i;
      int32_t s;

      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      // processing probabilities
      int32_t* saIdx = (int32_t*)&solidAngleOrientationIndices[ 0 ];
      float* val0 = ( float* )&this->_values[ 0 ];

      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        float* val = val0;
        float* shCoef = ( float* )&shCoefficients[ *saIdx++ ][ 0 ];
        float probability = 0.0f;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          probability += *val++ * *shCoef++;

        }
        if ( probability < 0.0f )
        {

          probability = 0.0f;

        }
        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minimumProbability )
        {

          minimumProbability = probability;

        }
        if ( probability > maximumProbability )
        {

          maximumProbability = probability;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      // in case of multi-tissue spherical harmonics, _values[] contains
      //  <tissueCount> <shCoefficientCount1 > <series of SH coefficients> ...
      //  ............. <shCoefficientCountN > <series of SH coefficients>
      // and it is assumed that the first tissue class is the one of interest
      // (eg white matter in our case)

      int32_t shCoefficientCount = ( int32_t )( this->_values[ 1 ] + 0.5 );
      int32_t i;
      int32_t s;

      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      // processing probabilities
      int32_t* saIdx = (int32_t*)&solidAngleOrientationIndices[ 0 ];
      float* val0 = ( float* )&( this->_values[ 2 ] );

      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        float* val = val0;
        float* shCoef = ( float* )&shCoefficients[ *saIdx++ ][ 0 ];
        float probability = 0.0f;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          probability += *val++ * *shCoef++;

        }
        if ( probability < 0.0f )
        {

          probability = 0.0f;

        }
        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minimumProbability )
        {

          minimumProbability = probability;

        }
        if ( probability > maximumProbability )
        {

          maximumProbability = probability;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiWatson )
    {


      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 4 ],
                                                   this->_values[ 5 ],
                                                   this->_values[ 6 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 2 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        if ( probability < 0.0f )
        {

          probability = 0.0f;

        }
        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minimumProbability )
        {

          minimumProbability = probability;

        }
        if ( probability > maximumProbability )
        {

          maximumProbability = probability;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiBingham )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 4 ],
                                                   this->_values[ 5 ],
                                                   this->_values[ 6 ] );

      // extracting secondary orientation
      gkg::Vector3d< float > secondaryOrientation( this->_values[ 7 ],
                                                   this->_values[ 8 ],
                                                   this->_values[ 9 ] );

      // extracting kappa1 and kappa2
      double kappa1 = ( double )this->_values[ 2 ];
      double kappa2 = ( double )this->_values[ 3 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getBinghamDistribution(
                                         kappa1,
                                         kappa2,
                                         principalOrientation,
                                         secondaryOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        if ( probability < 0.0f )
        {

          probability = 0.0f;

        }
        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minimumProbability )
        {

          minimumProbability = probability;

        }
        if ( probability > maximumProbability )
        {

          maximumProbability = probability;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::PGSEAxonMapping )
    {


      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        if ( probability < 0.0f )
        {

          probability = 0.0f;

        }
        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minimumProbability )
        {

          minimumProbability = probability;

        }
        if ( probability > maximumProbability )
        {

          maximumProbability = probability;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
    {


      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        if ( probability < 0.0f )
        {

          probability = 0.0f;

        }
        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minimumProbability )
        {

          minimumProbability = probability;

        }
        if ( probability > maximumProbability )
        {

          maximumProbability = probability;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Ivim )
    {

      gkg::Tensor tensor( this->_values[ 3 ],
                          this->_values[ 4 ],
                          this->_values[ 5 ],
                          this->_values[ 6 ],
                          this->_values[ 7 ],
                          this->_values[ 8 ] );

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )tensor.getDisplacementProbability(
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        if ( probability < 0.0f )
        {

          probability = 0.0f;

        }
        unnormalizedProbabilities[ i ] = probability;
        if ( probability < minimumProbability )
        {

          minimumProbability = probability;

        }
        if ( probability > maximumProbability )
        {

          maximumProbability = probability;

        }

      }

    }

    return unnormalizedProbabilities;

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::OrientationDistributionFunction::"
             "getUnnormalizedOrientationProbabilitiesFast( "
             "int32_t coneUnitaryOrientationIndex, "
             "float& minimumProbability, "
             "float& maximumProbability ) const" );

}


int32_t 
gkg::OrientationDistributionFunction::
                                getUnnormalizedOrientationProbabilitiesMaxIndex(
                                     int32_t coneUnitaryOrientationIndex ) const
{


  try
  {

    if ( _basisType ==
         gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      int32_t shCoefficientCount = ( int32_t )this->_values.size();

      // using a cache system for speeding SH coefficient calculation when
      // used in field(s)
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      // in case of multi-tissue spherical harmonics, _values[] contains
      //  <tissueCount> <shCoefficientCount1 > <series of SH coefficients> ...
      //  ............. <shCoefficientCountN > <series of SH coefficients>
      // and it is assumed that the first tissue class is the one of interest
      // (eg white matter in our case)

      int32_t shCoefficientCount = ( int32_t )( this->_values[ 1 ] + 0.5 );

      // using a cache system for speeding SH coefficient calculation when
      // used in field(s)
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );

    }

    return getUnnormalizedOrientationProbabilitiesMaxIndexFast( 
                                                  coneUnitaryOrientationIndex );

  }
  GKG_CATCH( "int32_t "
             "gkg::OrientationDistributionFunction::"
             "getUnnormalizedOrientationProbabilitiesMaxIndex( "
             "int32_t coneUnitaryOrientationIndex ) const" );

}


int32_t 
gkg::OrientationDistributionFunction::
                            getUnnormalizedOrientationProbabilitiesMaxIndexFast(
                                     int32_t coneUnitaryOrientationIndex ) const
{


  try
  {

    // collecting the orientation indices contained in the solid angle
    const std::vector< int32_t >& solidAngleOrientationIndices =
      gkg::OrientationSetCache::getInstance().getSolidAngleOrientationIndices(
                                                  coneUnitaryOrientationIndex );

    // collecting solid angle orientation count
    int32_t solidAngleOrientationIndexCount =
      ( int32_t )solidAngleOrientationIndices.size();

    // initializing maximum index
    int32_t maximumIndex = 0;

    // processing probabilities
    if ( ( _basisType == gkg::OrientationDistributionFunction::Standard ) ||
         ( _basisType ==
           gkg::OrientationDistributionFunction::UnnormalizedStandard ) ||
         ( _basisType ==
           gkg::OrientationDistributionFunction::Shore ) )
    {

      int32_t i = 0;
      float probability = 0.0;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = this->_values[ solidAngleOrientationIndices[ i ] ];

        if ( probability > maxProba )
        {

          maxProba = probability;
          maximumIndex = i;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      int32_t shCoefficientCount = ( int32_t )this->_values.size();
      int32_t i;
      int32_t s;

      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      // processing probabilities
      float maxProba = -1e38;

      int32_t* saIdx = (int32_t*)&solidAngleOrientationIndices[ 0 ];
      float* val0 = ( float* )&this->_values[ 0 ];

      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        float* val = val0;
        float* shCoef = ( float* )&shCoefficients[ *saIdx++ ][ 0 ];
        float probability = 0.0f;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          probability += *val++ * *shCoef++;

        }
        if ( probability > maxProba )
        {

          maxProba = probability;
          maximumIndex = i;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      // in case of multi-tissue spherical harmonics, _values[] contains
      //  <tissueCount> <shCoefficientCount1 > <series of SH coefficients> ...
      //  ............. <shCoefficientCountN > <series of SH coefficients>
      // and it is assumed that the first tissue class is the one of interest
      // (eg white matter in our case)

      int32_t shCoefficientCount = ( int32_t )( this->_values[ 1 ] + 0.5 );
      int32_t i;
      int32_t s;

      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      // processing probabilities
      float maxProba = -1e38;

      int32_t* saIdx = (int32_t*)&solidAngleOrientationIndices[ 0 ];
      float* val0 = ( float* )&( this->_values[ 2 ] );

      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        float* val = val0;
        float* shCoef = ( float* )&shCoefficients[ *saIdx++ ][ 0 ];
        float probability = 0.0f;
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          probability += *val++ * *shCoef++;

        }
        if ( probability > maxProba )
        {

          maxProba = probability;
          maximumIndex = i;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiWatson )
    {


      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 4 ],
                                                   this->_values[ 5 ],
                                                   this->_values[ 6 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 2 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        if ( probability > maxProba )
        {

          maxProba = probability;
          maximumIndex = i;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiBingham )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 4 ],
                                                   this->_values[ 5 ],
                                                   this->_values[ 6 ] );

      // extracting secondary orientation
      gkg::Vector3d< float > secondaryOrientation( this->_values[ 7 ],
                                                   this->_values[ 8 ],
                                                   this->_values[ 9 ] );

      // extracting kappa1 and kappa2
      double kappa1 = ( double )this->_values[ 2 ];
      double kappa2 = ( double )this->_values[ 3 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getBinghamDistribution(
                                         kappa1,
                                         kappa2,
                                         principalOrientation,
                                         secondaryOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        if ( probability > maxProba )
        {

          maxProba = probability;
          maximumIndex = i;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::PGSEAxonMapping )
    {


      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        if ( probability > maxProba )
        {

          maxProba = probability;
          maximumIndex = i;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
    {


      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        if ( probability > maxProba )
        {

          maxProba = probability;
          maximumIndex = i;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Ivim )
    {

      gkg::Tensor tensor( this->_values[ 3 ],
                          this->_values[ 4 ],
                          this->_values[ 5 ],
                          this->_values[ 6 ],
                          this->_values[ 7 ],
                          this->_values[ 8 ] );

      // computing sub-set of probabilities
      int32_t i = 0;
      float probability = 0.0;
      float maxProba = -1e38;
      for ( i = 0; i < solidAngleOrientationIndexCount; i++ )
      {

        probability = ( float )tensor.getDisplacementProbability(
                                         _orientationSet->getOrientation(
                                          solidAngleOrientationIndices[ i ] ) );

        if ( probability > maxProba )
        {

          maxProba = probability;
          maximumIndex = i;

        }

      }

    }

    return maximumIndex;

  }
  GKG_CATCH( "int32_t "
             "gkg::OrientationDistributionFunction::"
             "getUnnormalizedOrientationProbabilitiesMaxIndexFast( "
             "int32_t coneUnitaryOrientationIndex ) const" );

}


float gkg::OrientationDistributionFunction::getOrientationProbability(
                                                           int32_t index ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= ( int32_t )_orientationSet->getCount() ) )
    {

      throw std::runtime_error( "invalid index" );

    }

#endif

    float probability = 0.0;

    if ( _basisType == gkg::OrientationDistributionFunction::Standard )
    {

      probability = this->_values[ index ];

    }
    else if ( ( _basisType ==
                gkg::OrientationDistributionFunction::UnnormalizedStandard ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::SphericalHarmonics ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::
                                              MultiTissueSphericalHarmonics ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::NoddiWatson ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::NoddiBingham ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::PGSEAxonMapping ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::CTOGSEAxonMapping ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::Ivim ) )
    {

      std::vector< float > probabilities = getOrientationProbabilities();
      probability = probabilities[ index ];

    }
    else if ( _basisType == gkg::OrientationDistributionFunction::Shore )
    {

      probability = this->_values[ index ];

    }

    return probability;

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getOrientationProbability( int32_t index ) const" );

}


float gkg::OrientationDistributionFunction::getOrientationProbability(
                        const gkg::Vector3d< float >& unitaryOrientation ) const
{

  try
  {

    float probability = 0.0;

    if ( _basisType == gkg::OrientationDistributionFunction::Standard )
    {

      probability = this->_values[ _orientationSet->getNearestOrientationIndex(
                                                         unitaryOrientation ) ];

    }
    else if ( ( _basisType ==
                gkg::OrientationDistributionFunction::UnnormalizedStandard ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::SphericalHarmonics ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::
                                              MultiTissueSphericalHarmonics ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::NoddiWatson ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::NoddiBingham ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::PGSEAxonMapping ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::CTOGSEAxonMapping ) ||
              ( _basisType ==
                gkg::OrientationDistributionFunction::Ivim ) )
    {

      std::vector< float > probabilities = getOrientationProbabilities();
      probability = probabilities[ _orientationSet->getNearestOrientationIndex(
                                                         unitaryOrientation ) ];

    }
    else if ( _basisType == gkg::OrientationDistributionFunction::Shore )
    {

      probability = this->_values[ _orientationSet->getNearestOrientationIndex(
                                                         unitaryOrientation ) ];

    }

    return probability;

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getOrientationProbability( "
             "const gkg::Vector3d< float >& unitaryOrientation ) const" );

}


float gkg::OrientationDistributionFunction::
                            getOrientationProbabilityWithSpecificNormalization(
                                              int32_t index,
                                              float minimumProbability,
                                              float maximumProbability ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= ( int32_t )_orientationSet->getCount() ) )
    {

      throw std::runtime_error( "invalid index" );

    }

#endif

    std::vector< float > probabilities;
    getOrientationProbabilitiesWithSpecificNormalization( probabilities,
                                                          minimumProbability,
                                                          maximumProbability );
    return probabilities[ index ];

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getOrientationProbabilityWithSpecificNormalization( "
             "int32_t index, "
             "float minimumProbability, "
             "float maximumProbability ) const" );

}


float gkg::OrientationDistributionFunction::
                             getOrientationProbabilityWithSpecificNormalization(
                               const gkg::Vector3d< float >& unitaryOrientation,
                               float minimumProbability,
                               float maximumProbability ) const
{

  try
  {

    return getOrientationProbabilityWithSpecificNormalization(
              _orientationSet->getNearestOrientationIndex( unitaryOrientation ),
              minimumProbability,
              maximumProbability );

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getOrientationProbabilityWithSpecificNormalization( "
             "const gkg::Vector3d< float >& unitaryOrientation, "
             "float minimumProbability, "
             "float maximumProbability ) const" );

}


float gkg::OrientationDistributionFunction::
                             getOrientationProbabilityWithSpecificNormalization(
                               const gkg::Vector3d< float >& unitaryOrientation,
                               float globalMinimumProbability,
                               float globalMaximumProbability,
                               std::vector< float >& orientationProbabilities,
                               float& localMinimumProbability,
                               float& localMaximumProbability ) const
{

  try
  {

    int32_t index = _orientationSet->getNearestOrientationIndex(
                                                           unitaryOrientation );

    orientationProbabilities = this->getOrientationProbabilities();

    gkg::MinMaxFilter< std::vector< float >, float > minMaxFilter;
    std::pair< float, float > localMinMaxProbability;

    minMaxFilter.filter( orientationProbabilities,
                         localMinMaxProbability );


    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( localMinMaxProbability.first,
                localMinMaxProbability.second,
                globalMinimumProbability,
                globalMaximumProbability );
    rescaler.rescale( orientationProbabilities, orientationProbabilities );

    localMinimumProbability = localMinMaxProbability.first;
    localMaximumProbability = localMinMaxProbability.second;

    return orientationProbabilities[ index ];

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getOrientationProbabilityWithSpecificNormalization( "
             "const gkg::Vector3d< float >& unitaryOrientation, "
             "float globalMinimumProbability, "
             "float globalMaximumProbability, "
             "std::vector< float >& orientationProbabilities, "
             "float& localMinimumProbability, "
             "float& localMaximumProbability ) const" );

}


float gkg::OrientationDistributionFunction::
                             getOrientationProbabilityWithSpecificNormalization(
                           const std::vector< float >& orientationProbabilities,
                           float localMinimumProbability,
                           float localMaximumProbability,
                           const gkg::Vector3d< float >& unitaryOrientation,
                           float globalMinimumProbability,
                           float globalMaximumProbability ) const
{

  try
  {

    int32_t index = _orientationSet->getNearestOrientationIndex(
                                                           unitaryOrientation );
    float unscaledOrientationProbability = orientationProbabilities[ index ];
    float rescaledOrientationProbability = 0.0f;
    if ( unscaledOrientationProbability < localMinimumProbability )
    {

      rescaledOrientationProbability = globalMinimumProbability;

    }
    else if ( unscaledOrientationProbability > localMaximumProbability )
    {

      rescaledOrientationProbability = globalMaximumProbability;

    }
    else
    {

      rescaledOrientationProbability = 
                  globalMinimumProbability +
                  ( unscaledOrientationProbability - localMinimumProbability ) * 
                  ( ( globalMaximumProbability - globalMinimumProbability ) /
                    ( localMaximumProbability - localMinimumProbability ) );

    }

    return rescaledOrientationProbability;

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction:: "
             "getOrientationProbabilityWithSpecificNormalization( "
             "const std::vector< float >& orientationProbabilities, "
             "float localMinimumProbability, "
             "float localMaximumProbability, "
             "const gkg::Vector3d< float >& unitaryOrientation, "
             "float globalMinimumProbability, "
             "float globalMaximumProbability ) const" );

}


float
gkg::OrientationDistributionFunction::getMinimumOrientationProbability() const
{

  try
  {

    float minimum = 0.0;
    gkg::MinimumFilter< std::vector< float >, float > minimumFilter;
    minimumFilter.filter( getOrientationProbabilities(), minimum );

    return minimum;

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getMinimumOrientationProbability() const" );

}


float
gkg::OrientationDistributionFunction::getMaximumOrientationProbability() const
{

  try
  {

    float maximum = 0.0;
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
    maximumFilter.filter( getOrientationProbabilities(), maximum );

    return maximum;

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getMaximumOrientationProbability() const" );

}


void
gkg::OrientationDistributionFunction::getMinMaxOrientationProbability(
                                                   float& min, float& max) const
{

  try
  {

    gkg::MinMaxFilter< std::vector< float >, float > minMaxFilter;
    std::pair< float, float > localMinMaxProbability;

    minMaxFilter.filter( getOrientationProbabilities(),
                         localMinMaxProbability );
    min = localMinMaxProbability.first;
    max = localMinMaxProbability.second;

  }
  GKG_CATCH( "void gkg::OrientationDistributionFunction::"
             "getMinMaxOrientationProbability( "
             "float& min, float& max) const" );

}


void
gkg::OrientationDistributionFunction::getMinMaxAndOrientationProbabilities(
                                            std::vector< float >& probabilities,
                                            float& min, float& max) const
{

  try
  {

    gkg::MinMaxFilter< std::vector< float >, float > minMaxFilter;
    std::pair< float, float > localMinMaxProbability;
    getOrientationProbabilities( probabilities );
    minMaxFilter.filter( probabilities,
                         localMinMaxProbability );
    min = localMinMaxProbability.first;
    max = localMinMaxProbability.second;

  }
  GKG_CATCH( "void "
             "gkg::OrientationDistributionFunction::"
             "getMinMaxAndOrientationProbabilities( "
             "std::vector< float >& probabilities, "
             "float& min, float& max) const" );

}


const gkg::Vector3d< float >&
gkg::OrientationDistributionFunction::getPrincipalOrientation() const
{

  try
  {

    return _orientationSet->getOrientation( getPrincipalOrientationIndex() );

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::OrientationDistributionFunction::getPrincipalOrientation() "
             "const" );

}


int32_t
gkg::OrientationDistributionFunction::getPrincipalOrientationIndex() const
{

  try
  {

    int32_t index = 0;
    gkg::MaximumIndexFilter< std::vector< float >, int32_t > maximumIndexFilter;
    maximumIndexFilter.filter( getOrientationProbabilities(), index );

    return index;

  }
  GKG_CATCH( "int32_t "
             "gkg::OrientationDistributionFunction::"
             "getPrincipalOrientationIndex() const" );

}


int32_t 
gkg::OrientationDistributionFunction::getPrincipalOrientationIndexFast() const
{

  try
  {

    float maxProba = -1e38;
    int32_t maxIndex = 0;

    if ( ( _basisType == gkg::OrientationDistributionFunction::Standard ) ||
         ( _basisType == 
                  gkg::OrientationDistributionFunction::UnnormalizedStandard ) ||
         ( _basisType == 
                  gkg::OrientationDistributionFunction::Shore ) )
    {

      int32_t i = 0;
      std::vector< float >::const_iterator
        v = this->_values.begin(),
        ve = this->_values.end();

      while ( v != ve )
      {

        if ( *v > maxProba )
        {

          maxProba = *v;
          maxIndex = i;

        }

        i++;
        ++v;

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      int32_t shCoefficientCount = ( int32_t )this->_values.size();
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o, s;

      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      for ( o = 0; o < orientationCount; o++ )
      {

        float p = 0.0f;
        float* val = ( float* )&this->_values[ 0 ];
        float* shCoef = ( float* )&shCoefficients[ o ][ 0 ];
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          p += *val++ * *shCoef++;

        }

        if ( p > maxProba )
        {

          maxProba = p;
          maxIndex = o;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      // in case of multi-tissue spherical harmonics, _values[] contains
      //  <tissueCount> <shCoefficientCount1 > <series of SH coefficients> ...
      //  ............. <shCoefficientCountN > <series of SH coefficients>
      // and it is assumed that the first tissue class is the one of interest
      // (eg white matter in our case)

      int32_t shCoefficientCount = ( int32_t )( this->_values[ 1 ] + 0.5 );
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o, s;

      gkg::SymmetricalSphericalHarmonicsCache::getInstance().update( 
                                                           *_orientationSet,
                                                           shCoefficientCount );
      const std::vector< std::vector< float > >& shCoefficients =
       gkg::SymmetricalSphericalHarmonicsCache::getInstance().getCoefficients();

      for ( o = 0; o < orientationCount; o++ )
      {

        float p = 0.0f;
        float* val = ( float* )&( this->_values[ 2 ] );
        float* shCoef = ( float* )&shCoefficients[ o ][ 0 ];
        for ( s = 0; s < shCoefficientCount; s++ )
        {

          p += *val++ * *shCoef++;

        }

        if ( p > maxProba )
        {

          maxProba = p;
          maxIndex = o;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiWatson )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 3 ],
                                                   this->_values[ 4 ],
                                                   this->_values[ 5 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 2 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o;
      float p = 0.0f;

      for ( o = 0; o < orientationCount; o++ )
      {

        p = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation( o ) );

        if ( p > maxProba )
        {

          maxProba = p;
          maxIndex = o;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::NoddiBingham )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 4 ],
                                                   this->_values[ 5 ],
                                                   this->_values[ 6 ] );

      // extracting secondary orientation
      gkg::Vector3d< float > secondaryOrientation( this->_values[ 7 ],
                                                   this->_values[ 8 ],
                                                   this->_values[ 9 ] );

      // extracting kappa1 and kappa2
      double kappa1 = ( double )this->_values[ 2 ];
      double kappa2 = ( double )this->_values[ 3 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o;
      float p = 0.0f;

      for ( o = 0; o < orientationCount; o++ )
      {

        p = ( float )factory->getBinghamDistribution(
                                         kappa1,
                                         kappa2,
                                         principalOrientation,
                                         secondaryOrientation,
                                         _orientationSet->getOrientation( o ) );

        if ( p > maxProba )
        {

          maxProba = p;
          maxIndex = o;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::PGSEAxonMapping )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o;
      float p = 0.0f;

      for ( o = 0; o < orientationCount; o++ )
      {

        p = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation( o ) );

        if ( p > maxProba )
        {

          maxProba = p;
          maxIndex = o;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
    {

      NumericalAnalysisImplementationFactory* 
        factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

      // extracting principal orientation
      gkg::Vector3d< float > principalOrientation( this->_values[ 8 ],
                                                   this->_values[ 9 ],
                                                   this->_values[ 10 ] );

      // extracting kappa
      double kappa = ( double )this->_values[ 5 ];

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o;
      float p = 0.0f;

      for ( o = 0; o < orientationCount; o++ )
      {

        p = ( float )factory->getWatsonDistribution(
                                         kappa,
                                         principalOrientation,
                                         _orientationSet->getOrientation( o ) );

        if ( p > maxProba )
        {

          maxProba = p;
          maxIndex = o;

        }

      }

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::Ivim )
    {

      gkg::Tensor tensor( this->_values[ 3 ],
                          this->_values[ 4 ],
                          this->_values[ 5 ],
                          this->_values[ 6 ],
                          this->_values[ 7 ],
                          this->_values[ 8 ] );

      // computing probabilities
      int32_t orientationCount = _orientationSet->getCount();
      int32_t o;
      float p = 0.0f;

      for ( o = 0; o < orientationCount; o++ )
      {

        p = ( float )tensor.getDisplacementProbability(
                                         _orientationSet->getOrientation( o ) );

        if ( p > maxProba )
        {

          maxProba = p;
          maxIndex = o;

        }

      }

    }

    return maxIndex;

  }
  GKG_CATCH( "int32_t gkg::OrientationDistributionFunction::"
             "getPrincipalOrientationIndexFast() const" );

}


int32_t 
gkg::OrientationDistributionFunction::getGibbsSampledOrientationIndexFast(
                                    const gkg::RandomGenerator& randomGenerator,
                                    float temperature ) const
{

  try
  {

    std::vector< float > 
      probabilities = this->getUnnormalizedOrientationProbabilities();

    gkg::MinMaxFilter< std::vector< float >, float >
      minMaxFilter;

    std::pair< float, float > minMaxValues( 0.0, 0.0 );
    minMaxFilter.filter( probabilities, minMaxValues );

    float probabilityRange = minMaxValues.second - minMaxValues.first;
    float temperatureTimesProbabilityRange = temperature * probabilityRange;

    // processing Gibb's likelyhoods
    std::list< float > likelyhoods;
    std::vector< float >::const_iterator
      p = probabilities.begin(),
      pe = probabilities.end();
    float likelyhood = 0.0;
    while ( p != pe )
    {

      if ( temperature != 1.0f )
      {

        likelyhood += ( float )std::exp( ( *p - minMaxValues.first ) /
                                         temperatureTimesProbabilityRange );

      }
      else
      {

        likelyhood += ( float )( *p - minMaxValues.first );

      }

      likelyhoods.push_back( likelyhood );
      ++ p;

    }

    // choosing randomly the orientation
    float maximumLikelyhood = likelyhoods.back();
    float randomLikelyhood = 
      ( float )gkg::NumericalAnalysisSelector::getInstance().
                             getImplementationFactory()->getUniformRandomNumber(
                                                 randomGenerator,
                                                 0.0,
                                                 ( double )maximumLikelyhood );


    std::list< float >::const_iterator
      l = likelyhoods.begin(),
      le = likelyhoods.end();
    int32_t randomOrientationIndex = 0;
    gkg::Vector3d< float > randomOrientation;
    while ( l != le )
    {

      if ( *l >= randomLikelyhood )
      {

        break;

      }

      ++ randomOrientationIndex;
      ++ l;

    }

    return randomOrientationIndex;

  }
  GKG_CATCH( "int32_t gkg::OrientationDistributionFunction::"
             "getGibbsSampledOrientationIndexFast( "
             "float temperature ) const" );

}



const gkg::Vector3d< float >& 
gkg::OrientationDistributionFunction::getGibbsSampledOrientation(
                                    const gkg::RandomGenerator& randomGenerator,
                                    float temperature ) const
{

  try
  {

    return _orientationSet->getOrientation(
                           getGibbsSampledOrientationIndexFast( randomGenerator,
                                                                temperature ) );

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::OrientationDistributionFunction::"
             "getGibbsSampledOrientation( "
             "float temperature ) const" );

}



void 
gkg::OrientationDistributionFunction::getGibbsSampledOrientationIndicesFast(
                              const gkg::RandomGenerator& randomGenerator,
                              std::vector< int32_t >& sampledOrientationIndices,
                              float temperature ) const
{

  try
  {

    if ( sampledOrientationIndices.empty() )
    {

      throw std::runtime_error(
                          "the sampledOrientationIndices vector size is nul!" );

    }

    std::vector< float > 
      probabilities = this->getUnnormalizedOrientationProbabilities();

    gkg::MinMaxFilter< std::vector< float >, float >
      minMaxFilter;

    std::pair< float, float > minMaxValues( 0.0, 0.0 );
    minMaxFilter.filter( probabilities, minMaxValues );

    float probabilityRange = minMaxValues.second - minMaxValues.first;
    float temperatureTimesProbabilityRange = temperature * probabilityRange;

    // processing Gibb's likelyhoods
    std::list< float > likelyhoods;
    std::vector< float >::const_iterator
      p = probabilities.begin(),
      pe = probabilities.end();
    float likelyhood = 0.0;
    while ( p != pe )
    {

      if ( temperature != 1.0f )
      {

        likelyhood += ( float )std::exp( ( *p - minMaxValues.first ) /
                                         temperatureTimesProbabilityRange );

      }
      else
      {

        likelyhood += ( float )( *p - minMaxValues.first );

      }

      likelyhoods.push_back( likelyhood );
      ++ p;

    }

    // choosing randomly the orientation
    float maximumLikelyhood = likelyhoods.back();
    float randomLikelyhood = 0.0f;

    std::vector< int32_t >::iterator
      i = sampledOrientationIndices.begin(),
      ie = sampledOrientationIndices.end();
    while ( i != ie )
    {

      randomLikelyhood = 
      ( float )gkg::NumericalAnalysisSelector::getInstance().
                             getImplementationFactory()->getUniformRandomNumber(
                                                 randomGenerator,
                                                 0.0,
                                                 ( double )maximumLikelyhood );


      std::list< float >::const_iterator
        l = likelyhoods.begin(),
        le = likelyhoods.end();
      int32_t randomOrientationIndex = 0;
      gkg::Vector3d< float > randomOrientation;
      while ( l != le )
      {

        if ( *l >= randomLikelyhood )
        {

          break;

        }

        ++ randomOrientationIndex;
        ++ l;

      }

      *i = randomOrientationIndex;

      ++ i;

    }

  }
  GKG_CATCH( "void " 
             "gkg::OrientationDistributionFunction::"
             "getGibbsSampledOrientationIndicesFast( "
             "const gkg::RandomGenerator& randomGenerator, "
             "std::vector< int32_t >& sampledOrientationIndices, "
             "float temperature ) const" );

}


void gkg::OrientationDistributionFunction::getGibbsSampledOrientations(
                     const gkg::RandomGenerator& randomGenerator,
                     std::vector< gkg::Vector3d< float > >& sampledOrientations,
                     float temperature ) const
{

  try
  {

    if ( sampledOrientations.empty() )
    {

      throw std::runtime_error( "the sampledOrientations vector size is nul!" );

    }
    int32_t sampleCount = ( int32_t )sampledOrientations.size();
    std::vector< int32_t > sampledOrientationIndices( sampleCount );

    this->getGibbsSampledOrientationIndicesFast( randomGenerator,
                                                 sampledOrientationIndices,
                                                 temperature );

    std::vector< gkg::Vector3d< float > >::iterator
      o = sampledOrientations.begin(),
      oe = sampledOrientations.end();
    std::vector< int32_t >::const_iterator
      i = sampledOrientationIndices.begin();
    while ( o != oe )
    {

      *o = _orientationSet->getOrientation( *i );

      ++ i;
      ++ o;

    }

  }
  GKG_CATCH( "void gkg::OrientationDistributionFunction::"
             "getGibbsSampledOrientations( "
             "const gkg::RandomGenerator& randomGenerator, "
             "std::vector< gkg::Vector3d< float > >& sampledOrientations, "
             "float temperature ) const" );

}




const gkg::Vector3d< float >&
gkg::OrientationDistributionFunction::getPrincipalOrientation( 
                           const gkg::Vector3d< float >& coneUnitaryOrientation,
                           float apertureAngle ) const
{

  try
  {

    int32_t coneUnitaryOrientationIndex =
      _orientationSet->getNearestOrientationIndex( coneUnitaryOrientation );

    return _orientationSet->getOrientation( getPrincipalOrientationIndex(
                                                    coneUnitaryOrientationIndex,
                                                    apertureAngle ) );

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::OrientationDistributionFunction::getPrincipalOrientation( "
             "const gkg::Vector3d< float >& coneUnitaryOrientation, "
             "float apertureAngle ) "
             "const" );

}


int32_t
gkg::OrientationDistributionFunction::getPrincipalOrientationIndex( 
                                            int32_t coneUnitaryOrientationIndex,
                                            float /* apertureAngle */ ) const
{

  try
  {

    // updating orientation set cache for speeding-up calculation
    //gkg::OrientationSetCache::getInstance().update( *_orientationSet,
    //                                                apertureAngle );
    if ( gkg::OrientationSetCache::getInstance().getOrientationSet() !=
         _orientationSet )
    {

      throw std::runtime_error( "Cache error : bad orientation sets." );

    }

    // collecting the orientation indices contained in the solid angle
    const std::vector< int32_t >& solidAngleOrientationIndices =
      gkg::OrientationSetCache::getInstance().getSolidAngleOrientationIndices(
                                                  coneUnitaryOrientationIndex );

    if ( solidAngleOrientationIndices.empty() )
    {

      throw std::runtime_error( "empty index vector" );

    }

    int32_t index = getUnnormalizedOrientationProbabilitiesMaxIndex( 
                                                  coneUnitaryOrientationIndex );

    return solidAngleOrientationIndices[ index ];

  }
  GKG_CATCH( "int32_t "
             "gkg::OrientationDistributionFunction::"
             "getPrincipalOrientationIndex( "
             "int32_t coneUnitaryOrientationIndex, "
             "float apertureAngle ) "
             "const" );

}


int32_t
gkg::OrientationDistributionFunction::getPrincipalOrientationIndexFast( 
                                     int32_t coneUnitaryOrientationIndex ) const
{

  try
  {

    // collecting the orientation indices contained in the solid angle
    const std::vector< int32_t >& solidAngleOrientationIndices =
      gkg::OrientationSetCache::getInstance().getSolidAngleOrientationIndices(
                                                  coneUnitaryOrientationIndex );

    if ( solidAngleOrientationIndices.empty() )
    {

      throw std::runtime_error( "empty index vector" );

    }

    int32_t index = getUnnormalizedOrientationProbabilitiesMaxIndexFast( 
                                                  coneUnitaryOrientationIndex );

    return solidAngleOrientationIndices[ index ];

  }
  GKG_CATCH( "int32_t "
             "gkg::OrientationDistributionFunction::"
             "getPrincipalOrientationIndexFast( "
             "int32_t coneUnitaryOrientationIndex ) "
             "const" );

}


float
gkg::OrientationDistributionFunction::getGeneralizedFractionalAnisotropy() const
{

  try
  {

    float generalizedFractionalAnisotropy = 0.0;
    if ( _basisType ==
         gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      std::vector< float >::const_iterator sh = this->_values.begin(),
                                           she = this->_values.end();

      float sh0Square = *sh * *sh;
      ++ sh;

      float sumOfSquare = sh0Square;

      while ( sh != she )
      {

        sumOfSquare += *sh * *sh;
        ++ sh;

      }

      if ( gkg::equal( sumOfSquare, 0.0f, 1e-10f ) )
      {

        generalizedFractionalAnisotropy = 0.0;

      }
      else
      {

        float ratioOfSquare = sh0Square / sumOfSquare;
	
        if ( ratioOfSquare > 1.0 )
        {

          generalizedFractionalAnisotropy = 0.0;

        }
        else
        {

          generalizedFractionalAnisotropy = std::sqrt( 1.0 - ratioOfSquare );

        }

      }
      //generalizedFractionalAnisotropy *= M_PI;

    }
    else if ( _basisType ==
              gkg::OrientationDistributionFunction::
                                                 MultiTissueSphericalHarmonics )
    {

      // in case of multi-tissue spherical harmonics, _values[] contains
      //  <tissueCount> <shCoefficientCount1 > <series of SH coefficients> ...
      //  ............. <shCoefficientCountN > <series of SH coefficients>
      // and it is assumed that the first tissue class is the one of interest
      // (eg white matter in our case)

      int32_t shCoefficientCount = ( int32_t )( this->_values[ 1 ] + 0.5 );
      const float* sh = &this->_values[ 2 ];
      const float* she = sh + shCoefficientCount;

      float sh0Square = *sh * *sh;
      ++ sh;

      float sumOfSquare = sh0Square;

      while ( sh != she )
      {

        sumOfSquare += *sh * *sh;
        ++ sh;

      }

      if ( gkg::equal( sumOfSquare, 0.0f, 1e-10f ) )
      {

        generalizedFractionalAnisotropy = 0.0;

      }
      else
      {

        float ratioOfSquare = sh0Square / sumOfSquare;
	
        if ( ratioOfSquare > 1.0 )
        {

          generalizedFractionalAnisotropy = 0.0;

        }
        else
        {

          generalizedFractionalAnisotropy = std::sqrt( 1.0 - ratioOfSquare );

        }

      }
      //generalizedFractionalAnisotropy *= M_PI;

    }
    else
    {

      std::vector< float > probabilities = getOrientationProbabilities();

      int32_t orientationCount = ( int32_t )probabilities.size();

      float stdDev = 0.0;
      float rms = 0.0;
      float mean = 0.0;
      float deviation = 0.0;

      std::vector< float >::const_iterator
        p = probabilities.begin(),
        pe = probabilities.end();
      while ( p != pe )
      {

        mean += *p;
        ++ p;

      }
      mean /= ( float )orientationCount;

      p = probabilities.begin();
      while ( p != pe )
      {

        deviation = *p - mean;
        stdDev += deviation * deviation;
        rms += *p * *p;

        ++ p;

      }


      if ( gkg::equal( rms, 0.0f, 1e-10f ) )
      {

        generalizedFractionalAnisotropy = 0.0;

      }
      else
      {

        generalizedFractionalAnisotropy =
                       std::sqrt( ( ( float )orientationCount * stdDev ) /
                                  ( ( float )( orientationCount - 1 ) * rms ) );

      }

    }

    return generalizedFractionalAnisotropy;

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getGeneralizedFractionalAnisotropy() const" );

}


float gkg::OrientationDistributionFunction::getStandardDeviation() const
{

  try
  {

    std::vector< float > probabilities = getOrientationProbabilities();

    int32_t orientationCount = ( int32_t )probabilities.size();

    float mean = 1.0 / ( float )orientationCount;
    float stdDev = 0.0;
    float deviation = 0.0;

    std::vector< float >::const_iterator
      p = probabilities.begin(),
      pe = probabilities.end();
    while ( p != pe )
    {

      deviation = *p - mean;
      stdDev += deviation * deviation;

      ++ p;

    }

    return std::sqrt( stdDev / ( float )( orientationCount - 1 ) );

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getStandardDeviation() const" );

}


float gkg::OrientationDistributionFunction::getNormalizedEntropy() const
{

  try
  {

    std::vector< float > probabilities = getOrientationProbabilities();

    int32_t orientationCount = ( int32_t )probabilities.size();

    float meanLog = 0;

    std::vector< float >::const_iterator
      p = probabilities.begin(),
      pe = probabilities.end();
    while ( p != pe )
    {

      if ( *p > 0.0 )
      {

        meanLog += std::log( *p );

      }

      ++ p;

    }

    return - ( ( float )orientationCount /
               ( float )std::log( ( double )orientationCount ) ) * meanLog;

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getNormalizedEntropy() const" );

}


float gkg::OrientationDistributionFunction::getNematicOrder() const
{

  try
  {

    std::vector< float > probabilities = getOrientationProbabilities();

    int32_t orientationCount = ( int32_t )probabilities.size();
    int32_t o = 0;

    // processing mean orientation
    gkg::Vector3d< float > meanOrientation( 0.0, 0.0, 0.0 );
    std::vector< float >::const_iterator
      p = probabilities.begin(),
      pe = probabilities.end();
    while ( p != pe )
    {

      meanOrientation += _orientationSet->getOrientation( o ) * *p;

      ++ p;
      ++ o;

    }

    // processing nematic order
    float nematicOrder = 0.0;
    float cosinus = 0.0;
    for ( o = 0; o < orientationCount; o++ )
    {

      cosinus = std::cos( _orientationSet->getOrientation( o ).dot(
                                                   meanOrientation ) );
      nematicOrder += 3.0 * cosinus * cosinus - 1.0;

    }

    nematicOrder /= 2.0;

    return nematicOrder;

  }
  GKG_CATCH( "float gkg::OrientationDistributionFunction::"
             "getNematicOrder() const" );

}


