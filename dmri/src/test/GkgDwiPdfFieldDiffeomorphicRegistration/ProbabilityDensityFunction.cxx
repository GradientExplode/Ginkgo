#include "ProbabilityDensityFunction.h"
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-processing-coordinates/DisplacementSetCache.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/GeodesicSO3Interpolator.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-transform/Rotation3d.h>
//#include <gkg-dmri-ph-basis/PolarHarmonicsCache.h>
#include <gkg-dmri-pdf/PdfMoment.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <cfloat>

#include <numeric>

#include "PolarHarmonicsCache.h"


gkg::ProbabilityDensityFunction::ProbabilityDensityFunction(
                          gkg::ProbabilityDensityFunction::BasisType basisType )
                                : gkg::DiffusionFeature(),
                                  _basisType( basisType ),
                                  _outputMultipleShellSampling( 0 ),
                                  _outputCartesianGridSize( 0 ),
                                  _outputCartesianGridResolution( 0 )
{
}


gkg::ProbabilityDensityFunction::ProbabilityDensityFunction(
     gkg::ProbabilityDensityFunction::BasisType basisType,
     const std::map< float, gkg::OrientationSet >* outputMultipleShellSampling )
                                : gkg::DiffusionFeature(),
                                  _basisType( basisType ),
                                  _outputMultipleShellSampling(
                                                  outputMultipleShellSampling ),
                                  _outputCartesianGridSize( 0 ),
                                  _outputCartesianGridResolution( 0 )
{

  try
  {

    if ( _outputMultipleShellSampling )
    {

      if ( _basisType == gkg::ProbabilityDensityFunction::Standard )
      {

        int32_t sampleCount = 0;
        std::map< float, gkg::OrientationSet >::const_iterator
          s = _outputMultipleShellSampling->begin(),
          se = _outputMultipleShellSampling->end();
        while ( s != se )
        {

          sampleCount += s->second.getCount();
          ++ s;

        }
        this->_values.resize( sampleCount );

      }

    }

  }
  GKG_CATCH( "gkg::ProbabilityDensityFunction::ProbabilityDensityFunction( "
             "gkg::ProbabilityDensityFunction::BasisType basisType, "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling )" );

}


gkg::ProbabilityDensityFunction::ProbabilityDensityFunction(
                  gkg::ProbabilityDensityFunction::BasisType basisType,
                  const gkg::Vector3d< int32_t >* outputCartesianGridSize,
                  const gkg::Vector3d< double >* outputCartesianGridResolution )
                                : gkg::DiffusionFeature(),
                                  _basisType( basisType ),
                                  _outputMultipleShellSampling( 0 ),
                                  _outputCartesianGridSize( 
                                                      outputCartesianGridSize ),
                                  _outputCartesianGridResolution( 
                                                 outputCartesianGridResolution )
{

  try
  {

    if ( _outputCartesianGridSize && _outputCartesianGridResolution )
    {

      if ( _basisType == gkg::ProbabilityDensityFunction::Standard )
      {

        int32_t sampleCount = outputCartesianGridSize->x * 
                              outputCartesianGridSize->y * 
                              outputCartesianGridSize->z;
        this->_values.resize( sampleCount );

      }

    }

  }
  GKG_CATCH( "gkg::ProbabilityDensityFunction::ProbabilityDensityFunction( "
             "gkg::ProbabilityDensityFunction::BasisType basisType, "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution )" );

}


gkg::ProbabilityDensityFunction::ProbabilityDensityFunction(
                                  const gkg::ProbabilityDensityFunction& other )
                               : gkg::DiffusionFeature( other ),
                                  _basisType( other._basisType ),
                                  _outputMultipleShellSampling( 
                                           other._outputMultipleShellSampling ),
                                  _outputCartesianGridSize( 
                                              other._outputCartesianGridSize ),
                                  _outputCartesianGridResolution( 
                                         other._outputCartesianGridResolution )
{
}


gkg::ProbabilityDensityFunction::~ProbabilityDensityFunction()
{
}


gkg::ProbabilityDensityFunction&
gkg::ProbabilityDensityFunction::operator=(
                                  const gkg::ProbabilityDensityFunction& other )
{

  try
  {

    this->gkg::DiffusionFeature::operator=( other );
    _basisType = other._basisType;
    _outputMultipleShellSampling = other._outputMultipleShellSampling;
    _outputCartesianGridSize = other._outputCartesianGridSize;
    _outputCartesianGridResolution = other._outputCartesianGridResolution;
  
    return *this;

  }
  GKG_CATCH( "gkg::ProbabilityDensityFunction& "
             "gkg::ProbabilityDensityFunction::operator=( "
             "const gkg::ProbabilityDensityFunction& other )" );

}


gkg::ProbabilityDensityFunction::BasisType 
gkg::ProbabilityDensityFunction::getBasisType() const
{

  try
  {

    return _basisType;

  }
  GKG_CATCH( "gkg::ProbabilityDensityFunction::BasisType "
             "gkg::ProbabilityDensityFunction::getBasisType() const" );

}


std::string 
gkg::ProbabilityDensityFunction::getBasisTypeName() const
{

  try
  {

    std::string basisTypeName = "";

    if ( _basisType == gkg::ProbabilityDensityFunction::Standard )
    {

      basisTypeName = "std";

    }
    else if ( _basisType == gkg::ProbabilityDensityFunction::SecondOrderTensor )
    {

      basisTypeName = "te";

    }
    else if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      basisTypeName = "ph";

    }

    return basisTypeName;

  }
  GKG_CATCH( "std::string "
             "gkg::ProbabilityDensityFunction::getBasisTypeName() const" );

}


void gkg::ProbabilityDensityFunction::setBasisType(
                          gkg::ProbabilityDensityFunction::BasisType basisType )
{

  try
  {

    _basisType = basisType;

  }
  GKG_CATCH( "void gkg::ProbabilityDensityFunction::setBasisType( "
             "gkg::ProbabilityDensityFunction::BasisType basisType )" );

}


void gkg::ProbabilityDensityFunction::setBasisType(
                                              const std::string& basisTypeName )
{

  try
  {

    if ( basisTypeName == "std" )
    {

      _basisType = gkg::ProbabilityDensityFunction::Standard;

    }
    else if ( basisTypeName == "te" )
    {

      _basisType = gkg::ProbabilityDensityFunction::SecondOrderTensor;

    }
    else if ( basisTypeName == "ph" )
    {

      _basisType = gkg::ProbabilityDensityFunction::PolarHarmonics;

    }

  }
  GKG_CATCH( "void gkg::ProbabilityDensityFunction::setBasisType( "
             "const std::string& basisTypeName )" );

}


void gkg::ProbabilityDensityFunction::setStandardBasisType()
{

  try
  {

    if ( _basisType == gkg::ProbabilityDensityFunction::Standard )
    {

      // nothing special to do

    }
    else if ( _basisType ==
              gkg::ProbabilityDensityFunction::SecondOrderTensor )
    {

      _values = getDisplacementProbabilities();

    }
    else if ( _basisType ==
              gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      _values = getDisplacementProbabilities();

    }
    _basisType = gkg::ProbabilityDensityFunction::Standard;

  }
  GKG_CATCH( "void gkg::ProbabilityDensityFunction:setStandardBasisType()" );

}


void gkg::ProbabilityDensityFunction::updatePolarHarmonicsCache() const
{

  try
  {

    // using a cache system for speeding polar harmonics calculation when
    // used in field(s)
    if ( _basisType ==
         gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      int32_t basisOrder = ( int32_t )_values[ 0 ];
      gkg::PolarHarmonicsCache::getInstance().update( basisOrder );

    }

  }
  GKG_CATCH( "void gkg::ProbabilityDensityFunction::updatePolarHarmonicsCache( "
             ") const" );

}


void gkg::ProbabilityDensityFunction::updateDisplacementCache(
  const std::map< float, gkg::OrientationSet >&
                                             outputMultipleShellSampling ) const
{

  try
  {

    // updating orientation set cache for speeding-up calculation
    gkg::DisplacementSetCache::getInstance().update( 
                                                  outputMultipleShellSampling );

  }
  GKG_CATCH( "void gkg::ProbabilityDensityFunction::updateDisplacementCache( "
             "const std::map< float, gkg::OrientationSet >& "
             "outputMultipleShellSampling ) const" );

}


