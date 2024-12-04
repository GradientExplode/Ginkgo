#ifndef _gkg_processing_bias_EntropyBiasFunction2d_i_h_
#define _gkg_processing_bias_EntropyBiasFunction2d_i_h_


#include <gkg-processing-bias/EntropyBiasFunction2d.h>
#include <gkg-processing-bias/BiasNelderMeadFunction2d_i.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


#define GKG_ENTROPY_SMOOTHING_SIGMA  1.0


template < class T >
inline
gkg::EntropyBiasFunction2d< T >::EntropyBiasFunction2d(
                            gkg::RCPointer< gkg::BiasFunctor< T > > biasFunctor,
                            const gkg::Volume< T >& in,
                            const gkg::Volume< int16_t >& mask,
                            const gkg::Volume< float >& weight,
                            gkg::Volume< T >& out,
                            gkg::Volume< float >& bias,
                            const std::string& planeAxis,
                            int32_t levelCount,
                            bool isSmoothingApplied )
                              : gkg::BiasNelderMeadFunction2d< T >( biasFunctor,
                                                                    in,
                                                                    mask,
                                                                    weight,
                                                                    out,
                                                                    bias,
                                                                    planeAxis ),
                                   _levelCount( levelCount ),
                                   _isSmoothingApplied( isSmoothingApplied )
{

  try
  {

    if ( _isSmoothingApplied )
    {

      _dericheGaussianCoefficients = 
        new gkg::DericheGaussianCoefficients< double >(
              GKG_ENTROPY_SMOOTHING_SIGMA,
              gkg::DericheGaussianCoefficients< double >::Smoothing );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::EntropyBiasFunction2d< T >::EntropyBiasFunction2d( "
             "gkg::RCPointer< gkg::BiasFunctor< T > > biasFunctor, "
             "const gkg::Volume< T >& in, "
             "const gkg::Volume< int16_t >& mask, "
             "const gkg::Volume< float >& weight, "
             "gkg::Volume< T >& out, "
             "gkg::Volume< float >& bias, "
             "const std::string& planeAxis, "
             "int32_t levelCount, "
             "bool isSmoothingApplied )" );

}


template < class T >
inline
gkg::EntropyBiasFunction2d< T >::~EntropyBiasFunction2d()
{

  if ( _isSmoothingApplied )
  {

    delete _dericheGaussianCoefficients;

  }

}


template < class T >
inline
double gkg::EntropyBiasFunction2d< T >::getValueAt(
                                            const gkg::Vector& parameter ) const
{

  try
  {

    gkg::Matrix probabilities( _levelCount, 1 );
    probabilities.setZero();
    int32_t level = 0;

    if ( this->_planeAxis == "x" )
    {

      int32_t lineCount = this->_in.getSizeY();
      int32_t sliceCount = this->_in.getSizeZ();

      double minimum = 1e38;
      double maximum = -1e38;

      int32_t line, slice;
      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( line = 0; line < lineCount; line++ )
        {

          if ( this->_mask( this->_referenceSlice, line, slice ) &&
               ( this->_weight( this->_referenceSlice, line, slice ) > 0.0 ) )
          {

            double value = ( double )this->_out( this->_referenceSlice,
                                                 line, slice, this->_time );
            if ( value < minimum )
            {

              minimum = value;

            } 
            else if ( value > maximum )
            {

              maximum = value;

            } 

          }

          if ( this->_mask( this->_biasedSlice, line, slice ) &&
               ( this->_weight( this->_biasedSlice, line, slice ) > 0.0 ) )
          {

            double value = this->_biasFunctor->getValueAt( 
                                         this->_out( this->_biasedSlice,
                                                     line, slice, this->_time ),
                                         parameter( 0 ) );
            if ( value < minimum )
            {

              minimum = value;

            } 
            else if ( value > maximum )
            {

              maximum = value;

            } 

          }

        }

      }

      // processing raw join probabilities
      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( line = 0; line < lineCount; line++ )
        {

          if ( this->_mask( this->_referenceSlice, line, slice ) &&
               ( this->_weight( this->_referenceSlice, line, slice ) > 0.0 ) )
          {

            double value = ( double )this->_out( this->_referenceSlice,
                                                 line, slice, this->_time );
            level = ( int32_t )( ( ( value - minimum ) /
                                   ( maximum - minimum ) ) *
                                 ( this->_levelCount - 1 ) );
            if ( level < 0 )
            {

               level = 0;

            }
            else if ( level >= this->_levelCount )
            {

               level = this->_levelCount - 1;

            }
            ++ probabilities( level, 0 );

          }

          if ( this->_mask( this->_biasedSlice, line, slice ) &&
               ( this->_weight( this->_biasedSlice, line, slice ) > 0.0 ) )
          {

            double value = this->_biasFunctor->getValueAt( 
                                    this->_out( this->_biasedSlice, line, slice,
                                                this->_time ),
                                    parameter( 0 ) );
            level = ( int32_t )( ( ( value - minimum ) /
                                   ( maximum - minimum ) ) *
                                 ( this->_levelCount - 1 ) );
            if ( level < 0 )
            {

               level = 0;

            }
            else if ( level >= this->_levelCount )
            {

               level = this->_levelCount - 1;

            }
            ++ probabilities( level, 0 );

          }

        }

      }

    }
    else if ( this->_planeAxis == "y" )
    {

      int32_t sizeX = this->_in.getSizeX();
      int32_t sliceCount = this->_in.getSizeZ();

      double minimum = 1e38;
      double maximum = -1e38;

      int32_t x, slice;
      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( this->_mask( x, this->_referenceSlice, slice ) &&
               ( this->_weight( x, this->_referenceSlice, slice ) > 0.0 ) )
          {

            double value = ( double )this->_out( x, this->_referenceSlice,
                                                 slice, this->_time );
            if ( value < minimum )
            {

              minimum = value;

            } 
            else if ( value > maximum )
            {

              maximum = value;

            } 

          }

          if ( this->_mask( x, this->_biasedSlice, slice ) &&
               ( this->_weight( x, this->_biasedSlice, slice ) > 0.0 ) )
          {

            double value = this->_biasFunctor->getValueAt( 
                                       this->_out( x, this->_biasedSlice, slice,
                                                   this->_time ),
                                       parameter( 0 ) );
            if ( value < minimum )
            {

              minimum = value;

            } 
            else if ( value > maximum )
            {

              maximum = value;

            } 

          }

        }

      }

      // processing raw join probabilities
      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( x = 0; x < sizeX; x++ )
       {

          if ( this->_mask( x, this->_referenceSlice, slice ) &&
               ( this->_weight( x, this->_referenceSlice, slice ) > 0.0 ) )
          {

            double value = ( double )this->_out( x, this->_referenceSlice,
                                                 slice, this->_time );
            level = ( int32_t )( ( ( value - minimum ) /
                                   ( maximum - minimum ) ) *
                                 ( this->_levelCount - 1 ) );
            if ( level < 0 )
            {

               level = 0;

            }
            else if ( level >= this->_levelCount )
            {

               level = this->_levelCount - 1;

            }
            ++ probabilities( level, 0 );

          }

          if ( this->_mask( x, this->_biasedSlice, slice ) &&
               ( this->_weight( x, this->_biasedSlice, slice ) > 0.0 ) )
          {

            double value = this->_biasFunctor->getValueAt( 
                                       this->_out( x, this->_biasedSlice, slice,
                                                   this->_time ),
                                       parameter( 0 ) );
            level = ( int32_t )( ( ( value - minimum ) /
                                   ( maximum - minimum ) ) *
                                 ( this->_levelCount - 1 ) );
            if ( level < 0 )
            {

               level = 0;

            }
            else if ( level >= this->_levelCount )
            {

               level = this->_levelCount - 1;

            }
            ++ probabilities( level, 0 );

          }

        }

      }

    }
    else if ( this->_planeAxis == "z" )
    {

      int32_t sizeX = this->_in.getSizeX();
      int32_t lineCount = this->_in.getSizeY();

      double minimum = 1e38;
      double maximum = -1e38;

      int32_t x, line;
      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {


          if ( this->_mask( x, line, this->_biasedSlice ) &&
               ( this->_weight( x, line, this->_referenceSlice ) > 0.0 ) )
          {

            double value = ( double )this->_out( x, line, this->_referenceSlice,
                                                 this->_time );
            if ( value < minimum )
            {

              minimum = value;

            } 
            else if ( value > maximum )
            {

              maximum = value;

            } 

          }

          if ( this->_mask( x, line, this->_biasedSlice ) &&
               ( this->_weight( x, line, this->_biasedSlice ) > 0.0 ) )
          {

            double value = this->_biasFunctor->getValueAt( 
                         this->_out( x, line, this->_biasedSlice, this->_time ),
                         parameter( 0 ) );
            if ( value < minimum )
            {

              minimum = value;

            } 
            else if ( value > maximum )
            {

              maximum = value;

            } 

          }

        }

      }

      // processing raw join probabilities
      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( this->_mask( x, line, this->_referenceSlice ) &&
               ( this->_weight( x, line, this->_referenceSlice ) > 0.0 ) )
          {

            double value = ( double )this->_out( x, line, this->_referenceSlice,
                                                 this->_time );
            level = ( int32_t )( ( ( value - minimum ) /
                                   ( maximum - minimum ) ) *
                                 ( this->_levelCount - 1 ) );
            if ( level < 0 )
            {

               level = 0;

            }
            else if ( level >= this->_levelCount )
            {

               level = this->_levelCount - 1;

            }
            ++ probabilities( level, 0 );

          }

          if ( this->_mask( x, line, this->_biasedSlice ) &&
               ( this->_weight( x, line, this->_biasedSlice ) > 0.0 ) )
          {

            double value = this->_biasFunctor->getValueAt( 
                         this->_out( x, line, this->_biasedSlice, this->_time ),
                         parameter( 0 ) );
            level = ( int32_t )( ( ( value - minimum ) /
                                   ( maximum - minimum ) ) *
                                 ( this->_levelCount - 1 ) );
            if ( level < 0 )
            {

               level = 0;

            }
            else if ( level >= this->_levelCount )
            {

               level = this->_levelCount - 1;

            }
            ++ probabilities( level, 0 );

          }

        }

      }

    }

    // smoothing the join probabilities
    if ( _isSmoothingApplied )
    {

      gkg::Matrix smoothedProbabilities;

      gkg::DericheGaussianFilter3d< double >::getInstance().filter(
        probabilities,
        *this->_dericheGaussianCoefficients,
        gkg::DericheGaussianFilter3d< double >::X_AXIS,
        smoothedProbabilities );
      probabilities = smoothedProbabilities;

    }

    // processing the normalization factor
    double normalizationFactor = 0.0;
    for ( level = 0; level < this->_levelCount; level++ )
    {

      normalizationFactor += probabilities( level, 0 );

    }

    // normalizing the join probabilities
    if ( normalizationFactor > 0.0 )
    {

      for ( level = 0; level < this->_levelCount; level++ )
      {

        probabilities( level, 0 ) /= normalizationFactor;

      }

    }

    // processing the entropy
    double probability = 0.0;
    double entropy = 0.0;
    for ( level = 0; level < this->_levelCount; level++ )
    {

      probability = probabilities( level, 0 );
      if ( probability > 0.0 )
      {

        entropy -= probability * std::log( probability );

      }

    }

    return entropy;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double gkg::EntropyBiasFunction2d< T >::"
             "getValueAt( "
             "const gkg::Vector& parameter ) const" );

}


#undef GKG_ENTROPY_SMOOTHING_SIGMA


#endif

