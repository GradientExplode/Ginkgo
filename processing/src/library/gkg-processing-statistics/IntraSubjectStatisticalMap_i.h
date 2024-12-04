#ifndef _gkg_processing_statistics_IntraSubjectStatisticalMap_i_h_
#define _gkg_processing_statistics_IntraSubjectStatisticalMap_i_h_

#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-histogram/HistogramAnalysis.h>
#include <gkg-processing-statistics/IntraSubjectStatisticalMap.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-processing-statistics/StructureBasedStatisticalMap_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/UpdatableMeanAndStandardDeviation.h>
#include <gkg-core-exception/Exception.h>
#include <map>


template < class T, class S, class Compare >
inline
gkg::IntraSubjectStatisticalMap< T, S, Compare >::IntraSubjectStatisticalMap(
         const std::string& name,
         const gkg::Volume< T >& contrastVolume,
         const gkg::Volume< S >& labelVolume,
         std::list< gkg::IntraSubjectStatisticalMap< T, S, Compare >::Functor >&
           functors,
         const S& background )
                                                 : _background( background ),
                                                   _statisticalMap( name )
{

  try
  {


    // sanity checks
    if ( contrastVolume.getSizeT() != 1 )
    {

      throw std::runtime_error( "T size of contrast volume is not equal to 1" );

    }
    if ( ( contrastVolume.getSizeX() != labelVolume.getSizeX() ) ||
         ( contrastVolume.getSizeX() != labelVolume.getSizeX() ) ||
         ( contrastVolume.getSizeX() != labelVolume.getSizeX() ) )
    {

      throw std::runtime_error(
                               "inconsistent label & contrast volume size(s)" );

    }

    // looping over functor(s)
    typename std::list<
      typename gkg::IntraSubjectStatisticalMap< 
                                            T,
                                            S,
                                            Compare >::Functor >::const_iterator   

      f = functors.begin(),
      fe = functors.end();

    while ( f != fe )
    {
      
      ( *f )( contrastVolume, labelVolume, _background, _statisticalMap );
      ++ f;

    }


  }
  GKG_CATCH( "template < class T, class S, class Compare > "
             "inline "
             "gkg::IntraSubjectStatisticalMap< T, S, Compare >::"
             "IntraSubjectStatisticalMap( "
             "const gkg::Volume< T >& contrastVolume, "
             "const gkg::Volume< S >& labelVolume, "
             "std::list< gkg::IntraSubjectStatisticalMap< T, S, Compare >::"
             "Functor >& functors, "
             "const S& background )" );

}


template < class T, class S, class Compare >
inline
gkg::IntraSubjectStatisticalMap< T, S, Compare >::IntraSubjectStatisticalMap(
                 const gkg::IntraSubjectStatisticalMap< T, S, Compare >& other )
                                                 : _background(
                                                            other._background ),
                                                   _statisticalMap(
                                                         other._statisticalMap )
{
}


template < class T, class S, class Compare >
inline
gkg::IntraSubjectStatisticalMap< T, S, Compare >::~IntraSubjectStatisticalMap()
{
}


template < class T, class S, class Compare >
inline
const S& gkg::IntraSubjectStatisticalMap< T, S, Compare >::getBackground() const
{

  return _background;

}


template < class T, class S, class Compare >
inline
const gkg::StructureBasedStatisticalMap< S, Compare >&
gkg::IntraSubjectStatisticalMap< T, S, Compare >::getStatisticalMap() const
{

  return _statisticalMap;

}