void gkg::ProbabilityDensityFunction::updateDisplacementCache(
            const gkg::Vector3d< int32_t >& outputCartesianGridSize,
            const gkg::Vector3d< double >& outputCartesianGridResolution ) const
{

  try
  {

    // updating orientation set cache for speeding-up calculation
    gkg::DisplacementSetCache::getInstance().update(
                                                outputCartesianGridSize,
                                                outputCartesianGridResolution );

  }
  GKG_CATCH( "void gkg::ProbabilityDensityFunction::updateDisplacementCache( "
             "const gkg::Vector3d< int32_t >& outputCartesianGridSize, "
             "const gkg::Vector3d< double >& outputCartesianGridResolution "
             ") const" );

}


bool gkg::ProbabilityDensityFunction::checkAndModifyOutputSampling(
           std::map< float, gkg::OrientationSet >* outputMultipleShellSampling )
{

  try
  { 

    bool isModified = false;

    if ( _outputMultipleShellSampling &&
         _basisType == gkg::ProbabilityDensityFunction::Standard )
    {

      // first detecting the boundaries of the displacements
      float originalLowerDisplacementMagnitude = 
        _outputMultipleShellSampling->begin()->first;
      float originalUpperDisplacementMagnitude = 
        _outputMultipleShellSampling->rbegin()->first;

      // first detecting the boundaries of the displacements
      float newLowerDisplacementMagnitude = 
        outputMultipleShellSampling->begin()->first;
      float newUpperDisplacementMagnitude = 
        outputMultipleShellSampling->rbegin()->first;

      // in case the target output sampling goes beyong the boundaries of the
      // actually computed PDF values, we need to restrict it to the former
      // boundaries
      if ( ( newLowerDisplacementMagnitude <
             originalLowerDisplacementMagnitude ) ||
           ( newUpperDisplacementMagnitude >
             originalUpperDisplacementMagnitude ) )
      {

        isModified = true;
        std::list< std::map< float, gkg::OrientationSet >::iterator >
          entriesToBeDeleted;

        std::map< float, OrientationSet >::iterator
          d = outputMultipleShellSampling->begin(),
          de = outputMultipleShellSampling->end();
        while ( d != de )
        {

          if ( ( d->first < originalLowerDisplacementMagnitude ) ||
               ( d->first > originalUpperDisplacementMagnitude ) )
          {

            entriesToBeDeleted.push_back( d );

          }
          ++ d;

        }

        std::list< std::map< float, gkg::OrientationSet >::iterator
                  >::const_iterator
          e = entriesToBeDeleted.begin(),
          ee = entriesToBeDeleted.end();
        while ( e != ee )
        {

          outputMultipleShellSampling->erase( *e );
          ++ e;

        }

        if ( ( newLowerDisplacementMagnitude <
               originalLowerDisplacementMagnitude ) &&
             ( outputMultipleShellSampling->find(
                                       originalLowerDisplacementMagnitude ) ==
               outputMultipleShellSampling->end() ) )
        {

          std::map< float, gkg::OrientationSet >::const_iterator
            originalLowerShellIterator = _outputMultipleShellSampling->find(
                                         originalLowerDisplacementMagnitude );

          ( *outputMultipleShellSampling )[
                                        originalLowerDisplacementMagnitude ] =
             originalLowerShellIterator->second;

        }
        if ( ( newUpperDisplacementMagnitude >
               originalUpperDisplacementMagnitude ) &&
             ( outputMultipleShellSampling->find(
                                       originalUpperDisplacementMagnitude ) ==
               outputMultipleShellSampling->end() ) )
        {

          std::map< float, gkg::OrientationSet >::const_iterator
            originalUpperShellIterator = _outputMultipleShellSampling->find(
                                         originalLowerDisplacementMagnitude );

          ( *outputMultipleShellSampling )[
                                        originalUpperDisplacementMagnitude ] =
             originalUpperShellIterator->second;

        }

      }

    }

    return isModified;

  }
  GKG_CATCH( "bool gkg::ProbabilityDensityFunction::"
             "checkAndModifyOutputSampling( "
             "std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling )" );

}


void gkg::ProbabilityDensityFunction::setOutputSampling(
     std::map< float, gkg::OrientationSet >* outputMultipleShellSampling,
     int32_t resamplingNeighborCount,
     float resamplingGaussianSigma )
{

  try
  {

    if ( outputMultipleShellSampling == 0 )
    {

      throw std::runtime_error( "nul output multiple-shell sampling" );

    }

    // setting the output multiple shell sampling for the first time
    if ( _outputMultipleShellSampling == 0 )
    {

      _outputMultipleShellSampling = outputMultipleShellSampling;

    }
    // modifying the output multiple shell sampling and possibly need to
    // resample values in case of standard basis
    else
    {

      if ( _basisType == gkg::ProbabilityDensityFunction::Standard )
      { 

        // reallocating target values

        std::map< float, gkg::OrientationSet >::const_iterator
          newD = outputMultipleShellSampling->begin(),
          newDe = outputMultipleShellSampling->end();
        int32_t targetValueCount = 0;
        while ( newD != newDe )
        {

          targetValueCount += newD->second.getCount();
          ++ newD;

        }
        std::vector< float > allNewValues( targetValueCount );

        // now really resampling value(s)
        gkg::GeodesicSO3Interpolator
          geodesicSO3Interpolator( resamplingNeighborCount,
                                   resamplingGaussianSigma );

        newD = outputMultipleShellSampling->begin();
        newDe = outputMultipleShellSampling->end();
        int32_t targetValueIndex = 0;
        while ( newD != newDe )
        {

          std::map< float, gkg::OrientationSet >::const_iterator
            firstOldD;
          int32_t firstValueIndex = 0;
          std::map< float, gkg::OrientationSet >::const_iterator
            secondOldD;
          int32_t secondValueIndex = 0;
          int32_t accumulatedValueIndex = 0;
          bool hasOnlyOneD = false;

          std::map< float, gkg::OrientationSet >::const_iterator
            oldD = _outputMultipleShellSampling->begin(),
            oldDe = _outputMultipleShellSampling->end();

          while ( oldD != oldDe )
          {

            // in case the same displacement is present in the older output
            // sampling
            if ( newD->first == oldD->first )
            {

              firstOldD = oldD;
              firstValueIndex = accumulatedValueIndex;
              hasOnlyOneD = true;
              break;

            }
            // in case the displacement is bounded by two original displacements
            else if ( newD->first < oldD->first )
            {

              firstOldD = oldD;
              -- firstOldD;
              firstValueIndex = accumulatedValueIndex - oldD->second.getCount();
              secondOldD = oldD;
              secondValueIndex = accumulatedValueIndex;
              hasOnlyOneD = false;
              break;

            }
            accumulatedValueIndex += oldD->second.getCount();
            ++ oldD;

          }

          // resampling in case the same displacement is present in the older
          // output sampling
          if ( hasOnlyOneD )
          {

            const gkg::OrientationSet& targetOrientationSet = newD->second;
            int32_t targetOrientationCount = targetOrientationSet.getCount();
            const gkg::OrientationSet& originalOrientationSet =
                                                              firstOldD->second;

            std::vector< float > newValues( targetOrientationCount );

            int32_t o = 0;
            float value = 0.0f;
            const gkg::Vector3d< float >* optr = 
                                   &targetOrientationSet.getOrientations()[ 0 ];
            float* valuePtr = &this->_values[ firstValueIndex ];
            for ( o = 0; o < targetOrientationCount; o++ )
            {

              std::vector< std::pair< int32_t, float > >
                neighborIndicesAndWeights = geodesicSO3Interpolator.
                                                   getNeighborIndicesAndWeights(
                                     originalOrientationSet,
                                     *optr++ );

              value = 0.0f;
              std::vector< std::pair< int32_t, float > >::const_iterator
                n = neighborIndicesAndWeights.begin(),
                ne = neighborIndicesAndWeights.end();
              while ( n != ne )
              {

                value += n->second * valuePtr[ n->first ];
                ++ n;

              }
              newValues[ o ] = value;

            }

            float* newValuePtr = &allNewValues[ targetValueIndex ];
            for ( o = 0; o < targetOrientationCount; o++ )
            {

              *newValuePtr++ = newValues[ o ];

            }

          }
          // resampling in case the displacement is bounded by two original 
          // displacements
          else
          {

            const gkg::OrientationSet& targetOrientationSet = newD->second;
            int32_t targetOrientationCount = targetOrientationSet.getCount();

            const gkg::OrientationSet& originalFirstOrientationSet =
                                                              firstOldD->second;
            const gkg::OrientationSet& originalSecondOrientationSet =
                                                             secondOldD->second;

            // resampling on the first original shell
            std::vector< float > newFirstValues( targetOrientationCount );

            int32_t o = 0;
            float value = 0.0f;
            const gkg::Vector3d< float >* optr = 
                                   &targetOrientationSet.getOrientations()[ 0 ];
            float* valuePtr = &this->_values[ firstValueIndex ];
            for ( o = 0; o < targetOrientationCount; o++ )
            {

              std::vector< std::pair< int32_t, float > >
                neighborIndicesAndWeights = geodesicSO3Interpolator.
                                                   getNeighborIndicesAndWeights(
                                     originalFirstOrientationSet,
                                     *optr++ );

              value = 0.0f;
              std::vector< std::pair< int32_t, float > >::const_iterator
                n = neighborIndicesAndWeights.begin(),
                ne = neighborIndicesAndWeights.end();
              while ( n != ne )
              {

                value += n->second * valuePtr[ n->first ];
                ++ n;

              }
              newFirstValues[ o ] = value;

            }

            // resampling on the first original shell
            std::vector< float > newSecondValues( targetOrientationCount );
            for ( o = 0; o < targetOrientationCount; o++ )
            {

              std::vector< std::pair< int32_t, float > >
                neighborIndicesAndWeights = geodesicSO3Interpolator.
                                                   getNeighborIndicesAndWeights(
                                     originalSecondOrientationSet,
                                     targetOrientationSet.getOrientation( o ) );

              value = 0.0f;
              std::vector< std::pair< int32_t, float > >::const_iterator
                n = neighborIndicesAndWeights.begin(),
                ne = neighborIndicesAndWeights.end();
              valuePtr = &this->_values[ secondValueIndex ];
              while ( n != ne )
              {

                value += n->second * valuePtr[ n->first ];
                ++ n;

              }
              newSecondValues[ o ] = value;

            }

            // new radial values correspond to barycentric values between the 
            // two shells
            float denom = secondOldD->first - firstOldD->first;
            float alpha = ( secondOldD->first - newD->first ) / denom;
            float beta = ( newD->first - firstOldD->first ) / denom;
            float* newValuePtr = &allNewValues[ targetValueIndex ];
            for ( o = 0; o < targetOrientationCount; o++ )
            {

              *newValuePtr++ =
                      alpha * newFirstValues[ o ] + beta * newSecondValues[ o ];

            }


          }

          targetValueIndex += newD->second.getCount();
          ++ newD;

        }

        this->_values = allNewValues;
        _outputMultipleShellSampling = outputMultipleShellSampling;

      }

    }

  }
  GKG_CATCH( "void gkg::ProbabilityDensityFunction::setOutputSampling( "
             "std::map< float, gkg::OrientationSet >* "
             "outputMultipleShellSampling, "
             "int32_t resamplingNeighborCount, "
             "float resamplingGaussianSigma )" );

}


