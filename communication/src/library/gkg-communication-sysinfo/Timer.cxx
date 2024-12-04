#include <gkg-communication-sysinfo/Timer.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::Timer::Timer()
{
}


gkg::Timer::~Timer()
{
}


void gkg::Timer::storeTime()
{

  try
  {

    _times.push_back( clock() );

  }
  GKG_CATCH( "void gkg::Timer::storeTime()" );

}


std::list< double > gkg::Timer::getTimesInMilliseconds() const
{

  try
  {

    std::list< double > timesInMilliseconds;

    std::list< clock_t >::const_iterator 
      t = _times.begin(),
      te = _times.end();

    while ( t != te )
    {

      timesInMilliseconds.push_back( ( double )*t /
                                     ( CLOCKS_PER_SEC / 100.0 ) );
      ++ t;

    }

    return timesInMilliseconds;

  }
  GKG_CATCH( "std::list< double > gkg::Timer::getTimesInMilliseconds() const" );

}



std::list< double > gkg::Timer::getTimesInSeconds() const
{

  try
  {

    std::list< double > timesInSeconds;

    std::list< clock_t >::const_iterator 
      t = _times.begin(),
      te = _times.end();

    while ( t != te )
    {

      timesInSeconds.push_back( ( double )*t /
                                ( CLOCKS_PER_SEC / 0.1 ) );
      ++ t;

    }
    return timesInSeconds;

  }
  GKG_CATCH( "std::list< double > gkg::Timer::getTimesInSeconds() const" );

}



std::list< double > gkg::Timer::getTimeLapsInMilliseconds() const
{

  try
  {

    std::list< double > timeLapsInMilliseconds;
    std::list< clock_t >::const_iterator 
      t1 = _times.begin(),
      te = _times.end();

    std::list< clock_t >::const_iterator t2 = t1;
    ++ t2;

    while ( t2 != te )
    {

      timeLapsInMilliseconds.push_back(
                                 getClockDifferenceInMilliseconds( *t1, *t2 ) );
      ++ t1;
      ++ t2;

    }

    return timeLapsInMilliseconds;

  }
  GKG_CATCH( "std::list< double > "
             "gkg::Timer::getTimeLapsInMilliseconds() const" );

}


std::list< double > gkg::Timer::getTimeLapsInSeconds() const
{

  try
  {

    std::list< double > timeLapsInSeconds;
    std::list< clock_t >::const_iterator 
      t1 = _times.begin(),
      te = _times.end();

    std::list< clock_t >::const_iterator t2 = t1;
    ++ t2;

    while ( t2 != te )
    {

      timeLapsInSeconds.push_back( getClockDifferenceInSeconds( *t1, *t2 ) );
      ++ t1;
      ++ t2;

    }

    return timeLapsInSeconds;

  }
  GKG_CATCH( "std::list< double > "
             "gkg::Timer::getTimeLapsInMilliseconds() const" );

}


void gkg::Timer::printTimesInMilliseconds() const
{

  try
  {

    std::list< clock_t >::const_iterator 
      t = _times.begin(),
      te = _times.end();

    while ( t != te )
    {

      std::cout << ( double )*t / ( CLOCKS_PER_SEC / 100.0 )
                << " " << std::flush;
      ++ t;

    }

  }
  GKG_CATCH( "void gkg::Timer::printTimesInMilliseconds() const" );

}


void gkg::Timer::printTimesInSeconds() const
{

  try
  {

    std::list< clock_t >::const_iterator 
      t = _times.begin(),
      te = _times.end();

    while ( t != te )
    {

      std::cout << ( double )*t / ( CLOCKS_PER_SEC / 0.1 )
                << " " << std::flush;
      ++ t;

    }

  }
  GKG_CATCH( "void gkg::Timer::printTimesInSeconds() const" );

}


void gkg::Timer::printLapsInMilliseconds() const
{

  try
  {

    std::list< clock_t >::const_iterator 
      t1 = _times.begin(),
      te = _times.end();

    std::list< clock_t >::const_iterator t2 = t1;
    ++ t2;

    while ( t2 != te )
    {

      std::cout << getClockDifferenceInMilliseconds( *t1, *t2 )
                << " " << std::flush;
      ++ t1;
      ++ t2;

    }

  }
  GKG_CATCH( "void gkg::Timer::printLapsInMilliseconds() const" );

}


void gkg::Timer::printLapsInSeconds() const
{

  try
  {

    std::list< clock_t >::const_iterator 
      t1 = _times.begin(),
      te = _times.end();

    std::list< clock_t >::const_iterator t2 = t1;
    ++ t2;

    while ( t2 != te )
    {

      std::cout << getClockDifferenceInSeconds( *t1, *t2 )
                << " " << std::flush;
      ++ t1;
      ++ t2;

    }

  }
  GKG_CATCH( "void gkg::Timer::printLapsInSeconds() const" );

}


double gkg::Timer::getClockDifferenceInMilliseconds( clock_t clock1, 
                                                     clock_t clock2 ) const
{

  try
  {

    double differenceTicks = clock2 - clock1;
    double differenceInMilliseconds = ( differenceTicks ) /
                                      ( CLOCKS_PER_SEC / 100.0 );
    return differenceInMilliseconds;

  }
  GKG_CATCH( "double gkg::Timer::getClockDifferenceInMilliseconds( "
             "clock_t clock1, clock_t clock2 ) const" );

}


double gkg::Timer::getClockDifferenceInSeconds( clock_t clock1, 
                                                clock_t clock2 ) const
{

  try
  {

    double differenceTicks = clock2 - clock1;
    double differenceInSeconds = ( differenceTicks ) /
                                 ( CLOCKS_PER_SEC / 0.1 );
    return differenceInSeconds;

  }
  GKG_CATCH( "double gkg::Timer::getClockDifferenceInSeconds( "
             "clock_t clock1, clock_t clock2 ) const" );

}
