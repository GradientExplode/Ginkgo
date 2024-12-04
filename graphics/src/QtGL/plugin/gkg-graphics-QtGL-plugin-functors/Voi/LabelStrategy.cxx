#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ConnectivityFactory.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume_i.h>


gkg::VoiLabelStrategy::~VoiLabelStrategy()
{
}


gkg::VoiLabelStrategy::TypeId gkg::VoiLabelStrategy::id()
{ 

  return (gkg::VoiLabelStrategy::TypeId)0;
  
}


float gkg::VoiLabelStrategy::grayLevel( gkg::Volume< float >& vol, 
                                        gkg::Vector3d< int32_t >& p, 
                                        int32_t t )
{

  return vol( p.x, p.y, p.z, t );

}


void gkg::VoiLabelStrategy::doit( gkg::VoiData*, gkg::Volume< int16_t >&, 
                                  gkg::Vector3d< int32_t >&, int32_t, 
                                  int32_t, std::list< Vector2d< int32_t > >& )
{
}


void gkg::VoiLabelStrategy::fill( gkg::VoiData* d, gkg::Volume< int16_t >& lab, 
                                  int32_t x, int32_t y, int32_t z, 
                                  int32_t fond, int32_t col, 
                                  int32_t* /* nbPt */ )
{

  gkg::VoiConnectivityFactory cfact;
  std::list< gkg::Vector3d< int32_t > > c = 
    cfact.create( gkg::VoiEvolutionInfo::Connex4, id() );
  gkg::BoundingBox< int32_t > bb( 0, lab.getSizeX() - 1,
                                  0, lab.getSizeY() - 1,
                                  0, lab.getSizeZ() - 1 );
  std::list< gkg::Vector3d< int32_t > > front;
  std::list< gkg::Vector3d< int32_t > > next;
  gkg::Vector3d< int32_t > tmpP( x, y, z );
  bool inObj = true;

  front.push_back( tmpP );
  bck.clear();
  bck.push_back( tmpP );
  lab( tmpP ) = (int16_t)col;

  std::list< gkg::Vector3d< int32_t > >::iterator fit, fitEnd;
  std::list< gkg::Vector3d< int32_t > >::iterator
    itc, iec = c.end();

  for ( int32_t loop=0; 
        ( loop < 1000 ) && ( front.size() < gkg::VoiLabelStrategy::maxFill );
        loop++ )
  {

    fit = front.begin();
    fitEnd = front.end();

    while ( fit != fitEnd )
    {

      inObj = true;
      itc = c.begin();
      
      while ( itc != iec )
      {
      
        tmpP = *fit + *itc;
	
        if ( bb.contains( tmpP.x, tmpP.y, tmpP.z ) )
        {

          int32_t val = lab( tmpP );

          if ( val == fond )
          {
	
            next.push_back( tmpP );
            lab( tmpP ) = (int16_t)col;
	    
          }
          else if ( val != col )
          {
	  
            inObj = false;
	  
          }
	  
        }
	
        ++itc;
	
      }

      if ( !inObj )
      {
      
        next.push_back( *fit );

      }
	  
      ++fit;

    }

    bck.insert( bck.end(), next.begin(), next.end() );
    front.clear();
    front = next;
    next.clear();

  }

  d->notifyLabel( bck );

}