void gkg::ProbabilityDensityFunction::setOutputSampling(
                  const gkg::Vector3d< int32_t >* outputCartesianGridSize,
                  const gkg::Vector3d< double >* outputCartesianGridResolution )
{

  try
  {

    if ( outputCartesianGridSize == 0 )
    {

      throw std::runtime_error( "nul output Cartesian grid size" );

    }
    if ( outputCartesianGridResolution == 0 )
    {

      throw std::runtime_error( "nul output Cartesian grid resolution" );

    }
    _outputCartesianGridSize = outputCartesianGridSize;
    _outputCartesianGridResolution = outputCartesianGridResolution;

  }
  GKG_CATCH( "void gkg::ProbabilityDensityFunction::setOutputSampling( "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution )" );

}


void gkg::ProbabilityDensityFunction::finiteStrainReorientation( 
                                    gkg::Matrix& jacobianMatrix,
                                    gkg::ProbabilityDensityFunction& otherPdf )
{

  try
  {

    // getting access to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // extracting jacobian matrix eigenvalues for scaling
    gkg::Vector jacobianEigenvalues( 3 );
    gkg::Matrix jacobianEigenMatrix( 3, 3 );
    factory->getEigenSystem( jacobianMatrix,
                             jacobianEigenvalues,
                             jacobianEigenMatrix );    
    /*
    std::cout << "this->_values[ 4 ] =" << this->_values[ 4 ] << std::endl;
    std::cout << "this->_values[ 5 ] =" << this->_values[ 5 ] << std::endl;
    std::cout << "this->_values[ 6 ] =" << this->_values[ 6 ] << std::endl;
    std::cout << "jacobianEigenvalues =\n" << jacobianEigenvalues << std::endl;
    std::cout << std::endl;
    */

    gkg::Matrix inverseJacobianNorm( 3, 3 );
    inverseJacobianNorm = jacobianMatrix.getComposition( 
                                           jacobianMatrix.getTransposition() );
    factory->getPower( inverseJacobianNorm, -0.5, inverseJacobianNorm );

    gkg::Matrix finiteStrainMatrix( 3, 3 );
    finiteStrainMatrix = inverseJacobianNorm.getComposition( jacobianMatrix );
    factory->getMoorePenrosePseudoInverse( finiteStrainMatrix,
                                           finiteStrainMatrix );

    if ( _basisType == gkg::ProbabilityDensityFunction::Standard )
    {

      // copy current values of this
      std::vector< float > formerValues = this->_values;
      std::vector< float > currentValues(this->_values.size(), 0.0f );

      // copying the current outputMultipleShellSampling
      std::map< float, gkg::OrientationSet > outputMultipleShellSampling = 
                                       *( this->_outputMultipleShellSampling );

      // creating a reoriented outputMultipleShellSampling
      std::map< float, gkg::OrientationSet >::iterator
        oms = outputMultipleShellSampling.begin(),
        omse = outputMultipleShellSampling.end();

      gkg::Vector3d< float > formerDisplacement( 0.0f, 0.0f, 0.0f );
      gkg::Vector3d< float > displacement( 0.0f, 0.0f, 0.0f );
      float currentValue = 0.0f;
    
      while ( oms != omse )
      {

        float displacementAmplitude = oms->first;

        gkg::OrientationSet& orientationSet = oms->second;
      
        gkg::OrientationSet::iterator
          os = orientationSet.begin(),
          ose = orientationSet.end();
        while ( os != ose )
        { 
        
          gkg::Vector3d< float >& orientation = ( *os );


          formerDisplacement.x = ( float )( orientation.x * 
                                                       displacementAmplitude );
          formerDisplacement.y = ( float )( orientation.y * 
                                                       displacementAmplitude );
          formerDisplacement.z = ( float )( orientation.z * 
                                                       displacementAmplitude );

          currentValue = this->getDisplacementProbability( displacement );
       

          displacement.x = finiteStrainMatrix( 0, 0 ) * formerDisplacement.x +
                           finiteStrainMatrix( 0, 1 ) * formerDisplacement.y +
                           finiteStrainMatrix( 0, 2 ) * formerDisplacement.z; 
          displacement.y = finiteStrainMatrix( 1, 0 ) * formerDisplacement.x +
                           finiteStrainMatrix( 1, 1 ) * formerDisplacement.y +
                           finiteStrainMatrix( 1, 2 ) * formerDisplacement.z; 
          displacement.z = finiteStrainMatrix( 2, 0 ) * formerDisplacement.x +
                           finiteStrainMatrix( 2, 1 ) * formerDisplacement.y +
                           finiteStrainMatrix( 2, 2 ) * formerDisplacement.z; 

        /*
        std::cout << "formerDisplacement = " << formerDisplacement << std::endl;
        std::cout << "formerDisplacement index = " 
                  << gkg::DisplacementSetCache::getInstance().getInstance().
                 getNearestDisplacementIndex( formerDisplacement ) << std::endl;
        std::cout << "displacement = " << displacement << std::endl;
        std::cout << "displacement index = " 
                  << gkg::DisplacementSetCache::getInstance().getInstance().
                       getNearestDisplacementIndex( displacement ) << std::endl;
        std::cout << std::endl;
        */

        // reorientation method :
  
        // 1) for each vector of the oms, apply the jacobian matrix 
        //    transformation and find the other vector(s) in the oms which 
        //    is(are) the closest to this reoriented vector 
        // 2) assign the value of the reoriented vector to this other vector(s)

        // (!) at least 300 orientations per shell to have a good approximation

          currentValues[ gkg::DisplacementSetCache::getInstance().getInstance().
                             getNearestDisplacementIndex( displacement ) ] +=
                                                                   currentValue;

          ++ os;

        }

        ++ oms;

      }

      //this->setValues( currentValues );  

      // checking that reorientation does not lead an increase of dissimilarity
      float formerDissimilarity = this->getDissimilarity( otherPdf);

      this->setValues( currentValues );  

      float newDissimilarity = this->getDissimilarity( otherPdf);

      if ( newDissimilarity > formerDissimilarity )
      {
    
        this->setValues( formerValues ); 

      }   

    }

    else if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      // ROTATION

      //std::cout << "this->_values[ 1 ] before =" 
      //          << this->_values[ 1 ] << std::endl;

      this->_values[ 1 ] += std::atan2( ( float )finiteStrainMatrix( 2,1 ),
                                        ( float )finiteStrainMatrix( 2,2 ) );
      //std::cout << "this->_values[ 1 ] after =" 
      //          << this->_values[ 1 ] << std::endl;


      //std::cout << "this->_values[ 2 ] before =" 
      //          << this->_values[ 2 ] << std::endl;

      this->_values[ 2 ] += std::atan2( 
                            -( float )finiteStrainMatrix( 2,0 ),
                            std::sqrt( ( float )( 
                                std::pow( finiteStrainMatrix( 2,1 ), 2 ) + 
                                std::pow( finiteStrainMatrix( 2,2 ), 2 ) ) ) );
      //std::cout << "this->_values[ 2 ] after =" 
      //          << this->_values[ 2 ] << std::endl;

      //std::cout << "this->_values[ 3 ] before =" 
      //          << this->_values[ 3 ] << std::endl;

      this->_values[ 3 ] += std::atan2( ( float )finiteStrainMatrix( 1,0 ),
                                        ( float )finiteStrainMatrix( 0,0 ) );
      //std::cout << "this->_values[ 3 ] after =" 
      //          << this->_values[ 3 ] << std::endl;

      // SCALING
      /*
      double scaling1 = std::sqrt( std::abs( jacobianEigenvalues( 0 ) ) );
      double scaling2 = std::sqrt( std::abs( jacobianEigenvalues( 1 ) ) );
      double scaling3 = std::sqrt( std::abs( jacobianEigenvalues( 2 ) ) );

      double maxScaling = std::max( scaling1, std::max( scaling2, scaling3 ) );

      if ( maxScaling == scaling1 )
      {

        this->_values[ 4 ] *= scaling1;

      }
      else if ( maxScaling == scaling2 )
      {

        this->_values[ 5 ] *= scaling2;

      }
      else
      {

        this->_values[ 6 ] *= scaling3;

      }
    
      */

    }

  }
  GKG_CATCH( "void gkg::ProbabilityDensityFunction::"
             "finiteStrainReorientation(" 
             "gkg::Matrix& jacobianMatrix )" );

}



