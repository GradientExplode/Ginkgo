// threaded loop


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-core-cppext/CUniStd.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


class FooContext : public gkg::LoopContext< int32_t >
{

  public:

    FooContext( /* put here the parameters of the algorithm */ )
              : gkg::LoopContext< int32_t >() { }
    ~FooContext() { }

    void doIt( int32_t startIndex, int32_t count )
    {

      for ( int32_t i = startIndex; i < startIndex + count; i++ )
      {

        sleep( 1 );
	lock();
        std::cerr << " index = " << i
                  << "  [" 
                  << gkg::SystemInfo::getInstance().getCurrentThreadId()
                  << "]" 
                  << std::endl;
	unlock();

      }

    }

  private:

    // private parameters of the algorithm

};


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    FooContext fooContext/* (my parameters for the algorithm )*/;
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &fooContext,
                    3,     // the start index of the global loop
                    15,    // the count to be done
                    2,     // 2 CPU used for this example
                    2,     // 2 threads are launched on each CPU
                    true   // verbose mode
                  );

    // That will give, for instance, on a single processor computer:
    // 1 thread does iterations i=3,4,5,6,7.
    // Parallely, 1 thread does iterations i=8,9,10,11,12.
    // The first of the threads that has finished realizes the 5 last iterations
    // i=13,14,15,16,17.
    // As a result, 2 threads are created on that single-processor computer.
  
    // As for a computer doted with 2 processors:
    // 3 threads are created; 2 threads on the processor I that deals with
    // iterations i=3,4,5,6,7 and i=13,14,15,16,17, and 1 thread on 
    // processor II that deals with iterations i=8,9,10,11,12
  
    // Typically, one should use 1 thread by CPU, and one should use all
    // CPUs in image processing algorithms; so we often have to implement
    // a threaded loop of the following type :
    //   gkg::ThreadedLoop< int32_t >
    //       loop( &myAlgoContext, tartIter, count, 0, 1 );
    // or simply
    //   gkg::ThreadedLoop< int32_t > loop( &myAlgoContext, tartIter, count );
		   
    threadedLoop.launch(); // start the loop process

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
