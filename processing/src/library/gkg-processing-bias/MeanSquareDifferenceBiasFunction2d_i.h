#ifndef _gkg_processing_bias_MeanSquareDifferenceBiasFunction2d_i_h_
#define _gkg_processing_bias_MeanSquareDifferenceBiasFunction2d_i_h_


#include <gkg-processing-bias/MeanSquareDifferenceBiasFunction2d.h>
#include <gkg-processing-bias/BiasNelderMeadFunction2d_i.h>


template < class T >
inline
gkg::MeanSquareDifferenceBiasFunction2d< T >::
                                             MeanSquareDifferenceBiasFunction2d(
                            gkg::RCPointer< gkg::BiasFunctor< T > > biasFunctor,
                            const gkg::Volume< T >& in,
                            const gkg::Volume< int16_t >& mask,
                            const gkg::Volume< float >& weight,
                            gkg::Volume< T >& out,
                            gkg::Volume< float >& bias,
                            const std::string& planeAxis )
                                 : gkg::BiasNelderMeadFunction2d< T >( 
                                                                    biasFunctor,
                                                                    in,
                                                                    mask,
                                                                    weight,
                                                                    out,
                                                                    bias,
                                                                    planeAxis )
{
}


template < class T >
inline
gkg::MeanSquareDifferenceBiasFunction2d< T >::
                                           ~MeanSquareDifferenceBiasFunction2d()
{
}


template < class T >
inline
double gkg::MeanSquareDifferenceBiasFunction2d< T >::getValueAt(
                                            const gkg::Vector& parameter ) const
{

  try
  {

    double distance = 0.0;

    if ( this->_planeAxis == "x" )
    {

      int32_t lineCount = this->_in.getSizeY();
      int32_t sliceCount = this->_in.getSizeZ();

      double difference = 0.0;

      int32_t line, slice;
      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( line = 0; line < lineCount; line++ )
        {

          if ( this->_mask( this->_referenceSlice, line, slice ) &&
               this->_mask( this->_biasedSlice, line, slice ) )
          {

            difference = ( ( double )this->_out( this->_referenceSlice, line,
                                                 slice, this->_time ) -
                           this->_biasFunctor->getValueAt( 
                                    this->_out( this->_biasedSlice, line, slice,
                                                this->_time ),
                                    parameter( 0 ) ) );
            distance += difference * difference * 
                        std::min( this->_weight( this->_referenceSlice, line,
                                                 slice ),
                                  this->_weight( this->_biasedSlice, line,
                                                 slice ) );

          }

        }

      }

    }
    else if ( this->_planeAxis == "y" )
    {

      int32_t sizeX = this->_in.getSizeX();
      int32_t sliceCount = this->_in.getSizeZ();

      double difference = 0.0;

      int32_t x, slice;
      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( this->_mask( x, this->_referenceSlice, slice ) &&
               this->_mask( x, this->_biasedSlice, slice ) )
          {

            difference = ( ( double )this->_out( x, this->_referenceSlice,
                                                 slice, this->_time ) - 
                           this->_biasFunctor->getValueAt( 
                                     this->_out( x, this->_biasedSlice, slice,
                                                 this->_time ),
                                     parameter( 0 ) ) );
            distance += difference * difference * 
                        std::min( this->_weight( x, this->_referenceSlice,
                                                 slice ),
                                  this->_weight( x, this->_biasedSlice,
                                                 slice ) );

          }

        }

      }

    }
    else if ( this->_planeAxis == "z" )
    {

      int32_t sizeX = this->_in.getSizeX();
      int32_t lineCount = this->_in.getSizeY();

      double difference = 0.0;

      int32_t x, line;
      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( this->_mask( x, line, this->_referenceSlice ) &&
               this->_mask( x, line, this->_biasedSlice ) )
          {

            difference = ( ( double )this->_out( x, line, this->_referenceSlice,
                                                 this->_time ) - 
                           this->_biasFunctor->getValueAt( 
                                       this->_out( x, line, this->_biasedSlice,
                                                   this->_time ),
                                       parameter( 0 ) ) );
            distance += difference * difference * 
                      std::min( this->_weight( x, line, this->_referenceSlice ),
                                this->_weight( x, line, this->_biasedSlice ) );

          }

        }

      }

   }

    return distance;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double gkg::MeanSquareDifferenceBiasFunction2d< T >::"
             "getValueAt( "
             "const gkg::Vector& parameter ) const" );

}


#endif