void gkg::ProbabilityDensityFunction::affineReorientation( 
                                    gkg::Matrix& jacobianMatrix,
                                    gkg::ProbabilityDensityFunction& otherPdf )
{

  try
  {

    if ( _basisType == gkg::ProbabilityDensityFunction::Standard )
    {

      // reorientation method :
  
      // 1) for each vector of the oms, apply the jacobian matrix 
      //    transformation and find the other vector(s) in the oms which 
      //    is(are) the closest to this reoriented vector 
      // 2) assign the value of the reoriented vector to this other vector(s)

      // getting access to the numerical analysis factory
      gkg::NumericalAnalysisImplementationFactory* factory =
       gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

      gkg::Matrix affineReorientationMatrix( 3, 3 );    
        factory->getMoorePenrosePseudoInverse( jacobianMatrix,
                                           affineReorientationMatrix );

      // copy current values of this
      std::vector< float > formerValues = this->_values;
      std::vector< float > currentValues(this->_values.size(), 0.0f );

      // copying the current outputMultipleShellSampling
      std::map< float, gkg::OrientationSet > outputMultipleShellSampling = 
                                       *( this->_outputMultipleShellSampling );

      // creating a reoriented outputMultipleShellSampling
      std::map< float, gkg::OrientationSet >::iterator
        oms = outputMultipleShellSampling.begin(),
        omse = outputMultipleShellSampling.end();

      gkg::Vector3d< float > formerDisplacement( 0.0f, 0.0f, 0.0f );
      gkg::Vector3d< float > displacement( 0.0f, 0.0f, 0.0f );
      float currentValue = 0.0f;
    
      while ( oms != omse )
      {

        float displacementAmplitude = oms->first;

        gkg::OrientationSet& orientationSet = oms->second;
      
        gkg::OrientationSet::iterator
          os = orientationSet.begin(),
          ose = orientationSet.end();
        while ( os != ose )
        { 
        
          gkg::Vector3d< float >& orientation = ( *os );


          formerDisplacement.x = ( float )( orientation.x * 
                                                       displacementAmplitude );
          formerDisplacement.y = ( float )( orientation.y * 
                                                       displacementAmplitude );
          formerDisplacement.z = ( float )( orientation.z * 
                                                       displacementAmplitude );

          currentValue = this->getDisplacementProbability( displacement );
       

          displacement.x = affineReorientationMatrix( 0, 0 ) * 
                                                         formerDisplacement.x +
                         affineReorientationMatrix( 0, 1 ) * 
                                                         formerDisplacement.y +
                         affineReorientationMatrix( 0, 2 ) * 
                                                         formerDisplacement.z; 
          displacement.y = affineReorientationMatrix( 1, 0 ) * 
                                                         formerDisplacement.x +
                         affineReorientationMatrix( 1, 1 ) * 
                                                         formerDisplacement.y +
                         affineReorientationMatrix( 1, 2 ) * 
                                                         formerDisplacement.z; 
          displacement.z = affineReorientationMatrix( 2, 0 ) * 
                                                         formerDisplacement.x +
                         affineReorientationMatrix( 2, 1 ) * 
                                                         formerDisplacement.y +
                         affineReorientationMatrix( 2, 2 ) * 
                                                         formerDisplacement.z; 




          currentValues[ gkg::DisplacementSetCache::getInstance().getInstance().
                             getNearestDisplacementIndex( displacement ) ] +=
                                                                   currentValue;

          ++ os;

        }

        ++ oms;

      }

      //this->setValues( currentValues );  

      // checking that reorientation does not lead an increase of dissimilarity
      float formerDissimilarity = this->getDissimilarity( otherPdf );

      this->setValues( currentValues );  

      float newDissimilarity = this->getDissimilarity( otherPdf );

      if ( newDissimilarity > formerDissimilarity )
      {
    
        this->setValues( formerValues ); 

      }  

    } 

    else
    {

      throw std::runtime_error( "affine reorientation is available"
                                "only in standard basis" );

    }
       
  }
  GKG_CATCH( "void gkg::ProbabilityDensityFunction::"
             "affineReorientation(" 
             "gkg::Matrix& jacobianMatrix,"
             "gkg::ProbabilityDensityFunction& otherPdf )" );

}


