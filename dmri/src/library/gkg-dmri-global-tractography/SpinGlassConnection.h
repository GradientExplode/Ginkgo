#ifndef _gkg_dmri_global_tractography_SpinGlassConnection_h_
#define _gkg_dmri_global_tractography_SpinGlassConnection_h_


#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class SpinGlassConnection
{

  public:

    SpinGlassConnection( uint64_t identity,
                         const std::pair< RCPointer< SpinGlass >,
                                          SpinGlass::ExtremityType >&
                                                   spinGlassAndExtremityType1,
                         const std::pair< RCPointer< SpinGlass >,
                                          SpinGlass::ExtremityType >&
                                                   spinGlassAndExtremityType2 );

    SpinGlassConnection( const SpinGlassConnection& other );
    virtual ~SpinGlassConnection();

    SpinGlassConnection& operator=( const SpinGlassConnection& other );

    uint64_t getIdentity() const;

    const RCPointer< SpinGlass >& getSpinGlass1() const;
    const SpinGlass::ExtremityType& getSpinGlassExtremityType1() const;

    const RCPointer< SpinGlass >& getSpinGlass2() const;
    const SpinGlass::ExtremityType& getSpinGlassExtremityType2() const;

    void getExtremityTypeOfConnectedSpinGlass(
                                const RCPointer< SpinGlass >& spinGlass,
                                SpinGlass::ExtremityType& extremityType ) const;

    Vector3d< float > getFirstExtremity() const;
    Vector3d< float > getSecondExtremity() const;

    //float getInteractionPotential() const;

    void disconnect();
    void connect( const std::pair< RCPointer< SpinGlass >,
                                   SpinGlass::ExtremityType >&
                                                   spinGlassAndExtremityType1,
                  const std::pair< RCPointer< SpinGlass >,
                                   SpinGlass::ExtremityType >&
                                                   spinGlassAndExtremityType2 );

  protected:

    uint64_t _identity;
    std::pair< RCPointer< SpinGlass >,
               SpinGlass::ExtremityType > _spinGlassAndExtremityType1;
    std::pair< RCPointer< SpinGlass >,
               SpinGlass::ExtremityType > _spinGlassAndExtremityType2;

};


struct SpinGlassConnectionCompare /*: 
                               public std::binary_function< SpinGlassConnection,
                                                            SpinGlassConnection,
                                                            bool >
                               deprecated in c++17 */
{

  typedef SpinGlassConnection first_argument_type;
  typedef SpinGlassConnection second_argument_type;
  typedef bool result_type;

  bool operator()( const SpinGlassConnection& s1,
                   const SpinGlassConnection& s2 ) const
  {

    return ( s1.getIdentity() < s2.getIdentity() );

  }

};


}


#endif