template < class T, class S, class Compare >
inline
void gkg::IntraSubjectStatisticalMap< T, S, Compare >::
                                       grayLevelMeanAndStandardDeviationFunctor(
               const Volume< T >& contrastVolume,
               const Volume< S >& labelVolume,
               const S& background,
               gkg::StructureBasedStatisticalMap< S, Compare >& statisticalMap )
{

  try
  {

    typename std::map< S, gkg::UpdatableMeanAndStandardDeviation, Compare >
      grayLevelStatistics;

    typename
      std::map< S, gkg::UpdatableMeanAndStandardDeviation, Compare >::iterator
      g;

    // collecting volume size(s)
    int32_t sizeX = contrastVolume.getSizeX();
    int32_t sizeY = contrastVolume.getSizeY();
    int32_t sizeZ = contrastVolume.getSizeZ();

    // looping over voxel(s)
    int32_t x, y, z;
    S label;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          label = labelVolume( x, y, z );
          if ( label != background )
          {

            g = grayLevelStatistics.find( label );
            if ( g != grayLevelStatistics.end() )
            {

              g->second.add( ( double )contrastVolume( x, y, z ) );

            }
            else
            {

              gkg::UpdatableMeanAndStandardDeviation
                updatableMeanAndStandardDeviation;
              updatableMeanAndStandardDeviation.add(
                                          ( double )contrastVolume( x, y, z ) );
              grayLevelStatistics.insert(
                   std::make_pair( label, updatableMeanAndStandardDeviation ) );

            }


          }

        }

      }

    }

    // adding mean and standard deviation to statistical map
    typename
      std::map< S,
                gkg::UpdatableMeanAndStandardDeviation,
                Compare >::const_iterator
      cg = grayLevelStatistics.begin(),
      cge = grayLevelStatistics.end();
    while ( cg != cge )
    {

      gkg::StatisticalParameterMap statisticalParameterMap;
      statisticalParameterMap.setStatisticalParameter(
                      "mean", cg->second.getMean() );
      statisticalParameterMap.setStatisticalParameter(
                      "standard_deviation", cg->second.getStandardDeviation() );
      statisticalMap.addStatisticalParameterMap( cg->first,
                                                  "gray_level",
                                                  statisticalParameterMap );
      ++ cg;

    }

  }
  GKG_CATCH( "template < class T, class S, class Compare > "
             "inline "
             "void gkg::IntraSubjectStatisticalMap< T, S, Compare >:: "
             "grayLevelMeanAndStandardDeviationFunctor( "
             "const Volume< T >& contrastVolume, "
             "const Volume< S >& labelVolume, "
             "const S& background, "
             "gkg::StructureBasedStatisticalMap< S, Compare >& "
             "statisticalMap )" );

}


template < class T, class S, class Compare >
inline
void gkg::IntraSubjectStatisticalMap< T, S, Compare >::
                                                   grayLevelAlphaAndBetaFunctor(
               const Volume< T >& contrastVolume,
               const Volume< S >& labelVolume,
               const S& background,
               gkg::StructureBasedStatisticalMap< S, Compare >& statisticalMap )
{

  try
  {

    typename std::map< S, gkg::UpdatableMeanAndStandardDeviation, Compare >
      grayLevelStatistics;

    typename
      std::map< S, gkg::UpdatableMeanAndStandardDeviation, Compare >::iterator
      g;

    // collecting volume size(s)
    int32_t sizeX = contrastVolume.getSizeX();
    int32_t sizeY = contrastVolume.getSizeY();
    int32_t sizeZ = contrastVolume.getSizeZ();

    // looping over voxel(s)
    int32_t x, y, z;
    S label;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          label = labelVolume( x, y, z );
          if ( label != background )
          {

            g = grayLevelStatistics.find( label );
            if ( g != grayLevelStatistics.end() )
            {

              g->second.add( ( double )contrastVolume( x, y, z ) );

            }
            else
            {

              gkg::UpdatableMeanAndStandardDeviation
                updatableMeanAndStandardDeviation;
              updatableMeanAndStandardDeviation.add(
                                          ( double )contrastVolume( x, y, z ) );
              grayLevelStatistics.insert(
                   std::make_pair( label, updatableMeanAndStandardDeviation ) );

            }


          }

        }

      }

    }

    //computing grey_level_alpha for each roi

    typename std::map< S, gkg::UpdatableMeanAndStandardDeviation, Compare >
      grayLevelAlphaAndBetaStatistics;

    typename
      std::map< S,
                gkg::UpdatableMeanAndStandardDeviation,
                Compare >::const_iterator
      cg = grayLevelStatistics.begin(),
      cge = grayLevelStatistics.end();

    gkg::HistogramAnalysis histogramAnalysis( contrastVolume );
    
    while ( cg != cge )
    {

      double alpha = ( cg->second.getMean() - 
                             histogramAnalysis.getGrayMatterStatistics().mean )/
                     ( histogramAnalysis.getWhiteMatterStatistics().mean -
                             histogramAnalysis.getGrayMatterStatistics().mean );

      double beta = 2.0 * cg->second.getStandardDeviation() /
              ( histogramAnalysis.getWhiteMatterStatistics().standardDeviation +
                histogramAnalysis.getGrayMatterStatistics().standardDeviation );

      // adding gray_level_alpha and gray_level_beta to statistical map


      gkg::StatisticalParameterMap statisticalParameterMap;
      statisticalParameterMap.setStatisticalParameter( "value", alpha );
      statisticalMap.addStatisticalParameterMap( cg->first,
                                                 "gray_level_alpha",
                                                 statisticalParameterMap );

      gkg::StatisticalParameterMap statisticalParameterMap2;
      statisticalParameterMap2.setStatisticalParameter( "value", beta );
      statisticalMap.addStatisticalParameterMap( cg->first,
                                                 "gray_level_beta",
                                                 statisticalParameterMap2 );
      ++ cg;

    }

  }
  GKG_CATCH( "template < class T, class S, class Compare > "
             "inline "
             "void gkg::IntraSubjectStatisticalMap< T, S, Compare >:: "
             "grayLevelAlphaAndBetaFunctor( "
             "const Volume< T >& contrastVolume, "
             "const Volume< S >& labelVolume, "
             "const S& background, "
             "gkg::StructureBasedStatisticalMap< S, Compare >& "
             "statisticalMap )" );

}