std::vector< float > 
gkg::ProbabilityDensityFunction::getDisplacementProbabilities() const
{

  try
  {

    // updating displacement set cache to speed-up calculation
    if ( _outputMultipleShellSampling )
    {

      gkg::DisplacementSetCache::getInstance().update( 
                                                *_outputMultipleShellSampling );
    }
    else
    {

      gkg::DisplacementSetCache::getInstance().update( 
                                              *_outputCartesianGridSize,
                                              *_outputCartesianGridResolution );

    }
    int32_t displacementCount = 
                            gkg::DisplacementSetCache::getInstance().getCount();
    const std::vector< gkg::Vector3d< float > >& displacements = 
                    gkg::DisplacementSetCache::getInstance().getDisplacements();

    std::vector< float > probabilities;

    if ( _basisType == gkg::ProbabilityDensityFunction::Standard )
    {
      probabilities = this->_values;

    }
    else if ( _basisType == gkg::ProbabilityDensityFunction::SecondOrderTensor )
    {



    }
    else if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      // _values[  0 ] -> maximum order O of polar harmonics basis
      // _values[  1 ] -> rotation described using : - thetaX angle (in radian)
      // _values[  2 ] ->                            - thetaY angle (in radian)
      // _values[  3 ] ->                            - thetaZ angle (in radian)
      // _values[  4 ] -> scaling of the EAP along : - X axis
      // _values[  5 ] ->                            - Y axis
      // _values[  6 ] ->                            - Z axis
      // _values[  7 ] -> N polar harmonics coefficients
      // _values[ .. ] -> N = ( ( O/2 + 1 ) * ( O/2 + 2 ) * ( 2 * O + 3 ) ) / 6
      // _values[  N + 6 ] -> N polar harmonics coefficients


      // collecting the basis order and computing the final number of
      // coefficients of the polar harmonics basis

      int32_t basisOrder = this->_values[ 0 ];

      // updating the polar harmonics cache (computation of tuple (n1,n2,n3), ..
      // must have been already done previously using 
      // updatePolarHarmonicsCache()
      gkg::PolarHarmonicsCache& polarHarmonicsCache = 
                                        gkg::PolarHarmonicsCache::getInstance();
      polarHarmonicsCache.update( basisOrder );

      int32_t coefficientCount = polarHarmonicsCache.getCoefficientCount();

      // computing rotation matrix
      gkg::Rotation3d< float > rotation( this->_values[ 1 ],
                                         this->_values[ 2 ],
                                         this->_values[ 3 ] );

      // computing scaling matrix
      gkg::Vector3d< float > scaling( this->_values[ 4 ],
                                      this->_values[ 5 ],
                                      this->_values[ 6 ] );

      // rotating displacement vectors
      std::vector< gkg::Vector3d< float > >
        rotatedDisplacements( displacementCount );
      std::vector< gkg::Vector3d< float > >::const_iterator
        d = displacements.begin(),
        de = displacements.end();
      std::vector< gkg::Vector3d< float > >::iterator
        rd = rotatedDisplacements.begin();
      while ( d != de )
      {

        rotation.getInverse( *d, *rd );
        ++ d;
        ++ rd;

      }

      // resizing output probability vector
      probabilities.resize( displacementCount );

      // composing probability at the various displacements
      int32_t displacementIndex = 0;
      int32_t coefficientIndex = 0;        
      for ( displacementIndex = 0; displacementIndex < displacementCount;
            displacementIndex++ )
      {

        float& probability = probabilities[ displacementIndex ];

        probability = 0.0;
        for ( coefficientIndex = 0; coefficientIndex < coefficientCount; 
              coefficientIndex++ )
        {

          probability += this->_values[ 7 + coefficientIndex ] * 
                         polarHarmonicsCache.getRSpaceBasisFunction3dAt(
                                                           rotatedDisplacements,
                                                           scaling,
                                                           displacementIndex,
                                                           coefficientIndex );

        }
        probabilities[ displacementIndex ] = probability;

      }

    }

    return probabilities;

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::ProbabilityDensityFunction::getDisplacementProbabilities() "
             "const" );

}


float gkg::ProbabilityDensityFunction::getDisplacementProbability(
                                                           int32_t index ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) ||
         ( index >= 
           gkg::DisplacementSetCache::getInstance().getInstance().getCount() ) )
    {

      throw std::runtime_error( "invalid index" );

    }

#endif

    float probability = 0.0;

    if ( _basisType == gkg::ProbabilityDensityFunction::Standard )
    {

      probability = this->_values[ index ];

    }
    else if ( ( _basisType ==
                gkg::ProbabilityDensityFunction::SecondOrderTensor ) ||
              ( _basisType ==
                gkg::ProbabilityDensityFunction::PolarHarmonics ) )
    {

      std::vector< float > probabilities = getDisplacementProbabilities();
      probability = probabilities[ index ];

    }

    return probability;

  }
  GKG_CATCH( "float "
             "gkg::ProbabilityDensityFunction::getDisplacementProbability( "
             "int32_t index ) const" );

}


float gkg::ProbabilityDensityFunction::getDisplacementProbability( 
                              const gkg::Vector3d< float >& displacement ) const
{

  try
  {

    float probability = 0.0;

    if ( _basisType == gkg::ProbabilityDensityFunction::Standard )
    {

      probability =
      this->_values[ gkg::DisplacementSetCache::getInstance().getInstance().
                                 getNearestDisplacementIndex( displacement ) ];

    }
    else if ( ( _basisType ==
                gkg::ProbabilityDensityFunction::SecondOrderTensor ) ||
              ( _basisType ==
                gkg::ProbabilityDensityFunction::PolarHarmonics ) )
    {

      std::vector< float > probabilities = getDisplacementProbabilities();
      probability =
       probabilities[ gkg::DisplacementSetCache::getInstance().getInstance().
                                 getNearestDisplacementIndex( displacement ) ];

    }

    return probability;

  }
  GKG_CATCH( "float "
             "gkg::ProbabilityDensityFunction::getDisplacementProbability( "
             "const gkg::Vector3d< float >& displacement ) const" );

}


double gkg::ProbabilityDensityFunction::dot( 
                           const gkg::ProbabilityDensityFunction& other ) const
{

  try
  {

    if ( other.getBasisType() != _basisType )
    {

      throw std::runtime_error(
                    "cannot compute dot product when PDF basis types differ" );

    }

    double dotProduct = 0.0f;
    if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      const std::vector< float >& otherValues = other.getValues();

      if ( this->_values[ 0 ] != otherValues[ 0 ] )
      {

        throw std::runtime_error( "PDFs do not have the same PH order" );

      }

      // collecting scaling parameters of both PDFs
      gkg::Vector3d< float > scalingParameters( this->_values[ 4 ],
                                                this->_values[ 5 ],
                                                this->_values[ 6 ] );

      gkg::Vector3d< float > otherScalingParameters( otherValues[ 4 ],
                                                     otherValues[ 5 ],
                                                     otherValues[ 6 ] );

      // getting acces to the polar harmonics cache
      gkg::PolarHarmonicsCache& polarHarmonicsCache = 
                                       gkg::PolarHarmonicsCache::getInstance();
      this->updatePolarHarmonicsCache();

      // collecting the number of basis indices and the basis indices
      int32_t phCoefficientCount = polarHarmonicsCache.getCoefficientCount();

      const std::vector< gkg::Vector3d< int32_t > >& basisIndices = 
                                        polarHarmonicsCache.getBasisIndices();

      int32_t c1 = 0;
      int32_t c2 = 0;

      for ( c1 = 0; c1 < phCoefficientCount; c1++ )
      {

        const gkg::Vector3d< int32_t >& n1n2n3 = basisIndices[ c1 ];
        for ( c2 = 0; c2 < phCoefficientCount; c2++ )
        {

          const gkg::Vector3d< int32_t >& otherN1n2n3 = basisIndices[ c2 ];
          dotProduct += ( double )this->_values[ 7 + c1 ] *
                        ( double )otherValues[ 7 + c2 ] *
                        ( double )polarHarmonicsCache.getTmn(
                                           n1n2n3.x,
                                           otherN1n2n3.x,
                                           scalingParameters.x,
                                           otherScalingParameters.x ) *
                        ( double )polarHarmonicsCache.getTmn(
                                           n1n2n3.y,
                                           otherN1n2n3.y,
                                           scalingParameters.y,
                                           otherScalingParameters.y ) *
                        ( double )polarHarmonicsCache.getTmn(
                                           n1n2n3.z,
                                           otherN1n2n3.z,
                                           scalingParameters.z,
                                           otherScalingParameters.z );


        }

      }

    }
    else if( _basisType == gkg::ProbabilityDensityFunction::Standard )
    {

      const std::vector< float >& otherValues = other.getValues();

      // computed "windowed dot product" (i.e. cross correlation)
      /*
      int32_t n = 0;
      int32_t m = 0;
      int32_t mPlusn = 0;
      int32_t windowSize = 1;
      for ( m = 0; m < windowSize; ++m )
      {

        for( n = 0; n < _values.size(); ++n )
        { 

          mPlusn = ( m + n ) % _values.size();

          dotProduct += ( double )( ( double )_values[ n ] * 
                                    ( double )otherValues[ mPlusn ] );
  
        }

      } 
      
      if ( dotProduct == 0.0 )
        std::cout << "dotProduct = " << dotProduct << std::endl;
      */

      
      dotProduct = std::inner_product( _values.begin(), _values.end(), 
                                       otherValues.begin(), 0.0);
      

    }


    return dotProduct;

  }
  GKG_CATCH( "float gkg::ProbabilityDensityFunction::dot( "
             "const ProbabilityDensityFunction& other ) const" );

}

