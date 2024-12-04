#include <gkg-processing-morphology/Skeletonization.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-algobase/InverseFunction_i.h>
#include <gkg-processing-morphology/MaurerDistanceMapper.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-processing-topology/TopologyClassifier_i.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <map>
#include <list>
#include <iomanip>


template < class IN, class OUT >
gkg::Skeletonization< IN, OUT >::Skeletonization(
                                    const gkg::TestFunction< IN >& testFunction,
                                    const OUT& foreground,
                                    const OUT& background )
                               : _testFunction( testFunction ),
                                 _foreground( foreground ),
                                 _background( background )

{
}


template < class IN, class OUT >
gkg::Skeletonization< IN, OUT >::~Skeletonization()
{
}


template < class IN, class OUT >
void gkg::Skeletonization< IN, OUT >::skeletonize( 
                                         const gkg::Volume< IN >& inputVolume,
                                         gkg::Volume< OUT >& skeletonizedVolume,
                                         bool verbose ) const
{

  try
  {

    gkg::InverseFunction< IN >*
      inverseFunction = new InverseFunction< IN >( _testFunction );

    gkg::MaurerDistanceMapper< IN > distanceMapper( *inverseFunction );
    gkg::Volume< float > distanceMap;

    distanceMapper.getDistance( inputVolume, distanceMap, verbose );

    int32_t sizeX = inputVolume.getSizeX();
    int32_t sizeY = inputVolume.getSizeY();
    int32_t sizeZ = inputVolume.getSizeZ();
    gkg::Vector3d< int32_t > site;
    std::map< float, std::list< gkg::Vector3d< int32_t > > > fronts;
    gkg::GreaterThanFunction< float > testFunction( 0.0f );

    if ( verbose )
    {

      std::cout << "Getting fronts... " << std::flush;

    }

    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for ( site.x = 0; site.x < sizeX; site.x++ )
        {

          float distance = distanceMap( site );
			
	  if ( testFunction.getState( distance ) ) 
	  {
	          
            std::map< float, std::list< gkg::Vector3d< int32_t > > >::iterator 
	      f = fronts.find( distance ); 
	            
	    if ( f == fronts.end() ) 
	    {

	      std::list< gkg::Vector3d< int32_t > > sites;
	      sites.push_back( site ); 
	      fronts.insert( std::make_pair( distance, sites ) );

	    }
	    else
	    {

              f->second.push_back( site );

	    }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    int32_t transitionCount = 0;
    int32_t count = 0;
    gkg::BoundingBox< int32_t > bbox( distanceMap );
    std::list< gkg::Vector3d< int32_t > > neighbors =
      gkg::Neighborhood3d::getInstance().getNeighborOffsets( 
                                       gkg::Neighborhood3d::Neighborhood3d_26 );

    if ( verbose )
    {

      std::cout << "Iteration " << std::setw( 3 ) << count + 1
                << " - " << std::flush;

    }

    do 
    { 
	
      transitionCount = 0;

      std::map< float, std::list< gkg::Vector3d< int32_t > > >::iterator 
        f = fronts.begin(),
        fe = fronts.end(); 
        
      while ( f != fe )
      {
		
        std::list< gkg::Vector3d< int32_t > > sites;
        std::list< gkg::Vector3d< int32_t > >::iterator 
          p = f->second.begin(), 
          pe = f->second.end();
			  
        while ( p != pe )
        {

          bool isASimplePoint = 
            gkg::TopologyClassifier::getInstance().isSimplePoint( distanceMap, 
			                                          testFunction, 
			                                          *p );
				                                           
          if ( !isASimplePoint )
          {

            sites.push_back( *p );
				
          }
          else
          {

            int32_t n = 0;
            std::list< gkg::Vector3d< int32_t > >::iterator
              v = neighbors.begin(),
              ve = neighbors.end();
				  
            while ( v != ve )
            {

              gkg::Vector3d< int32_t > neighbor = *p + *v;
  				  
              if ( bbox.contains( neighbor ) && 
                   ( distanceMap( neighbor ) > 0.0f ) )
              {

                n++;

              }
  				  
              ++v;

            }
				  
            if ( n > 1 )
            {

              distanceMap( *p ) = 0.0f;

            }
            else
            {

              sites.push_back( *p );

            }

          }
			                                           
          ++p;
				
        }
			
	int32_t diffCount = (int32_t)( f->second.size() - 
	                               sites.size() );
	transitionCount += diffCount;
		
	if ( diffCount )
	{

	  f->second = sites;

	}
	
	++f;
		  
      }

      if ( verbose )
      {

        if ( count )
        {

          std::cout << gkg::Eraser( 28 ) << std::flush;

        }
        else
        {

          std::cout << gkg::Eraser( 6 ) << std::flush;

        }

        std::cout << std::setw( 3 ) << count + 1
                  << " - " << "Transitions : " 
                  << std::setw( 8 ) << transitionCount << std::flush;

      }

      count++;
	  
    }
    while ( transitionCount );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 25 ) << " - done" << std::endl;

    }

    gkg::Vector3d< double > resolution;

    inputVolume.getResolution( resolution );
    skeletonizedVolume.reallocate( sizeX, sizeY, sizeZ );
    skeletonizedVolume.fill( _background );
    skeletonizedVolume.setResolution( resolution );

    std::map< float,
              std::list< gkg::Vector3d< int32_t > > >::iterator 
      f = fronts.begin(),
      fe = fronts.end();
	
    while ( f != fe )
    {

      std::list< gkg::Vector3d< int32_t > >::iterator
        p = f->second.begin(),
        pe = f->second.end();
                      
      while ( p != pe )
      {

        skeletonizedVolume( *p ) = _foreground;
        ++p;

      }
      
      ++f;
      
    }

    delete inverseFunction;

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::Skeletonization< IN, OUT >::skeletonize( "
             "const gkg::Volume< IN >& inputVolume, "
             "gkg::Volume< OUT >& skeletonizedVolume, "
             "bool verbose = false ) const" );

}


template class gkg::Skeletonization< int8_t, uint8_t >;
template class gkg::Skeletonization< uint8_t, uint8_t >;
template class gkg::Skeletonization< int16_t, uint8_t >;
template class gkg::Skeletonization< uint16_t, uint8_t >;
template class gkg::Skeletonization< int32_t, uint8_t >;
template class gkg::Skeletonization< uint32_t, uint8_t >;
template class gkg::Skeletonization< int64_t, uint8_t >;
template class gkg::Skeletonization< uint64_t, uint8_t >;
template class gkg::Skeletonization< float, uint8_t >;
template class gkg::Skeletonization< double, uint8_t >;

template class gkg::Skeletonization< int8_t, int16_t >;
template class gkg::Skeletonization< uint8_t, int16_t >;
template class gkg::Skeletonization< int16_t, int16_t >;
template class gkg::Skeletonization< uint16_t, int16_t >;
template class gkg::Skeletonization< int32_t, int16_t >;
template class gkg::Skeletonization< uint32_t, int16_t >;
template class gkg::Skeletonization< int64_t, int16_t >;
template class gkg::Skeletonization< uint64_t, int16_t >;
template class gkg::Skeletonization< float, int16_t >;
template class gkg::Skeletonization< double, int16_t >;

template class gkg::Skeletonization< int8_t, int32_t >;
template class gkg::Skeletonization< uint8_t, int32_t >;
template class gkg::Skeletonization< int16_t, int32_t >;
template class gkg::Skeletonization< uint16_t, int32_t >;
template class gkg::Skeletonization< int32_t, int32_t >;
template class gkg::Skeletonization< uint32_t, int32_t >;
template class gkg::Skeletonization< int64_t, int32_t >;
template class gkg::Skeletonization< uint64_t, int32_t >;
template class gkg::Skeletonization< float, int32_t >;
template class gkg::Skeletonization< double, int32_t >;

template class gkg::Skeletonization< int8_t, float >;
template class gkg::Skeletonization< uint8_t, float >;
template class gkg::Skeletonization< int16_t, float >;
template class gkg::Skeletonization< uint16_t, float >;
template class gkg::Skeletonization< int32_t, float >;
template class gkg::Skeletonization< uint32_t, float >;
template class gkg::Skeletonization< int64_t, float >;
template class gkg::Skeletonization< uint64_t, float >;
template class gkg::Skeletonization< float, float >;
template class gkg::Skeletonization< double, float >;

template class gkg::Skeletonization< int8_t, double >;
template class gkg::Skeletonization< uint8_t, double >;
template class gkg::Skeletonization< int16_t, double >;
template class gkg::Skeletonization< uint16_t, double >;
template class gkg::Skeletonization< int32_t, double >;
template class gkg::Skeletonization< uint32_t, double >;
template class gkg::Skeletonization< int64_t, double >;
template class gkg::Skeletonization< uint64_t, double >;
template class gkg::Skeletonization< float, double >;
template class gkg::Skeletonization< double, double >;