template < class T, class S, class Compare >
inline
void gkg::IntraSubjectStatisticalMap< T, S, Compare >::surfaceFunctor(
               const Volume< T >& contrastVolume,
               const Volume< S >& labelVolume,
               const S& background,
               gkg::StructureBasedStatisticalMap< S, Compare >& statisticalMap )
{

  try
  {

    typename std::map< S, double, Compare > surfaces;

    typename std::map< S, double, Compare >::iterator s;

    // collecting volume size(s) and resolution(s)
    int32_t sizeX = contrastVolume.getSizeX();
    int32_t sizeY = contrastVolume.getSizeY();
    int32_t sizeZ = contrastVolume.getSizeZ();
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    if ( contrastVolume.getHeader().hasAttribute( "resolutionX" ) )
    {

      contrastVolume.getHeader().getAttribute( "resolutionX", resolutionX );

    }
    if ( contrastVolume.getHeader().hasAttribute( "resolutionY" ) )
    {

      contrastVolume.getHeader().getAttribute( "resolutionY", resolutionY );

    }
    if ( contrastVolume.getHeader().hasAttribute( "resolutionZ" ) )
    {

      contrastVolume.getHeader().getAttribute( "resolutionZ", resolutionZ );

    }
    double surfaceXY = resolutionX * resolutionY;
    double surfaceXZ = resolutionX * resolutionZ;
    double surfaceYZ = resolutionY * resolutionZ;

    gkg::BoundingBox< int32_t > bbox( labelVolume );   


    // looping over voxel(s)
    int32_t x, y, z;
    S label;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          label = labelVolume( x, y, z );
          if ( label != background )
          {

            double surface = 0.0;
            s = surfaces.find( label );
            std::list< gkg::Vector3d< int32_t > > neighbors =
            gkg::Neighborhood3d::getInstance().getNeighborOffsets( 
			                gkg::Neighborhood3d::Neighborhood3d_6 );

            std::list< gkg::Vector3d< int32_t > >::iterator 
              inb = neighbors.begin(),
              ine = neighbors.end();

            while ( inb != ine )
            {

              gkg::Vector3d< int32_t > point( x + inb->x, 
				              y + inb->y, 
				              z + inb->z );

              if ( bbox.contains( point ) )
              {

                if( !( labelVolume( point ) == label ) )
                {

                  if( inb->x != 0 )
                  {

                    surface += surfaceYZ;

                  }
                  if( inb->y != 0 )
                  {

                    surface += surfaceXZ;

                  }
                  if( inb->z != 0 )
                  {

                    surface += surfaceXY;
                  
                  }

                }

              }

              ++ inb;

            }

            s = surfaces.find( label );
            if ( s != surfaces.end() )
            {

              s->second += surface;

            }
            else
            {

              surfaces.insert( std::make_pair( label, surface ) );

            }

          }

        }

      }

    }

    // adding surface value to statistical map
    typename std::map< S, double, Compare >::const_iterator
      cs = surfaces.begin(),
      cse = surfaces.end();
    while ( cs != cse )
    {

      gkg::StatisticalParameterMap statisticalParameterMap;
      statisticalParameterMap.setStatisticalParameter( "value", cs->second );
      statisticalMap.addStatisticalParameterMap( cs->first,
                                                 "surface",
                                                 statisticalParameterMap );
      ++ cs;

    }

  }
  GKG_CATCH( "template < class T, class S, class Compare > "
             "inline "
             "void gkg::IntraSubjectStatisticalMap< T, S, Compare >:: "
             "surfaceFunctor( "
             "const Volume< T >& contrastVolume, "
             "const Volume< S >& labelVolume, "
             "const S& background, "
             "gkg::StructureBasedStatisticalMap< S, Compare >& "
             "statisticalMap )" );

}