float gkg::ProbabilityDensityFunction::getDissimilarity( 
                                   const gkg::ProbabilityDensityFunction& other,
                                   float orientationWeight,
                                   float shapeWeight ) const
{

  try
  {

    float orientationDissimilarity = 0.0f;
    float dissimilarity = 0.0f;
    double thisdotOther = 0.0;

    if ( other.getBasisType() != _basisType )
    {

      throw std::runtime_error(
                    "cannot compute similarity when PDF basis types differ" );

    }

    thisdotOther = ( double )this->dot( other );
    dissimilarity = 1.0f -  thisdotOther * thisdotOther / 
                                        ( this->dot( *this ) * 
                                          other.dot( other ) );
    dissimilarity = std::sqrt( dissimilarity );

    if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      dissimilarity *= shapeWeight;

      orientationDissimilarity = 
        std::sqrt( 
          std::pow( (float)( ( this->getValue( 1 ) + 
                               M_PI * ( 
                                 ( this->getValue( 1 ) < 0.0 ) ? 1.0 : 0.0 ) ) -
                             ( other.getValue( 1 ) + 
                               M_PI * ( 
                                 ( other.getValue( 1 ) < 0.0 ) ? 1.0 : 0.0 ) )
                           ), 2 ) +
          std::pow( (float)( ( this->getValue( 2 ) + 
                               M_PI * ( 
                                 ( this->getValue( 2 ) < 0.0 ) ? 1.0 : 0.0 ) ) -
                             ( other.getValue( 2 ) + 
                               M_PI * ( 
                                 ( other.getValue( 2 ) < 0.0 ) ? 1.0 : 0.0 ) )
                           ), 2 ) +
          std::pow( (float)( ( this->getValue( 3 ) + 
                               M_PI * ( 
                                 ( this->getValue( 3 ) < 0.0 ) ? 1.0 : 0.0 ) ) -
                             ( other.getValue( 3 ) + 
                               M_PI * ( 
                                 ( other.getValue( 3 ) < 0.0 ) ? 1.0 : 0.0 ) )
                           ), 2 ) );

      dissimilarity += orientationWeight * orientationDissimilarity;

    }
    

    //std::cout << "shapeSimilarity = " 
    //          << shapeWeight * shapeSimilarity << std::endl;
    //std::cout << "orientationSimilarity = " 
    //          << orientationWeight * orientationSimilarity << std::endl;

    return dissimilarity;

  }
  GKG_CATCH( "float gkg::ProbabilityDensityFunction::getDissimilarity( "
             "gkg::ProbabilityDensityFunction& other,"
             "float& orientationWeight,"
             "float& shapeWeight ) const" );

}


float gkg::ProbabilityDensityFunction::getPropagatorAnisotropy() const
{

  try
  {


    float propagatorAnisotropy = 0.0f;

    if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      ///propagatorAnisotropy = 

    }

    return propagatorAnisotropy;

  }
  GKG_CATCH( "float "
             "gkg::ProbabilityDensityFunction::getPropagatorAnisotropy() "
             "const" );

}


float gkg::ProbabilityDensityFunction::getReturnToOriginProbability() const
{

  try
  {

    float returnToOriginProbability = 0.0f;

    if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      // _values[  0 ] -> maximum order O of polar harmonics basis
      // _values[  1 ] -> rotation described using : - thetaX angle (in radian)
      // _values[  2 ] ->                            - thetaY angle (in radian)
      // _values[  3 ] ->                            - thetaZ angle (in radian)
      // _values[  4 ] -> scaling of the EAP along : - X axis
      // _values[  5 ] ->                            - Y axis
      // _values[  6 ] ->                            - Z axis
      // _values[  7 ] -> N polar harmonics coefficients
      // _values[ .. ] -> N = ( ( O/2 + 1 ) * ( O/2 + 2 ) * ( 2 * O + 3 ) ) / 6
      // _values[  N + 6 ] -> N polar harmonics coefficients

      // collecting the scaling parameters
      gkg::Vector3d< float > scalingParameters( _values[ 4 ],
                                                _values[ 5 ],
                                                _values[ 6 ] );

      float tensorDeterminant = ( scalingParameters.x * 
                                  scalingParameters.x ) *
                                ( scalingParameters.y * 
                                  scalingParameters.y ) *
                                ( scalingParameters.z * 
                                  scalingParameters.z );

      // getting acces to the polar harmonics cache
      gkg::PolarHarmonicsCache& polarHarmonicsCache = 
                                       gkg::PolarHarmonicsCache::getInstance();

      // collecting the number of basis indices
      int32_t phCoefficientCount = polarHarmonicsCache.getCoefficientCount();

      const std::vector< gkg::Vector3d< int32_t > >& basisIndices = 
                                        polarHarmonicsCache.getBasisIndices();
      const std::vector< float >& bN1N2N3s = polarHarmonicsCache.getBN1N2N3s();


      int32_t c = 0;
      int32_t N = 0;
      for ( c = 0; c < phCoefficientCount; c++ )
      {

        const gkg::Vector3d< int32_t >& n1n2n3 = basisIndices[ c ];
        N = n1n2n3.x + n1n2n3.y + n1n2n3.z;

        returnToOriginProbability += ( ( ( N / 2 ) % 2 == 1 ) ? -1.0f :
                                                                +1.0f ) *
                                     this->_values[ 7 + c ] *
                                     bN1N2N3s[ c ];

      }

      returnToOriginProbability *= 1 /
                                   ( float )std::sqrt( 8.0f *
                                                       M_PI * M_PI * M_PI *
                                                       tensorDeterminant );
    }

    return returnToOriginProbability;

  }
  GKG_CATCH( "float "
             "gkg::ProbabilityDensityFunction::getReturnToOriginProbability() "
             "const" );

}


