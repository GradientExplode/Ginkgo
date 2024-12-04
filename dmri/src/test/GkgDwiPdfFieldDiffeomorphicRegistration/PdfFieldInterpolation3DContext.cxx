#include <gkg-core-exception/Exception.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
//#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include "ProbabilityDensityFunction.h"
#include "PdfFieldInterpolation3DContext.h"


gkg::PdfFieldInterpolation3DContext::PdfFieldInterpolation3DContext(
                   gkg::PdfCartesianField& warpedField,
                   gkg::PdfCartesianField& inputField,
                   std::set< gkg::Vector3d< int32_t >, 
                             gkg::Vector3dCompare< int32_t > >& hasSiteWarped,
                   std::set< gkg::Vector3d< int32_t >, 
                             gkg::Vector3dCompare< int32_t > >& hasSiteInput,
                   std::vector< gkg::Vector3d< int32_t > >& inputFieldSites,
                   gkg::Volume< float >& flowFieldX, 
                   gkg::Volume< float >& flowFieldY, 
                   gkg::Volume< float >& flowFieldZ, 
                   gkg::Vector3d< int32_t >& inputFieldSize,
                   bool& addIntegerCoordinates,
                   int32_t& index )
                    : gkg::LoopContext< int32_t >(),
                    _warpedField( warpedField ),
                    _inputField( inputField ),
                    _hasSiteWarped( hasSiteWarped ),
                    _hasSiteInput( hasSiteInput ),
                    _inputFieldSites( inputFieldSites ),
                    _flowFieldX( flowFieldX ),
                    _flowFieldY( flowFieldY ),
                    _flowFieldZ( flowFieldZ ),
                    _inputFieldSize( inputFieldSize ),
                    _addIntegerCoordinates( addIntegerCoordinates ),
                    _index( index )
{
}