template < class T, class S, class Compare >
inline
void gkg::IntraSubjectStatisticalMap< T, S, Compare >::volumeFunctor(
               const Volume< T >& contrastVolume,
               const Volume< S >& labelVolume,
               const S& background,
               gkg::StructureBasedStatisticalMap< S, Compare >& statisticalMap )
{

  try
  {

    typename std::map< S, double, Compare > volumes;

    typename std::map< S, double, Compare >::iterator v;

    // collecting volume size(s) and resolution(s)
    int32_t sizeX = contrastVolume.getSizeX();
    int32_t sizeY = contrastVolume.getSizeY();
    int32_t sizeZ = contrastVolume.getSizeZ();
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    if ( contrastVolume.getHeader().hasAttribute( "resolutionX" ) )
    {

      contrastVolume.getHeader().getAttribute( "resolutionX", resolutionX );

    }
    if ( contrastVolume.getHeader().hasAttribute( "resolutionY" ) )
    {

      contrastVolume.getHeader().getAttribute( "resolutionY", resolutionY );

    }
    if ( contrastVolume.getHeader().hasAttribute( "resolutionZ" ) )
    {

      contrastVolume.getHeader().getAttribute( "resolutionZ", resolutionZ );

    }
    double voxelVolume = resolutionX * resolutionY * resolutionZ;

    // looping over voxel(s)
    int32_t x, y, z;
    S label;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          label = labelVolume( x, y, z );
          if ( label != background )
          {

            v = volumes.find( label );
            if ( v != volumes.end() )
            {

              v->second += voxelVolume;

            }
            else
            {

              volumes.insert( std::make_pair( label, voxelVolume ) );

            }


          }

        }

      }

    }

    // adding mean and standard deviation to statistical map
    typename std::map< S, double, Compare >::const_iterator
      cv = volumes.begin(),
      cve = volumes.end();
    while ( cv != cve )
    {

      gkg::StatisticalParameterMap statisticalParameterMap;
      statisticalParameterMap.setStatisticalParameter( "value", cv->second );
      statisticalMap.addStatisticalParameterMap( cv->first,
                                                 "volume",
                                                 statisticalParameterMap );
      ++ cv;

    }

  }
  GKG_CATCH( "template < class T, class S, class Compare > "
             "inline "
             "void gkg::IntraSubjectStatisticalMap< T, S, Compare >:: "
             "volumeFunctor( "
             "const Volume< T >& contrastVolume, "
             "const Volume< S >& labelVolume, "
             "const S& background, "
             "gkg::StructureBasedStatisticalMap< S, Compare >& "
             "statisticalMap )" );

}


#endif