float gkg::ProbabilityDensityFunction::getReturnToAxisProbability() const
{

  try
  {

    float returnToAxisProbability = 0.0f;

    if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      // _values[  0 ] -> maximum order O of polar harmonics basis
      // _values[  1 ] -> rotation described using : - thetaX angle (in radian)
      // _values[  2 ] ->                            - thetaY angle (in radian)
      // _values[  3 ] ->                            - thetaZ angle (in radian)
      // _values[  4 ] -> scaling of the EAP along : - X axis
      // _values[  5 ] ->                            - Y axis
      // _values[  6 ] ->                            - Z axis
      // _values[  7 ] -> N polar harmonics coefficients
      // _values[ .. ] -> N = ( ( O/2 + 1 ) * ( O/2 + 2 ) * ( 2 * O + 3 ) ) / 6
      // _values[  N + 6 ] -> N polar harmonics coefficients

      // collecting the scaling parameters
      gkg::Vector3d< float > scalingParameters( _values[ 4 ],
                                                _values[ 5 ],
                                                _values[ 6 ] );

      // getting acces to the polar harmonics cache
      gkg::PolarHarmonicsCache& polarHarmonicsCache = 
                                       gkg::PolarHarmonicsCache::getInstance();

      // collecting the number of basis indices
      int32_t phCoefficientCount = polarHarmonicsCache.getCoefficientCount();

      const std::vector< gkg::Vector3d< int32_t > >& basisIndices = 
                                        polarHarmonicsCache.getBasisIndices();
      const std::vector< float >& bN1N2N3s = polarHarmonicsCache.getBN1N2N3s();


      int32_t c = 0;
      int32_t N = 0;
      for ( c = 0; c < phCoefficientCount; c++ )
      {

        const gkg::Vector3d< int32_t >& n1n2n3 = basisIndices[ c ];
        N = n1n2n3.y + n1n2n3.z;

        returnToAxisProbability += ( ( ( N / 2 ) % 2 == 1 ) ? -1.0f :
                                                                +1.0f ) *
                                   this->_values[ 7 + c ] *
                                   bN1N2N3s[ c ];

      }

      returnToAxisProbability *= 1.0f /
                                 ( float )( 2.0f * M_PI * 
                                            scalingParameters.y *
                                            scalingParameters.z );

    }

    return returnToAxisProbability;

  }
  GKG_CATCH( "float "
             "gkg::ProbabilityDensityFunction::getReturnToAxisProbability() "
             "const" );

}


float gkg::ProbabilityDensityFunction::getReturnToPlaneProbability() const
{

  try
  {

    float returnToPlaneProbability = 0.0f;

    if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      // _values[  0 ] -> maximum order O of polar harmonics basis
      // _values[  1 ] -> rotation described using : - thetaX angle (in radian)
      // _values[  2 ] ->                            - thetaY angle (in radian)
      // _values[  3 ] ->                            - thetaZ angle (in radian)
      // _values[  4 ] -> scaling of the EAP along : - X axis
      // _values[  5 ] ->                            - Y axis
      // _values[  6 ] ->                            - Z axis
      // _values[  7 ] -> N polar harmonics coefficients
      // _values[ .. ] -> N = ( ( O/2 + 1 ) * ( O/2 + 2 ) * ( 2 * O + 3 ) ) / 6
      // _values[  N + 6 ] -> N polar harmonics coefficients

      // collecting the scaling parameters
      gkg::Vector3d< float > scalingParameters( _values[ 4 ],
                                                _values[ 5 ],
                                                _values[ 6 ] );

      // getting acces to the polar harmonics cache
      gkg::PolarHarmonicsCache& polarHarmonicsCache = 
                                       gkg::PolarHarmonicsCache::getInstance();

      // collecting the number of basis indices
      int32_t phCoefficientCount = polarHarmonicsCache.getCoefficientCount();

      const std::vector< gkg::Vector3d< int32_t > >& basisIndices = 
                                        polarHarmonicsCache.getBasisIndices();
      const std::vector< float >& bN1N2N3s = polarHarmonicsCache.getBN1N2N3s();


      int32_t c = 0;
      for ( c = 0; c < phCoefficientCount; c++ )
      {

        const gkg::Vector3d< int32_t >& n1n2n3 = basisIndices[ c ];

        returnToPlaneProbability += ( ( ( n1n2n3.x / 2 ) % 2 == 1 ) ? -1.0f :
                                                                      +1.0f ) *
                                    this->_values[ 7 + c ] *
                                    bN1N2N3s[ c ];

      }

      returnToPlaneProbability *= 1.0f /
                                  ( ( float )std::sqrt( 2.0f * M_PI ) * 
                                    scalingParameters.x );

    }

    return returnToPlaneProbability;

  }
  GKG_CATCH( "float "
             "gkg::ProbabilityDensityFunction::getReturnToPlaneProbability() "
             "const" );

}


float gkg::ProbabilityDensityFunction::getNonGaussianity() const
{

  try
  {

    return 1.0;


  }
  GKG_CATCH( "float "
             "gkg::ProbabilityDensityFunction::getNonGaussianity() "
             "const" );

}


gkg::PdfMoment 
gkg::ProbabilityDensityFunction::getRadialMoment(
                                   int32_t order,
                                   const OrientationSet& orientationSet ) const
{

  try
  {

    // getting access to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // collecting the orientation count
    int32_t orientationCount = orientationSet.getCount();

    // allocating a new pdf moment
    gkg::PdfMoment pdfMoment( order, gkg::PdfMoment::Standard );

    if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      // _values[  0 ] -> maximum order O of polar harmonics basis
      // _values[  1 ] -> rotation described using : - thetaX angle (in radian)
      // _values[  2 ] ->                            - thetaY angle (in radian)
      // _values[  3 ] ->                            - thetaZ angle (in radian)
      // _values[  4 ] -> scaling of the EAP along : - X axis
      // _values[  5 ] ->                            - Y axis
      // _values[  6 ] ->                            - Z axis
      // _values[  7 ] -> N polar harmonics coefficients
      // _values[ .. ] -> N = ( ( O/2 + 1 ) * ( O/2 + 2 ) * ( 2 * O + 3 ) ) / 6
      // _values[  N + 6 ] -> N polar harmonics coefficients

      // getting acces to the polar harmonics cache
      gkg::PolarHarmonicsCache& polarHarmonicsCache = 
                                       gkg::PolarHarmonicsCache::getInstance();

      // collecting the number of basis indices
      int32_t phCoefficientCount = polarHarmonicsCache.getCoefficientCount();
      const std::vector< gkg::Vector3d< int32_t > >& basisIndices = 
                                        polarHarmonicsCache.getBasisIndices();
      const std::vector< float >& sqrtOfFactorialOfN1N2N3s = 
                             polarHarmonicsCache.getSqrtOfFactorialOfN1N2N3s();

      // allocating the reconstruction matrix required to compute radial
      // moment(s) of order 'order' for all the orientation(s)
      gkg::Matrix radialMomentMatrix( orientationCount, phCoefficientCount );

      // collecting the scaling parameters
      gkg::Vector3d< float > scalingParameters( this->_values[ 4 ],
                                                this->_values[ 5 ],
                                                this->_values[ 6 ] );
      float tensorDeterminant = ( scalingParameters.x * 
                                  scalingParameters.x ) *
                                ( scalingParameters.y * 
                                  scalingParameters.y ) *
                                ( scalingParameters.z * 
                                  scalingParameters.z );

      // building the rotation
      gkg::Rotation3d< float > rotation3d( this->_values[ 1 ],
                                           this->_values[ 2 ],
                                           this->_values[ 3 ] );

      std::vector< float > rho( orientationCount );
      std::vector< float > alpha( orientationCount );
      std::vector< float > beta( orientationCount );
      std::vector< float > gamma( orientationCount );
      std::vector< float > normalizationFactor( orientationCount );
      int32_t o = 0;
      float orientationOverScaleX = 0.0;
      float orientationOverScaleY = 0.0;
      float orientationOverScaleZ = 0.0;
      for ( o = 0; o < orientationCount; o++ )
      {


        // remember that the PDF has to be rotated after computation; since the
        // provided orientation set corresponds to the final set chosen for
        // visualization, we need to go back to the orientation set before 
        // rotation, and therefore compute for each orientation its inverse
        // using the rotation matrix stemming from the tensor computation
        gkg::Vector3d< float > orientation;
        rotation3d.getInverse( orientationSet.getOrientation( o ),
                               orientation );
        orientationOverScaleX = orientation.x / scalingParameters.x;
        orientationOverScaleY = orientation.y / scalingParameters.y;
        orientationOverScaleZ = orientation.z / scalingParameters.z;
        rho[ o ] = 1.0f / ( float )std::sqrt( ( orientationOverScaleX * 
                                                orientationOverScaleX ) +
                                              ( orientationOverScaleY *
                                                orientationOverScaleY ) +
                                              ( orientationOverScaleZ *
                                                orientationOverScaleZ ) );

        alpha[ o ] = 2.0 * rho[ o ] * orientationOverScaleX;
        beta[ o ] = 2.0 * rho[ o ] * orientationOverScaleY;
        gamma[ o ] = 2.0 * rho[ o ] * orientationOverScaleZ;

        normalizationFactor[ o ] = ( float )std::pow( rho[ o ], 3 + order ) /
                                   ( float )std::sqrt( 
                                                     std::pow( 2, 2 - order ) *
                                                     M_PI * M_PI * M_PI *
                                                     tensorDeterminant );

      }

      int32_t c = 0;
      int32_t i = 0;
      int32_t j = 0;
      int32_t k = 0;
      int32_t n = 0;
      float numerator = 0.0f;
      float denominator = 0.0f;
      for ( c = 0; c < phCoefficientCount; c++ )
      {

        const gkg::Vector3d< int32_t >& n1n2n3 = basisIndices[ c ];

        float sqrtOfN1FactN2FactN3Fact = sqrtOfFactorialOfN1N2N3s[ c ];

        for ( o = 0; o < orientationCount; o++ )
        {

          double C = 0.0;
          for ( i = 0; i < n1n2n3.x + 1; i += 2 )
          {
 
            for ( j = 0; j < n1n2n3.y + 1; j += 2 )
            {

              for ( k = 0; k < n1n2n3.z + 1; k += 2 )
              {

                n = n1n2n3.x + n1n2n3.y + n1n2n3.z - i - j - k;
                numerator = ( ( ( ( i + j + k ) / 2 ) % 2 == 1 ) ? -1.0f :
                                                                   +1.0f ) *
                            ( float )factory->getGammaFunction( 
                                     ( double )( 3 + order + n ) / 2.0 ) *
                            ( float )std::pow( alpha[ o ], n1n2n3.x - i ) *
                            ( float )std::pow( beta[ o ], n1n2n3.y - j ) *
                            ( float )std::pow( gamma[ o ], n1n2n3.z - k );
                denominator = factory->getFactorialFunction(
                                           ( double )( n1n2n3.x - i ) ) *
                              factory->getFactorialFunction(
                                           ( double )( n1n2n3.y - j ) ) *
                              factory->getFactorialFunction(
                                           ( double )( n1n2n3.z - k ) ) *
                              factory->getFactorialFunction(
                                         factory->getFactorialFunction(
                                                 ( double )i ) ) *
                              factory->getFactorialFunction(
                                         factory->getFactorialFunction(
                                                 ( double )j ) ) *
                              factory->getFactorialFunction(
                                         factory->getFactorialFunction(
                                                 ( double )k ) );
                C += numerator / denominator;
                              

              }

            }

          }


          radialMomentMatrix( o, c ) = ( double )normalizationFactor[ o ] * 
                                       ( double )sqrtOfN1FactN2FactN3Fact * 
                                       C;

        }

      }

      gkg::Vector polarHarmonicsCoefficients;
      this->getPolarHarmonicsCoefficients( polarHarmonicsCoefficients );

      pdfMoment.setValues( radialMomentMatrix.getComposition(
                                                polarHarmonicsCoefficients ) );

    }

    return pdfMoment;

  }
  GKG_CATCH( "gkg::PdfMoment "
             "gkg::ProbabilityDensityFunction::getRadialMoment( "
             "int32_t order ) const" );

}


