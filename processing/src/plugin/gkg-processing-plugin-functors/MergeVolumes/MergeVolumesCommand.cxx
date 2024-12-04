#include <gkg-processing-plugin-functors/MergeVolumes/MergeVolumesCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-resampling/Resampler.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-transform/ParameterizedAffineTransform3d.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <climits>
#include <cfloat>


//
//   MergeVolumesProcess
//


class MergeVolumesProcess : public gkg::Process
{

  public:

    MergeVolumesProcess( const std::string& theFileNameFloating,
                  	 const std::string& theFileNameTransformation,
                  	 const std::string& theFileNameOut,
                  	 const int32_t& theResamplingOrder,
                  	 const std::string& theFusionMethod,
                  	 const bool& theForceReferenceSize,
                  	 const bool& theVerbose );

    const std::string& fileNameFloating;
    const std::string& fileNameTransformation;
    const std::string& fileNameOut;
    const int32_t& resamplingOrder;
    const std::string& fusionMethod;
    const bool& forceReferenceSize;
    const bool& verbose;

  private:

    template < class T >
    static void merge( gkg::Process& process,
                       const std::string& fileNameReference,
                       const gkg::AnalyzedObject&,
                       const std::string& );

};


MergeVolumesProcess::MergeVolumesProcess(
                            	   const std::string& theFileNameFloating,
                            	   const std::string& theFileNameTransformation,
                            	   const std::string& theFileNameOut,
                            	   const int32_t& theResamplingOrder,
                            	   const std::string& theFusionMethod,
                            	   const bool& theForceReferenceSize,
                            	   const bool& theVerbose )
                   : fileNameFloating( theFileNameFloating ),
                     fileNameTransformation( theFileNameTransformation ),
                     fileNameOut( theFileNameOut ),
                     resamplingOrder( theResamplingOrder ),
                     fusionMethod( theFusionMethod ),
                     forceReferenceSize( theForceReferenceSize ),
                     verbose( theVerbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &MergeVolumesProcess::merge< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &MergeVolumesProcess::merge< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &MergeVolumesProcess::merge< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &MergeVolumesProcess::merge< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &MergeVolumesProcess::merge< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &MergeVolumesProcess::merge< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &MergeVolumesProcess::merge< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &MergeVolumesProcess::merge< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &MergeVolumesProcess::merge< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &MergeVolumesProcess::merge< double > );

}