void gkg::VoiLabelStrategy::evolution( gkg::VoiData *data, 
                                       gkg::Vector3d< int32_t >& pt, int32_t t, 
                                       int32_t label, 
                                       gkg::VoiEvolutionInfo& ev )
{

  int32_t nModif = 100;
  float dU = 0.0, expo = 0.0;
  bool inObj = true;
  std::list< gkg::Vector3d< int32_t > >::iterator fit, fitEnd;

  gkg::VoiConnectivityFactory cfact;

  std::list< gkg::Vector3d< int32_t > > c = cfact.create( ev.connex(), id() );

  gkg::Volume< float >& vol = data->getBaseFusion().getT1();
  gkg::Volume< int16_t >& lab = data->label();

  gkg::BoundingBox< int32_t > bb( 0, vol.getSizeX() - 1,
                                  0, vol.getSizeY() - 1,
                                  0, vol.getSizeZ() - 1,
                                  0, vol.getSizeT() - 1 );

  float cI = ev.ising();
  float kin = ev.kin();
  float kout = ev.kout();

  std::list< gkg::Vector3d< int32_t > > front;
  std::list< gkg::Vector3d< int32_t > > next;

  front.push_back( pt );
  bck.clear();
  bck.push_back( pt );

  float evolValue = data->plage() * float( ev.value() );
  float var = evolValue * evolValue;
  float mean = vol( pt );

  std::list< gkg::Vector3d< int32_t > >::iterator 
    itc = c.begin(),
    iec = c.end();
  
  gkg::Vector3d< int32_t > tmpP;
  while ( itc != iec )
  {
  
    tmpP = pt + *itc;
  
    if ( bb.contains( tmpP.x, tmpP.y, tmpP.z, t ) )
    {
    
      mean += vol( tmpP.x, tmpP.y, tmpP.z, t );
    
    }
    
    ++itc;
  
  }

  mean /= c.size() + 1;
  lab( pt ) = (int16_t)label;

  for ( int32_t loop=0; 
        ( loop < 1000 ) && ( nModif > 2 ) && ( front.size() < 5000 );
        loop++ )
  {
    
    nModif = 0;
    fit = front.begin();
    fitEnd = front.end();

    while ( fit != fitEnd )
    {

      inObj = true;
      itc = c.begin();
      
      while ( itc != iec )
      {
      
        dU = 0.0;
        tmpP = *fit + *itc;
	
        if ( bb.contains( tmpP.x, tmpP.y, tmpP.z, t ) )
        {
	
          float val = vol( tmpP.x, tmpP.y, tmpP.z, t );

          if ( lab( tmpP ) == 0 )
          {
	
            expo = ( val - mean ) * ( val - mean ) / var;
            dU = ( expo > 4.0 ) ? 4.0 : expo;
            dU *= kin;
            dU -= kout;
	  
            std::list< gkg::Vector3d< int32_t > >::iterator 
              itc2 = c.begin(),
              iec2 = c.end();

            while ( itc2 != iec2 )
            {
	  
              gkg::Vector3d< int32_t > tp = tmpP + *itc2;
	  
              if ( bb.contains( tp.x, tp.y, tp.z, t ) )
              {
	    
                if ( lab( tp ) != (int16_t)label )
                {
	      
                  dU += cI;
		
                }
                else
                {
	      
                  dU -= cI;
		 
                }
	      
              }
	    
              ++itc2;
	    
            }

            if ( dU < 0.0 )
            {
	  
              next.push_back( tmpP );
              lab( tmpP ) = (int16_t)label;
              nModif++;
	    
            }
            else 
            {
	  
              inObj = false;
	    
            }
	  
          }
	  
        }

        ++itc;
	
      }

      if ( !inObj )
      {
      
        next.push_back( *fit );

      }
	  
      ++fit;
	  
    }

    bck.insert( bck.end(), next.begin(), next.end() );
    front.clear();
    front = next;
    next.clear();
      
  }

  data->notifyLabel( bck );
  
}


void gkg::VoiLabelStrategy::spline( VoiData*, gkg::Volume< int16_t >&,
                                    std::list< QPoint >&, int32_t, int32_t )
{
}


void gkg::VoiLabelStrategy::morphology( VoiData* data,
                                        gkg::Volume< int16_t >& roi,
                                        int32_t label )
{

  gkg::Volume< int16_t >& lab = data->label();
  int32_t x, sizeX = lab.getSizeX();
  int32_t y, sizeY = lab.getSizeY();
  int32_t z, sizeZ = lab.getSizeZ();
  int16_t* lPtr = &lab( 0 );
  int16_t* rPtr = &roi( 0 );

  bck.clear();

  for ( z = 0; z < sizeZ; z++ )
  {

    for ( y = 0; y < sizeY; y++ )
    {

      for ( x = 0; x < sizeX; x++, lPtr++, rPtr++ )
      {

        if ( !*lPtr && *rPtr )
        {

          *lPtr = *rPtr;
          bck.push_back( gkg::Vector3d< int32_t >( x, y, z ) );

       }
       else if ( !*rPtr && ( *lPtr == label ) )
       {

          *lPtr = 0;
          bck.push_back( gkg::Vector3d< int32_t >( x, y, z ) );

        }

      }

    }

  }

  data->notifyLabel( bck );
  bck.clear();

}


void gkg::VoiLabelStrategy::undo( gkg::VoiData *d )
{

  if ( bck.size() )
  {

    gkg::Volume< int16_t >& lab = d->label();
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      it = bck.begin(),
      ie = bck.end();

    while ( it != ie )
    {

      lab( *it ) = 0;
      ++it;
    
    }

    d->notifyLabel( bck );
    bck.clear();

  }

}


void gkg::VoiLabelStrategy::clearSelection( gkg::VoiData *d, int32_t l )
{

  std::list< gkg::Vector3d< int32_t > > bckClear;
  gkg::Volume< int16_t >& label = d->label();
  int32_t x, sizeX = label.getSizeX();
  int32_t y, sizeY = label.getSizeY();
  int32_t z, sizeZ = label.getSizeZ();

  for ( z = 0; z < sizeZ; z++ )
  {

    for ( y = 0; y < sizeY; y++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        gkg::Vector3d< int32_t > pt( x, y, z );

        if ( label( pt ) == (int16_t)l )
        {

          label( pt ) = 0;
          bckClear.push_back( pt );

        }

      }

    }

  }

  d->notifyLabel( bckClear );

}


void gkg::VoiLabelStrategy::clearAll( gkg::VoiData *d )
{

  std::list< gkg::Vector3d< int32_t > > bckClear;
  gkg::Volume< int16_t >& label = d->label();
  int32_t x, sizeX = label.getSizeX();
  int32_t y, sizeY = label.getSizeY();
  int32_t z, sizeZ = label.getSizeZ();

  for ( z = 0; z < sizeZ; z++ )
  {

    for ( y = 0; y < sizeY; y++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        gkg::Vector3d< int32_t > pt( x, y, z );

        if ( label( pt ) )
        {

          label( pt ) = 0;
          bckClear.push_back( pt );

        }

      }

    }

  }

  d->notifyLabel( bckClear );

}