void gkg::PdfFieldInterpolation3DContext::doIt( int32_t startIndex, 
                                                int32_t count )
{

  try
  {

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    int32_t inputSizeX = _inputFieldSize.x;
    int32_t inputSizeY = _inputFieldSize.y;
    int32_t inputSizeZ = _inputFieldSize.z;

    float flowFieldValueX = 0.0f;
    float flowFieldValueY = 0.0f; 
    float flowFieldValueZ = 0.0f; 

    int32_t floorFlowFieldValueX = 0.0f; 
    int32_t floorFlowFieldValueY = 0.0f; 
    int32_t floorFlowFieldValueZ = 0.0f;

    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;

    // getting the number of values of PDFs
    int32_t valueCount = _inputField.begin()->second.getValueCount();

    std::set< gkg::Vector3d< int32_t >, 
              gkg::Vector3dCompare< int32_t > >::iterator h1,h2;

    int32_t stopIndex = startIndex + count;

    int32_t o = 0;
    for ( o = startIndex; o < stopIndex; ++o  )
    {

      const gkg::Vector3d< int32_t >& site = _inputFieldSites[ o ];
      i = site.x;
      j = site.y;
      k = site.z;

      flowFieldValueX = _flowFieldX( i, j, k );
      flowFieldValueY = _flowFieldY( i, j, k );
      flowFieldValueZ = _flowFieldZ( i, j, k );

      floorFlowFieldValueX = std::floor( flowFieldValueX );
      floorFlowFieldValueY = std::floor( flowFieldValueY );
      floorFlowFieldValueZ = std::floor( flowFieldValueZ );
  
      dx = flowFieldValueX - floorFlowFieldValueX;
      dy = flowFieldValueY - floorFlowFieldValueY;
      dz = flowFieldValueZ - floorFlowFieldValueZ;
	
      if( _addIntegerCoordinates )
      {

        floorFlowFieldValueX += j;
        floorFlowFieldValueY += i;
        floorFlowFieldValueZ += k;

      }


      h1 = _hasSiteWarped.find( site );

      if ( h1 != _hasSiteWarped.end() )
      {

        gkg::ProbabilityDensityFunction inputPdf1;
        gkg::ProbabilityDensityFunction inputPdf2;
        gkg::ProbabilityDensityFunction inputPdf3;
        gkg::ProbabilityDensityFunction inputPdf4;
        gkg::ProbabilityDensityFunction inputPdf5;
        gkg::ProbabilityDensityFunction inputPdf6;
        gkg::ProbabilityDensityFunction inputPdf7;
        gkg::ProbabilityDensityFunction inputPdf8;

        gkg::ProbabilityDensityFunction& warpedPdf = 
                                         *( _warpedField.getItemFast( site ) );

        gkg::Vector3d< int32_t > interpolationCoordinates( 0, 0, 0 );

        bool isInputPdf1 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 );

        h2 = _hasSiteInput.find( interpolationCoordinates );

        if ( h2 != _hasSiteInput.end() )
        {

          inputPdf1 = *( _inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf1 = true;

        }


        bool isInputPdf2 = false;


        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY + 1, 0 ), 
                                                             inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 );


        h2 = _hasSiteInput.find( interpolationCoordinates );

        if ( h2 != _hasSiteInput.end() )
        {


          inputPdf2 = *( _inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf2 = true;

        }


        bool isInputPdf3 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX + 1, 0 ), 
                                                              inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 );

        h2 = _hasSiteInput.find( interpolationCoordinates );

        if ( h2 != _hasSiteInput.end() )
        {

          inputPdf3 = *( _inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf3 = true;

        }


        bool isInputPdf4 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                              inputSizeZ - 1 );

        h2 = _hasSiteInput.find( interpolationCoordinates );

        if ( h2 != _hasSiteInput.end() )
        {


          inputPdf4 = *( _inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf4 = true;

        }


        bool isInputPdf5 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY + 1, 0 ), 
                                                              inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX + 1, 0 ), 
                                                              inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 );

        h2 = _hasSiteInput.find( interpolationCoordinates );

        if ( h2 != _hasSiteInput.end() )
        {


          inputPdf5 = *( _inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf5 = true;

        }


        bool isInputPdf6 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY + 1, 0 ), 
                                                              inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                              inputSizeZ - 1 );
   
        h2 = _hasSiteInput.find( interpolationCoordinates );

        if ( h2 != _hasSiteInput.end() )
        {


          inputPdf6 = *( _inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf6 = true;

        }


        bool isInputPdf7 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX + 1, 0 ), 
                                                              inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                              inputSizeZ - 1 );

        h2 = _hasSiteInput.find( interpolationCoordinates );

        if ( h2 != _hasSiteInput.end() )
        {


          inputPdf7 = *( _inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf7 = true;

        }


        bool isInputPdf8 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY + 1, 0 ), 
                                                              inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX + 1, 0 ), 
                                                              inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                              inputSizeZ - 1 );

        h2 = _hasSiteInput.find( interpolationCoordinates );

        if ( h2 != _hasSiteInput.end() )
        {


          inputPdf8 = *( _inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf8 = true;

        }

        int32_t l = 0;
        float formerWeight = 0.0f;

        for ( l = _index; l < valueCount; ++l )
        {

          if ( ( _index == 1 ) && 
               ( l == 1 || l == 2 || l == 3 ) )
          {

            double value = 0.0;
            if ( isInputPdf1 )
            {
               
              if ( ( ( 1.0f - dx ) * ( 1.0f - dy ) * ( 1.0f - dz ) ) >= 
                   formerWeight )
              {

                value = inputPdf1.getValue( l ); 
                formerWeight = ( 1.0f - dx ) * ( 1.0f - dy ) * ( 1.0f - dz );

              }

             //std::cout << "rotation1 =" << inputPdf1.getValue(l) << std::endl;

            }
            if ( isInputPdf2 )
            {

              if ( ( ( 1.0f - dx ) * dy * ( 1.0f - dz ) ) >= 
                   formerWeight )
              {

                value = inputPdf2.getValue( l ); 
                formerWeight = ( 1.0f - dx ) * dy * ( 1.0f - dz );

              }

             //std::cout << "rotation2 =" << inputPdf2.getValue(l) << std::endl;
 
            }
            if ( isInputPdf3 )
            {

              if ( ( dx * ( 1.0f - dy ) * ( 1.0f - dz ) ) >= 
                   formerWeight )
              {

                value = inputPdf3.getValue( l ); 
                formerWeight = dx * ( 1.0f - dy ) * ( 1.0f - dz );

              }

             //std::cout << "rotation3 =" << inputPdf3.getValue(l) << std::endl;


            }
            if ( isInputPdf4 )
            {

              if ( ( ( 1.0f - dx ) * ( 1.0f - dy ) * dz ) >= 
                   formerWeight )
              {

                value = inputPdf4.getValue( l ); 
                formerWeight = ( 1.0f - dx ) * ( 1.0f - dy ) * dz;

              }

             //std::cout << "rotation4 =" << inputPdf4.getValue(l) << std::endl;

            }
            if ( isInputPdf5 )
            {

              if ( ( dx * dy * ( 1.0f - dz ) ) >= 
                   formerWeight )
              {

                value = inputPdf5.getValue( l ); 
                formerWeight = dx * dy * ( 1.0f - dz );

              } 

             //std::cout << "rotation5 =" << inputPdf5.getValue(l) << std::endl;

            }
            if ( isInputPdf6 )
            {

              if ( ( ( 1.0f - dx ) * dy * dz ) >= 
                   formerWeight )
              {

                value = inputPdf6.getValue( l ); 
                formerWeight = ( 1.0f - dx ) * dy * dz;

              }

             //std::cout << "rotation6 =" << inputPdf6.getValue(l) << std::endl;

            }
            if ( isInputPdf7 )
            {

              if ( ( dx * ( 1.0f - dy ) * dz ) >= 
                   formerWeight )
              {

                value = inputPdf7.getValue( l ); 
                formerWeight = dx * ( 1.0f - dy ) * dz;

              } 

             //std::cout << "rotation7 =" << inputPdf7.getValue(l) << std::endl;

            }
            if ( isInputPdf8 )
            {

              if ( ( dx * dy * dz ) >= 
                   formerWeight )
              {

                value = inputPdf8.getValue( l ); 
                formerWeight = dx * dy * dz;

              }

             //std::cout << "rotation8 =" << inputPdf8.getValue(l) << std::endl;

            }

            //std::cout << std::endl;
            warpedPdf.setValue( l, value );

          }

          else
          {

            double value = 0.0;
            if ( isInputPdf1 )
            {

              value += ( 1.0f - dx ) * ( 1.0f - dy ) * ( 1.0f - dz ) *
                       inputPdf1.getValue( l ); 

            }
            if ( isInputPdf2 )
            {

              value += ( 1.0f - dx ) * dy * ( 1.0f - dz ) *
                       inputPdf2.getValue( l ); 

            }
            if ( isInputPdf3 )
            {

              value += dx * ( 1.0f - dy ) * ( 1.0f - dz ) *
                       inputPdf3.getValue( l ); 

            }
            if ( isInputPdf4 )
            {

              value += ( 1.0f - dx ) * ( 1.0f - dy ) * dz *
                       inputPdf4.getValue( l ); 

            }
            if ( isInputPdf5 )
            {

              value += dx * dy * ( 1.0f - dz ) *
                       inputPdf5.getValue( l ); 

            }
            if ( isInputPdf6 )
            {

              value += ( 1.0f - dx ) * dy * dz *
                       inputPdf6.getValue( l ); 

            }
            if ( isInputPdf7 )
            {

              value += dx * ( 1.0f - dy ) * dz *
                       inputPdf7.getValue( l ); 

            }
            if ( isInputPdf8 )
            {

              value += dx * dy * dz *
                       inputPdf8.getValue( l ); 

            }

            warpedPdf.setValue( l, value );

          }

        }

      }  

    }

  }
  GKG_CATCH( "void gkg::PdfFieldInterpolation3DContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}