template < class T >
void MergeVolumesProcess::merge( gkg::Process& process,
                                 const std::string& fileNameReference,
                                 const gkg::AnalyzedObject&,
                                 const std::string& )
{

  try
  {

    MergeVolumesProcess&
      mergeProcess = static_cast< MergeVolumesProcess& >( process );

    const std::string& fileNameFloating = mergeProcess.fileNameFloating;
    const std::string& fileNameTransformation =
                                            mergeProcess.fileNameTransformation;
    const std::string& fileNameOut = mergeProcess.fileNameOut;
    const int32_t& resamplingOrder = mergeProcess.resamplingOrder;
    const std::string& fusionMethod = mergeProcess.fusionMethod;
    const bool& forceReferenceSize = mergeProcess.forceReferenceSize;
    const bool& verbose = mergeProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // reading reference volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameReference << "' : " << std::flush;

    }
    gkg::Volume< T > reference;
    gkg::Reader::getInstance().read( fileNameReference, reference );
    const int32_t referenceSizeX = reference.getSizeX();
    const int32_t referenceSizeY = reference.getSizeY();
    const int32_t referenceSizeZ = reference.getSizeZ();
    const int32_t referenceSizeT = reference.getSizeT();
    gkg::Vector3d< double > referenceResolution;

    reference.getResolution( referenceResolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading floating volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameFloating << "' : " << std::flush;

    }
    gkg::Volume< T > floating;
    gkg::Reader::getInstance().read( fileNameFloating, floating );
    const int32_t floatingSizeX = floating.getSizeX();
    const int32_t floatingSizeY = floating.getSizeY();
    const int32_t floatingSizeZ = floating.getSizeZ();
    const int32_t floatingSizeT = floating.getSizeT();
    gkg::Vector3d< double > floatingResolution;

    floating.getResolution( floatingResolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // sanity check
    ////////////////////////////////////////////////////////////////////////////
    if ( referenceSizeT != 1 )
    {

      gkg::Volume< T > copyReference;

      copyReference = reference;
      gkg::Vector3d< double > savedResolution( 1.0, 1.0, 1.0 );
      reference.getResolution( savedResolution );

      int32_t sizeX = reference.getSizeX();
      int32_t sizeY = reference.getSizeY();
      int32_t sizeZ = reference.getSizeZ();

      reference.reallocate( sizeX, sizeY, sizeZ, 1 );
      reference.setResolution( savedResolution );

      if ( reference.getHeader().hasAttribute( "transformations" ) )
      {

        reference.getHeader()[ "transformations" ] = \
                                 copyReference.getHeader()[ "transformations" ];

      }

      // copying volume
      typename gkg::Volume< T >::iterator i1  = reference.begin(),
                                          i1e = reference.begin() +
                                                sizeX * sizeY * sizeZ;
      typename gkg::Volume< T >::iterator i2  = copyReference.begin();

      while ( i1 != i1e )
      {

        *i2 = *i1;
        ++ i1;
        ++ i2;

      }

    }

    if ( floatingSizeT != 1 )
    {

      gkg::Volume< T > copyFloating;
      copyFloating = floating;
      gkg::Vector3d< double > savedResolution( 1.0, 1.0, 1.0 );
      floating.getResolution( savedResolution );

      int32_t sizeX = floating.getSizeX();
      int32_t sizeY = floating.getSizeY();
      int32_t sizeZ = floating.getSizeZ();

      floating.reallocate( sizeX, sizeY, sizeZ, 1 );
      floating.setResolution( savedResolution );
      if ( floating.getHeader().hasAttribute( "transformations" ) )
      {

        floating.getHeader()[ "transformations" ] = \
                                  copyFloating.getHeader()[ "transformations" ];

      }

      // copying volume
      typename gkg::Volume< T >::iterator i1  = floating.begin(),
                                          i1e = floating.begin() +
                                                sizeX * sizeY * sizeZ;
      typename gkg::Volume< T >::iterator i2  = copyFloating.begin();

      while ( i1 != i1e )
      {

        *i2 = *i1;
        ++ i1;
        ++ i2;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // Test if transformation should be used from header
    ////////////////////////////////////////////////////////////////////////////

    gkg::HomogeneousTransform3d< double > transform3d;
    int32_t referenceIndex = 0;

    if ( fileNameTransformation.empty() )
    {

      // Try to find transformation in header
      if ( reference.getHeader().hasAttribute( "transformations" ) &&
           floating.getHeader().hasAttribute( "transformations" ) )
      {


        if ( verbose )
        {

          std::cout << "trying to read transformation information"
                    << std::endl;

        }

        // If several referentials exist, take information from the
        // shared referential
        if ( reference.getHeader().hasAttribute( "referentials" ) &&
             floating.getHeader().hasAttribute( "referentials" ) )
        {

          std::vector< std::string > referenceNames;
          std::vector< std::string > floatingNames;

          reference.getHeader().getAttribute( "referentials", referenceNames );
          floating.getHeader().getAttribute( "referentials", floatingNames );

          bool found = false;
          int32_t rIndex = 0;
          std::vector< std::string >::iterator
            r = referenceNames.begin(),
            re = referenceNames.end();

          while ( !found && ( r != re ) )
          {

            std::vector< std::string >::iterator
              f = floatingNames.begin(),
              fe = floatingNames.end();

            while ( !found && ( f != fe ) )
            {

              if ( *f == *r )
              {

                found = true;
                referenceIndex = rIndex;

              }

              ++f;

            }

            rIndex++;
            ++r;

          }

        }

        // Get homogeneous transform matrix of reference volume
        gkg::GenericObjectList transformations;
        std::vector< double > referenceDirectCoefficients( 16 );

        try
        {

          reference.getHeader().getAttribute( "transformations",
                                               transformations );
          gkg::GenericObjectList transformation =
            transformations[ referenceIndex
                                        ]->getValue< gkg::GenericObjectList >();

          for ( int32_t i = 0; i < 16; ++ i )
          {

            referenceDirectCoefficients[ i ] = transformation[ i ]->getScalar();

          }

        }
        catch( std::exception& )
        {

          if ( verbose )
          {

            std::cerr << "failed to read attribute transformations"
                      << std::endl;

          }

          referenceDirectCoefficients[ 0 ] = 1.0;
          referenceDirectCoefficients[ 5 ] = 1.0;
          referenceDirectCoefficients[ 10 ] = 1.0;
          referenceDirectCoefficients[ 15 ] = 1.0;

        }

        if ( verbose )
        {

          std::cout << "reference matrix found:" << std::endl;
          for ( int32_t i = 0; i < 16; ++ i )
          {

            if ( i % 4 == 0 )
            {

              std::cout << std::endl;

            }

            std::cout << referenceDirectCoefficients[ i ] << " " << std::flush;

          }

          std::cout << std::endl;

        }


        // Get homogeneous transform matrix of floating volume
        std::vector< double > floatingDirectCoefficients( 16 );

        try
        {

          floating.getHeader().getAttribute( "transformations",
                                              transformations );
          gkg::GenericObjectList transformation =
            transformations[ referenceIndex
                                        ]->getValue< gkg::GenericObjectList >();

          for ( int32_t i = 0; i < 16; ++i )
          {

            floatingDirectCoefficients[ i ] = transformation[ i ]->getScalar();

          }

        }
        catch( std::exception& )
        {

          if ( verbose )
          {

            std::cerr << "failed to read attribute transformations"
                      << std::endl;

          }

          floatingDirectCoefficients[ 0 ] = 1.0;
          floatingDirectCoefficients[ 5 ] = 1.0;
          floatingDirectCoefficients[ 10 ] = 1.0;
          floatingDirectCoefficients[ 15 ] = 1.0;

        }

        if ( verbose )
        {

          std::cout << "floating matrix found:" << std::endl;
          for ( int32_t i = 0; i < 16; ++i )
          {

            if ( i % 4 == 0 )
            {

              std::cout << std::endl;

            }

            std::cout << floatingDirectCoefficients[ i ] << " " << std::flush;

          }

          std::cout << std::endl;

        }

        bool direct = true;
        bool indirect = false;

        gkg::HomogeneousTransform3d< double > transform3dFloatToMriReferential(
                                         floatingDirectCoefficients, indirect );

        gkg::HomogeneousTransform3d< double > transform3dMriReferentialToRef(
                                          referenceDirectCoefficients, direct );

        gkg::CompositeTransform3d< double > compositeTransform3d;

        compositeTransform3d.compose( transform3dFloatToMriReferential );
        compositeTransform3d.compose( transform3dMriReferentialToRef );

        transform3d = compositeTransform3d.getComposition();

      }
      else
      {

        throw std::runtime_error( "no transformation found in both headers" );

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading transform
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameTransformation.empty() )
    {

      if ( verbose )
      {

        std::cout << "reading transformation : " << std::flush;

      }

      std::ifstream is( fileNameTransformation.c_str() );

      if ( !is.good() )
      {

        throw std::runtime_error( "bad transformation filename" );

      }

      // Read transform matrix
      transform3d.readTrm( is );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    if ( verbose || fileNameOut.empty() )
    {

      // Display transformation between the 2 volumes

      std::cout << transform3d.getDirectCoefficient( 0, 3 ) << " "
                << transform3d.getDirectCoefficient( 1, 3 ) << " "
                << transform3d.getDirectCoefficient( 2, 3 ) << std::endl;
      std::cout << transform3d.getDirectCoefficient( 0, 0 ) << " "
                << transform3d.getDirectCoefficient( 0, 1 ) << " "
                << transform3d.getDirectCoefficient( 0, 2 ) << std::endl;
      std::cout << transform3d.getDirectCoefficient( 1, 0 ) << " "
                << transform3d.getDirectCoefficient( 1, 1 ) << " "
                << transform3d.getDirectCoefficient( 1, 2 ) << std::endl;
      std::cout << transform3d.getDirectCoefficient( 2, 0 ) << " "
                << transform3d.getDirectCoefficient( 2, 1 ) << " "
                << transform3d.getDirectCoefficient( 2, 2 ) << std::endl;
      std:: cout << "matrix above is the transformation between the 2 volumes"
                 << std::endl;

    }

    if ( fileNameOut.empty() )
    {

      // Stop here : only transformation info found are displayed
      std::cout << "stop here : only information found are displayed "
                << "if no output file is defined" << std::endl;
      return;

    }

    ////////////////////////////////////////////////////////////////////////////
    // Find size of result volume, also compute shift information
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int16_t > resultVolumeSize;
    gkg::Vector3d< int16_t > shiftReferenceInResultVolumeInVoxel;
    gkg::Vector3d< int16_t > shiftFloatingInResultVolumeInVoxel;


    if ( forceReferenceSize )
    {

      // Output volume size is set to reference volume size
      resultVolumeSize.x = reference.getSizeX();
      resultVolumeSize.y = reference.getSizeY();
      resultVolumeSize.z = reference.getSizeZ();

    }
    else
    {

      // Compute position of floating (shift) according to reference volume
      gkg::Vector3d< double > minSizeInMM( FLT_MAX, FLT_MAX, FLT_MAX );
      gkg::Vector3d< double > maxSizeInMM( -FLT_MAX, -FLT_MAX, -FLT_MAX );

      gkg::Vector3d< double > from;
      gkg::Vector3d< double > to;

      {

        int32_t x, y, z;

        // loop over all points of floating volume to find limits
        // (can be done faster testing only corners...)
        for ( z = 0, from.z = 0; z < floatingSizeZ; ++ z,
                                     from.z += floatingResolution.z )
        {

          for ( y = 0, from.y = 0; y < floatingSizeY; ++ y,
                                       from.y += floatingResolution.y )
          {

            for ( x = 0, from.x = 0; x < floatingSizeX; ++ x,
                                         from.x += floatingResolution.x )
            {

              transform3d.getDirect( from, to );

              // Test limits
              minSizeInMM.x = ( to.x < minSizeInMM.x ) ? to.x : minSizeInMM.x;
              minSizeInMM.y = ( to.y < minSizeInMM.y ) ? to.y : minSizeInMM.y;
              minSizeInMM.z = ( to.z < minSizeInMM.z ) ? to.z : minSizeInMM.z;

              maxSizeInMM.x = ( to.x > maxSizeInMM.x ) ? to.x : maxSizeInMM.x;
              maxSizeInMM.y = ( to.y > maxSizeInMM.y ) ? to.y : maxSizeInMM.y;
              maxSizeInMM.z = ( to.z > maxSizeInMM.z ) ? to.z : maxSizeInMM.z;

            }

          }

        }

      }

      if ( verbose )
      {

        std::cout << "minimum size in mm : " << minSizeInMM << std::endl;
        std::cout << "maximum size in mm : " << maxSizeInMM << std::endl;

      }

      gkg::Vector3d< int16_t > minSizeInVoxel, maxSizeInVoxel;

      minSizeInVoxel.x = (int32_t)( minSizeInMM.x / floatingResolution.x );
      maxSizeInVoxel.x = (int32_t)( maxSizeInMM.x / floatingResolution.x + 
                                    0.5 );

      minSizeInVoxel.y = (int32_t)( minSizeInMM.y / floatingResolution.y );
      maxSizeInVoxel.y = (int32_t)( maxSizeInMM.y / floatingResolution.y +
                                    0.5 );

      minSizeInVoxel.z = (int32_t)( minSizeInMM.z / floatingResolution.z );
      maxSizeInVoxel.z = (int32_t)( maxSizeInMM.z / floatingResolution.z + 
                                    0.5 );

      if ( verbose )
      {

        std::cout << "minimum size in voxel : " << minSizeInVoxel << std::endl;
        std::cout << "maximum size in voxel : " << maxSizeInVoxel << std::endl;

      }

      // Convert position unit : voxels into mm

      gkg::Vector3d< double > minResVolumeSizeInMM, maxResVolumeSizeInMM;

      minResVolumeSizeInMM.x = std::min( minSizeInMM.x, (double)0 );
      maxResVolumeSizeInMM.x = std::max( maxSizeInMM.x,
                                         (double)( reference.getSizeX() - 1 ) *
                                         (double)referenceResolution.x );

      minResVolumeSizeInMM.y = std::min( minSizeInMM.y, (double)0 );
      maxResVolumeSizeInMM.y = std::max( maxSizeInMM.y,
                                         (double)( reference.getSizeY() - 1 ) *
                                         (double)referenceResolution.y );

      minResVolumeSizeInMM.z = std::min( minSizeInMM.z, (double)0 );
      maxResVolumeSizeInMM.z = std::max( maxSizeInMM.z,
                                         (double)( reference.getSizeZ() - 1 ) *
                                         (double)referenceResolution.z );

      if ( verbose )
      {

        std::cout << "minimum resulting volume size in mm : "
                  << minResVolumeSizeInMM << std::endl;
        std::cout << "maximum resulting volume size in mm : "
                  << maxResVolumeSizeInMM << std::endl;

      }

      gkg::Vector3d< int32_t > minResVolumeSizeInVoxel, maxResVolumeSizeInVoxel;

      maxResVolumeSizeInVoxel.x = (int32_t)( maxResVolumeSizeInMM.x /
                                             referenceResolution.x + 0.5 );
      minResVolumeSizeInVoxel.x = (int32_t)( minResVolumeSizeInMM.x /
                                             referenceResolution.x );
      maxResVolumeSizeInVoxel.y = (int32_t)( maxResVolumeSizeInMM.y /
                                             referenceResolution.y + 0.5 );
      minResVolumeSizeInVoxel.y = (int32_t)( minResVolumeSizeInMM.y /
                                             referenceResolution.y );
      maxResVolumeSizeInVoxel.z = (int32_t)( maxResVolumeSizeInMM.z /
                                             referenceResolution.z + 0.5 );
      minResVolumeSizeInVoxel.z = (int32_t)( minResVolumeSizeInMM.z /
                                             referenceResolution.z );

      // Compute res volume size
      resultVolumeSize.x = maxResVolumeSizeInVoxel.x -
                        minResVolumeSizeInVoxel.x + 1;
      resultVolumeSize.y = maxResVolumeSizeInVoxel.y -
                        minResVolumeSizeInVoxel.y + 1;
      resultVolumeSize.z = maxResVolumeSizeInVoxel.z -
                        minResVolumeSizeInVoxel.z + 1;

      shiftReferenceInResultVolumeInVoxel.x = ( minSizeInMM.x < 0 ) ?
                   (int16_t)( -minSizeInMM.x / floatingResolution.x + 0.5 ) : 0;
      shiftReferenceInResultVolumeInVoxel.y = ( minSizeInMM.y < 0 ) ?
                   (int16_t)( -minSizeInMM.y / floatingResolution.y + 0.5 ) : 0;
      shiftReferenceInResultVolumeInVoxel.z = ( minSizeInMM.z < 0 ) ?
                   (int16_t)( -minSizeInMM.z / floatingResolution.z + 0.5 ) : 0;

      shiftFloatingInResultVolumeInVoxel.x = ( minSizeInMM.x < 0 ) ?
                                               minSizeInVoxel.x : 0;
      shiftFloatingInResultVolumeInVoxel.y = ( minSizeInMM.y < 0 ) ?
                                               minSizeInVoxel.y : 0;
      shiftFloatingInResultVolumeInVoxel.z = ( minSizeInMM.z < 0 ) ?
                                               minSizeInVoxel.z : 0;

    }

    if ( verbose )
    {

      std::cout << "shift of reference from resulting volume in voxels : "
                << shiftReferenceInResultVolumeInVoxel << std::endl;
      std::cout << "shift of floating from resulting volume in voxels : "
                << shiftFloatingInResultVolumeInVoxel << std::endl;

    }


    if ( verbose )
    {

      std::cout << "resulting volume size : " << resultVolumeSize << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // Create result volume
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< T > resultVolume( resultVolumeSize.x,
                                   resultVolumeSize.y,
                                   resultVolumeSize.z );

    gkg::Vector3d< double > resultResolution = referenceResolution;
    resultVolume.setResolution( resultResolution );


    gkg::BoundingBox< int32_t > bBox( 0, resultVolume.getSizeX() - 1,
                                      0, resultVolume.getSizeY() - 1,
                                      0, resultVolume.getSizeZ() - 1 );

    // Copy reference in result volume
    for ( int32_t z = 0; z < referenceSizeZ; ++ z )
    {

      for ( int32_t y = 0; y < referenceSizeY; ++ y )
      {

        for ( int32_t x = 0; x < referenceSizeX; ++ x )
        {

          gkg::Vector3d< int32_t >
            site( x + shiftReferenceInResultVolumeInVoxel.x,
                  y + shiftReferenceInResultVolumeInVoxel.y,
                  z + shiftReferenceInResultVolumeInVoxel.z );
          if ( bBox.contains( site ) )
          {

            resultVolume( site ) = reference( x, y, z );

          }

        }

      }

    }

    // Sanity check :
    if ( resamplingOrder != -1 && resamplingOrder != 0 && resamplingOrder != 1 )
    {

      throw std::runtime_error( "bad resampling order : expecting 0 or 1" );

    }


    gkg::Resampler< T >* resampler = 0;

    if ( resamplingOrder != -1 )
    {

      resampler = gkg::ResamplerFactory< T >::getInstance().getResampler(
                                                              resamplingOrder );

    }

    gkg::HomogeneousTransform3d< double > identityTransform3d;

    gkg::PartialVolumingQuickResampler< T >& quickResampler =
       gkg::PartialVolumingQuickResampler< T >::getInstance();

    gkg::Vector3d< int32_t > siteRes;
    gkg::Vector3d< double > siteResInMM;
    gkg::Vector3d< double > siteReferenceInMM;
    gkg::Vector3d< int32_t > siteFloating;
    gkg::Vector3d< double > siteFloatingInMM;


    // Fill result volume
    for ( siteRes.z = 0; siteRes.z < resultVolumeSize.z; ++ siteRes.z )
    {

      if ( verbose )
      {

        if ( siteRes.z != 0 )
        {

          std::cout << gkg::Eraser( 17 );

        }
        std::cout << "[ " << std::setw( 5 ) << siteRes.z
                  << " / " << std::setw( 5 ) << resultVolumeSize.z - 1
                  << " ]" << std::flush;

      }

      for ( siteRes.y = 0; siteRes.y < resultVolumeSize.y; ++ siteRes.y )
      {

        for ( siteRes.x = 0; siteRes.x < resultVolumeSize.x; ++ siteRes.x )
        {

          siteResInMM.x = ( siteRes.x + shiftFloatingInResultVolumeInVoxel.x ) *
                          resultResolution.x;
          siteResInMM.y = ( siteRes.y + shiftFloatingInResultVolumeInVoxel.y ) *
                          resultResolution.y;
          siteResInMM.z = ( siteRes.z + shiftFloatingInResultVolumeInVoxel.z ) *
                          resultResolution.z;

          transform3d.getInverse( siteResInMM, siteFloatingInMM );

          T floatingValueInterpolated;

          if ( resampler )
          {

            // use resampler created with specific order
            resampler->resample( floating,
                                 identityTransform3d,
                                 ( T )(-1),
                                 siteFloatingInMM,
                                 floatingValueInterpolated,
                                 &floatingResolution );

          }
          else
          {

            // use quick resampler
            quickResampler.resample( floating,
                                     ( T )(-1),
                                     siteFloatingInMM,
                                     floatingValueInterpolated,
                                     &floatingResolution,
                                     0 );

          }

          T currentValueResultVolume = resultVolume( siteRes );

          if ( ( currentValueResultVolume != 0 ) &&
               ( floatingValueInterpolated != T( -1 ) ) )
          {

            // Floating and reference mask overlap

            if ( fusionMethod == "average" )
            {

              resultVolume( siteRes ) = ( T )( ( resultVolume( siteRes ) +
                                                 floatingValueInterpolated ) /
                                               2.0 );

            }
            else if ( fusionMethod == "max" )
            {

              resultVolume( siteRes ) = ( T )std::max(
                                                    resultVolume( siteRes ),
                                                    floatingValueInterpolated );

            }
            else if ( fusionMethod == "add" )
            {

              resultVolume( siteRes ) = resultVolume( siteRes ) +
                                        floatingValueInterpolated;

            }
            else if ( fusionMethod == "none" )
            {

              // Set fusion to 0 if both ref and float value is not null
              if ( ( floatingValueInterpolated != 0 ) &&
                   ( currentValueResultVolume != 0 ) )
              {

                resultVolume( siteRes ) = 0;

              }
              else
              {

                resultVolume( siteRes ) = currentValueResultVolume;

              }

            }
            else
            {

              // Nothing to do

            }

          }
          else if ( floatingValueInterpolated != T( -1 ) )
          {

            // Only floating
            resultVolume( siteRes ) = floatingValueInterpolated;

          }
          else
          {

            // Only Reference (already copied):  nothing to do

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }


    //////////////////////////////////////////////////////////////////////////
    // computing shift of reference transformation
    //////////////////////////////////////////////////////////////////////////
    gkg::Vector3d< double > shiftRefInResVolumeInMM;
    shiftRefInResVolumeInMM.x = shiftReferenceInResultVolumeInVoxel.x *
                                referenceResolution.x;
    shiftRefInResVolumeInMM.y = shiftReferenceInResultVolumeInVoxel.y *
                                referenceResolution.y;
    shiftRefInResVolumeInMM.z = shiftReferenceInResultVolumeInVoxel.z *
                                referenceResolution.z;

    if ( verbose )
    {

      std::cout << "shift of reference to resulting volume in mm : "
                << shiftRefInResVolumeInMM << std::endl;

    }

    std::ofstream os( ( fileNameOut + "_shift.trm" ).c_str() );
    os << -shiftRefInResVolumeInMM.x << " "
       << -shiftRefInResVolumeInMM.y << " "
       << -shiftRefInResVolumeInMM.z << " " << std::endl
       << "1 0 0" << std::endl
       << "0 1 0" << std::endl
       << "0 0 1" << std::endl;
    os.close();


    //////////////////////////////////////////////////////////////////////////
    // Try to compute transformation info for res volume
    // (used for dicom for example)
    //////////////////////////////////////////////////////////////////////////

    // Try to get transformation from reference

    if ( reference.getHeader().hasAttribute( "transformations" ) )
    {

      // Get homogeneous transform matrix of reference volume
      gkg::GenericObjectList transformations;
      std::vector< double > refTransformation( 16 );

      try
      {

        reference.getHeader().getAttribute( "transformations",
                                             transformations );
        gkg::GenericObjectList transformation =
          transformations[ referenceIndex
                                        ]->getValue< gkg::GenericObjectList >();

        for ( int32_t i = 0; i < 16; ++ i )
        {

          refTransformation[ i ] = transformation[ i ]->getScalar();

        }

      }
      catch( std::exception& )
      {

        if ( verbose )
        {

          std::cerr << "failed to read attribute transformations" << std::endl;

        }

        refTransformation.clear();

      }


      // Compute new transformation if enough info has been retrieved
      if ( refTransformation.size() == 16 )
      {

        std::vector< double > resTransformation;

        // compute new translation
        gkg::Vector3d< double > newTranslation;

        gkg::Vector3d< double > refTranslation;
        refTranslation.x = refTransformation[ 3 ];
        refTranslation.y = refTransformation[ 7 ];
        refTranslation.z = refTransformation[ 11 ];


        if ( verbose )
        {

          std::cout << "reference translation : "
                    << refTranslation << std::endl;

        }


        newTranslation = refTranslation + shiftRefInResVolumeInMM;

        // Rotation is the same as reference rotation
        resTransformation.push_back( refTransformation[ 0 ] );
        resTransformation.push_back( refTransformation[ 1 ] );
        resTransformation.push_back( refTransformation[ 2 ] );
        resTransformation.push_back( newTranslation.x );
        resTransformation.push_back( refTransformation[ 4 ] );
        resTransformation.push_back( refTransformation[ 5 ] );
        resTransformation.push_back( refTransformation[ 6 ] );
        resTransformation.push_back( newTranslation.y );
        resTransformation.push_back( refTransformation[ 8 ] );
        resTransformation.push_back( refTransformation[ 9 ] );
        resTransformation.push_back( refTransformation[ 10 ] );
        resTransformation.push_back( newTranslation.z );

        resultVolume.getHeader().addAttribute( "transformations",
                                            resTransformation );

        if ( verbose )
        {

          std::cout << "transformations : " << resTransformation <<  std::endl;

        }

      }

    }

    if ( verbose )
    {

      std::cout << "writing output volume \'" << fileNameOut << "\' : "
                << std::flush;

    }

    // Write output volume
    gkg::Writer::getInstance().write( fileNameOut, resultVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void MergeVolumesProcess::merge( "
             "gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   MergeVolumesCommand
//


gkg::MergeVolumesCommand::MergeVolumesCommand( int32_t argc,
                                 	       char* argv[],
                                 	       bool loadPlugin,
                                 	       bool removeFirst )
                        : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::MergeVolumesCommand::MergeVolumesCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MergeVolumesCommand::MergeVolumesCommand(
                         	      const std::string& fileNameReference,
                         	      const std::string& fileNameFloating,
                         	      const std::string& fileNameTransformation,
                         	      const std::string& fileNameOut,
                         	      int32_t resamplingOrder,
                         	      const std::string& fusionMethod,
                         	      bool forceReferenceSize,
                         	      bool verbose )
                        : gkg::Command()
{

  try
  {

    execute( fileNameReference, fileNameFloating, fileNameTransformation,
             fileNameOut, resamplingOrder, fusionMethod, forceReferenceSize,
             verbose );

  }
  GKG_CATCH( "gkg::MergeVolumesCommand::MergeVolumesCommand( "
             "const std::string& fileNameReference, "
             "const std::string& fileNameFloating, "
             "const std::string& fileNameTransformation, "
             "const std::string& fileNameOut, int32_t resamplingOrder, "
             "const std::string& fusionMethod, bool forceReferenceSize, "
             "bool verbose )" );

}


gkg::MergeVolumesCommand::MergeVolumesCommand(
                                             const gkg::Dictionary& parameters )
                        : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameReference );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameFloating );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTransformation );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, resamplingOrder );
    DECLARE_STRING_PARAMETER( parameters, std::string, fusionMethod );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, forceReferenceSize );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameReference, fileNameFloating, fileNameTransformation,
             fileNameOut, resamplingOrder, fusionMethod, forceReferenceSize,
             verbose );

  }
  GKG_CATCH( "gkg::MergeVolumesCommand::MergeVolumesCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MergeVolumesCommand::~MergeVolumesCommand()
{
}


std::string gkg::MergeVolumesCommand::getStaticName()
{

  try
  {

    return "MergeVolumes";

  }
  GKG_CATCH( "std::string gkg::MergeVolumesCommand::getStaticName()" );

}


void gkg::MergeVolumesCommand::parse()
{

  try
  {

    std::string fileNameReference;
    std::string fileNameFloating;
    std::string fileNameTransformation;
    std::string fileNameOut;
    int32_t resamplingOrder = 0;
    std::string fusionMethod = "average";
    bool forceReferenceSize = false;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "Merge of 2 volumes",
                                  _loadPlugin );
    application.addSingleOption( "-reference",
                                 "Input reference volume",
                                 fileNameReference );
    application.addSingleOption( "-floating",
                                 "Input floating volume",
                                 fileNameFloating );
    application.addSingleOption( "-o",
                                 "name of the output generated volume"
                                 "If empty : only info are displayed",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-t",
                                 "transformation matrix file: floating "
                                 "to reference",
                                 fileNameTransformation,
                                 true );
    application.addSingleOption( "-order",
                                 "Resampling order (-1 (quick resampler),"
                                 " 0 (nearest neighbor) or 1 (linear), "
                                 "default : 0 )",
                                 resamplingOrder,
                                 true );
    application.addSingleOption( "-fusionMethod",
                                 "fusion method when volumes overlapped: \n"
                                 " - add: add voxels values\n"
                                 " - max: max of voxel values\n"
                                 " - average: take voxels average\n"
                                 " - none: set voxel value to 0\n"
                                 "(default=average)",
                                 fusionMethod,
                                 true );
    application.addSingleOption( "-forceReferenceSize",
                                 "Result volume size is forced to"
                                 " reference volume size",
                                 forceReferenceSize,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameReference, fileNameFloating, fileNameTransformation,
             fileNameOut, resamplingOrder, fusionMethod, forceReferenceSize,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MergeVolumesCommand::parse()" );

}


void gkg::MergeVolumesCommand::execute(
                         	      const std::string& fileNameReference,
                         	      const std::string& fileNameFloating,
                         	      const std::string& fileNameTransformation,
                         	      const std::string& fileNameOut,
                         	      int32_t resamplingOrder,
                         	      const std::string& fusionMethod,
                         	      bool forceReferenceSize,
                         	      bool verbose )
{

  try
  {

    // Sanity check
    if ( fusionMethod != "add" &&
         fusionMethod != "max" &&
         fusionMethod != "average" &&
         fusionMethod != "none" )
    {

      throw std::runtime_error( "bad fusionMethod: "
                                "expecting 'add', 'max' or 'average'" );

    }

    MergeVolumesProcess mergeProcess( fileNameFloating,
                                     fileNameTransformation,
                                     fileNameOut,
                                     resamplingOrder,
                                     fusionMethod,
                                     forceReferenceSize,
                                     verbose );
    mergeProcess.execute( fileNameReference );

  }
  GKG_CATCH( "void gkg::MergeVolumesCommand::execute( "
             "const std::string& fileNameReference, "
             "const std::string& fileNameFloating, "
             "const std::string& fileNameTransformation, "
             "const std::string& fileNameOut, int32_t resamplingOrder, "
             "const std::string& fusionMethod, bool forceReferenceSize, "
             "bool verbose )" );

}


RegisterCommandCreator(
                       MergeVolumesCommand,
    		       DECLARE_STRING_PARAMETER_HELP( fileNameReference ) +
    		       DECLARE_STRING_PARAMETER_HELP( fileNameFloating ) +
    		       DECLARE_STRING_PARAMETER_HELP( fileNameTransformation ) +
    		       DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    		       DECLARE_INTEGER_PARAMETER_HELP( resamplingOrder ) +
    		       DECLARE_STRING_PARAMETER_HELP( fusionMethod ) +
    		       DECLARE_BOOLEAN_PARAMETER_HELP( forceReferenceSize ) +
    		       DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
