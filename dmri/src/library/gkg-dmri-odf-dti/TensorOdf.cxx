#include <gkg-dmri-odf-dti/TensorOdf.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-core-exception/Exception.h>



gkg::TensorOdf::TensorOdf( const gkg::OrientationSet* orientationSet,
                           const gkg::Tensor& tensor  )
               : gkg::OrientationDistributionFunction(
                   gkg::OrientationDistributionFunction::Standard,
                   orientationSet ),
                 _tensor( tensor )
{

  try
  {

    if ( _orientationSet )
    {

      // p(r)=1/sqrt( (4 Pi Tau)^3 |D| ) x exp( - ( 1 / ( 4 Tau ) r^T D^-1 r ) )
      // psy(o)=int_{0 to inf} p(r o) dr
      //       =0.5 / sqrt( (4 Pi tau)^2 |D| o^T D^-1 o )

      float fourPiTauSquare = 4 * M_PI * _tensor.getDiffusionTime();
      fourPiTauSquare *= fourPiTauSquare; 

      float norm = _tensor.getNorm();

      int32_t o;
      if ( norm > 0.0 )
      {

        for ( o = 0; o < _orientationSet->getCount(); o++ )
        {

          _values[ o ] = 0.5 / std::sqrt( fourPiTauSquare * _tensor.getNorm() *
                             _tensor.getInverseDiffusionCoefficient( 
                                       _orientationSet->getOrientation( o ) ) );

        }

      }

      // normalizing
      float sum = 0.0;
      for ( o = 0; o < _orientationSet->getCount(); o++ )
      {

        sum += _values[ o ];

      }

      if ( sum > 0.0 )
      {

        for ( o = 0; o < _orientationSet->getCount(); o++ )
        {

          _values[ o ] /= sum;

        }

      }

    }

  }
  GKG_CATCH( "gkg::TensorOdf::TensorOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Tensor& tensor ) " );

}


gkg::TensorOdf::~TensorOdf()
{
}


float gkg::TensorOdf::getAnisotropy() const
{

  return _tensor.getFractionalAnisotropy();

}


const gkg::Tensor& gkg::TensorOdf::getTensor() const
{

  return _tensor;

}