gkg::OrientationDistributionFunction 
gkg::ProbabilityDensityFunction::getOrientationDistributionFunction(
                              const gkg::OrientationSet& orientationSet ) const
{

  try
  {

    gkg::OrientationDistributionFunction::BasisType
      odfBasisType = gkg::OrientationDistributionFunction::Standard;

    if ( _basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      odfBasisType = gkg::OrientationDistributionFunction::Standard;

    }

    gkg::OrientationDistributionFunction odf( odfBasisType, &orientationSet );

    gkg::Vector 
      moment( this->getRadialMoment( 0, orientationSet ).getValues() );

    odf.setValues( moment.normalize() );

    return odf;

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::ProbabilityDensityFunction::"
             "getOrientationDistributionFunction( "
             "const gkg::OrientationSet& orientationSet ) const" );

}


gkg::OrientationDistributionFunction 
gkg::ProbabilityDensityFunction::
                              getOrientationDistributionFunctionFromSingleShell(
        const gkg::Matrix& Bplus ) const
{

  try
  {

    gkg::OrientationDistributionFunction
      odf( gkg::OrientationDistributionFunction::SphericalHarmonics,
           &( _outputMultipleShellSampling->begin()->second ) );
    odf.setValues( Bplus.getComposition(
            gkg::Vector( this->getDisplacementProbabilities() ).normalize() ) );

    return odf;

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::ProbabilityDensityFunction::"
             "getOrientationDistributionFunction( "
             "const gkg::Matrix& Bplus ) const" );

}


gkg::OrientationDistributionFunction 
gkg::ProbabilityDensityFunction::
                              getOrientationDistributionFunctionFromSingleShell(
                              const int32_t& maximumSphericalHarmonicOrder,
                              const double& regularizationLCurveFactor ) const
{

  try
  {

    // collecting orientation set and count
    const gkg::OrientationSet& 
                 orientationSet = _outputMultipleShellSampling->begin()->second;
    int32_t orientationCount =  orientationSet.getCount();

    // constructing Bplus matrix 
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    int32_t N = ( ( maximumSphericalHarmonicOrder + 1 ) *
   	          ( maximumSphericalHarmonicOrder + 2 ) ) / 2;
    int32_t M = orientationCount;
    gkg::Matrix B( M, N );
    gkg::Matrix Bplus( N, M );
    int32_t n, m;
    gkg::Vector3d< float > orientation;
    double radial;
    double theta;
    double phi;
    for ( m = 0; m < M; m++ )
    {

      orientation = orientationSet.getOrientation( m );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
   	                      			   ( double )orientation.y,
   	 					   ( double )orientation.z,
   						   radial,
   						   theta,
   						   phi );
      for ( n = 0; n < N; n++ )
      {

   	B( m, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    // building Laplace-Beltrami smoothness matrix for regularization
    gkg::Matrix L( N, N );
    L.setZero();
    double sphericalLaplacian = 0;
    int32_t l;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      sphericalLaplacian = - l * ( l + 1 );
      L( n, n ) = sphericalLaplacian * sphericalLaplacian;

    }

    // building Bplus matrix with size N x M
    // Bplus = (B^T B + l L )^(-1) B^T
    Bplus.reallocate( N, M );
    factory->getThikonovPseudoInverse( B, L,
  				       ( double )regularizationLCurveFactor,
  				       Bplus );

    return this->getOrientationDistributionFunctionFromSingleShell( Bplus );

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::ProbabilityDensityFunction::"
             "getOrientationDistributionFunction( "
             "const int32_t& maximumSphericalHarmonicOrder, "
             "const double& regularizationLCurveFactor ) const" );

}


void gkg::ProbabilityDensityFunction::getPolarHarmonicsCoefficients(
                       std::vector< float >& polarHarmonicsCoefficients ) const
{

  try
  {

    int32_t phCoefficientCount = ( int32_t )this->_values.size() - 7;

    polarHarmonicsCoefficients.resize( phCoefficientCount );
    int32_t c = 0;
    for ( c = 0; c < phCoefficientCount; c++ )
    {

      polarHarmonicsCoefficients[ c ] = this->_values[ c + 7 ];

    }

  }
  GKG_CATCH( "void "
             "gkg::ProbabilityDensityFunction::getPolarHarmonicsCoefficients( "
             "std::vector< float >& polarHarmonicsCoefficients ) const" );

}


void gkg::ProbabilityDensityFunction::getPolarHarmonicsCoefficients(
                   gkg::Vector& polarHarmonicsCoefficients ) const
{

  try
  {

    int32_t phCoefficientCount = ( int32_t )this->_values.size() - 7;

    polarHarmonicsCoefficients.reallocate( phCoefficientCount );
    int32_t c = 0;
    for ( c = 0; c < phCoefficientCount; c++ )
    {

      polarHarmonicsCoefficients( c ) = ( double )this->_values[ c + 7 ];

    }

  }
  GKG_CATCH( "void "
             "gkg::ProbabilityDensityFunction::getPolarHarmonicsCoefficients( "
             "gkg::Vector& polarHarmonicsCoefficients ) const" );

}


