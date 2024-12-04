#ifndef _gkg_anatomist_plugin_objects_ABundleMapObject_h_
#define _gkg_anatomist_plugin_objects_ABundleMapObject_h_


#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <anatomist/object/oReader.h>
#include <graph/tree/tree.h>

#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-processing-colorimetry/RGBComponent.h>

#include <gkg-anatomist-plugin-rendering-options/ABundleMapRenderingOptionsWidget.h>

#include <vector>
#include <string>

#include <QObject>
#include <QtOpenGL>


namespace gkg
{


class ABundleMapReader;
// Assuming the structure for fiber information
struct showFibers {
    const gkg::LightCurve3d<float>* fiberPointer;
    const gkg::RGBComponent* color;
    std::string bundleLabel;
};

class ABundleMapObject : public QObject,
                         public anatomist::SliceableObject
{
  public:

    friend class AnaGkgModule;

    ABundleMapObject(
                  const RCPointer< BundleMap< std::string > >& bundleMap,
                  const RCPointer< std::vector< RGBComponent > >& bundleColors);
    virtual ~ABundleMapObject();

    int maxThreads;
    int fiberThreads;
    int bundleThreads;
    int nbbundleCount;
    std::map<std::string, gkg::RGBComponent> _bundleColorMap;
    
    //Vectorisation Fibre visible
    std::vector<gkg::showFibers> processFibersShowVector() const;
    //Fonction pour le streamlise visualisation
    void streamizeVisualisation() const;
    ////////////////////////////////////////////////////////////////////////////
    // Fonction Couleur pour la visualisation
    ////////////////////////////////////////////////////////////////////////////
    
   
    //Cree une map de couleur en fonction des labels
    std::map<std::string, gkg::RGBComponent> createBundleColorMap();
//processus pour associé au vecteur de fibre une couleur en fonction des bundles
    std::vector<gkg::showFibers> processBundlesColorVisualisation(
                              std::vector<gkg::showFibers> visibleFibers) const;
    gkg::Vector3d<float> processLocalDirectionColor(
          const gkg::Vector3d<float>& p1, const gkg::Vector3d<float>& p2) const;
    gkg::Vector3d<float> processScalarFeatureVolumeColor(float cPrime) const;
    gkg::Vector3d<float> processConstantMaterialColor() const;


    bool Is2DObject();
    bool Is3DObject();

    // tree menu specific to the object
    Tree* optionTree() const;

    // bounding box information
    float MinX2D() const;
    float MinY2D() const;
    float MinZ2D() const;
    float MaxX2D() const;
    float MaxY2D() const;
    float MaxZ2D() const;

    bool boundingBox( std::vector< float > & bmin,
                      std::vector< float > & bmax ) const;

    int32_t getFiberSubSamplingFactor() const;
    void setFiberSubSamplingFactor( int32_t value );

    float getFiberLineWidth() const;
    void setFiberLineWidth( float value );

    int32_t getFiberColorEncoding() const;
    const std::string& getFileNameScalarFeatureVolume() const;
    const std::string& 
                      getFileNameScalarFeatureVolumeToBundleTransform3d() const;
    double getLowerBound() const;
    double getUpperBound() const;

    void setFiberColorEncoding(
              int32_t colorEncodingType,
              const std::string& fileNameScalarFeatureVolume,
              const std::string& fileNameScalarFeatureVolumeToBundleTransform3d,
              double lowerBound,
              double upperBound );

    bool render( anatomist::PrimList& primitiveList,
                 const anatomist::ViewState& viewState );
    bool glMakeBodyGLL( const anatomist::ViewState& viewState,
                        const anatomist::GLList& glList ) const;
    
    void drawBundleColorsVisualisation() const;
    void drawFibersDirectionVisualisation() const;
    void drawFibersUniformColorVisualisation() const;
    
    // static methods
    static std::list< anatomist::AObject* > load( 
                             const std::string& fileName,
                             anatomist::ObjectReader::PostRegisterList& regList,
                             carto::Object options );
    static int32_t registerClass();
    static int32_t getClassType();

    // static public attributes
    static int32_t _classType;
    static Tree* _optionTree;
      
    // Setter pour les trois variables
    void setSplitLabels(std::map<std::string, bool>& newRightLabels,
                   std::map<std::string, bool>& newLeftLabels,
                   std::map<std::string, bool>& newInterLabels,
                   std::map<std::string, bool>& newEveryLabels);
     // Getter pour rightLabels
    std::map<std::string, bool>& getRightLabels_states();
    // Getter pour leftLabels
    std::map<std::string, bool>& getLeftLabels_states();
    // Getter pour otherLabels
    std::map<std::string, bool>& getInterLabels_states();
    
    std::map<std::string, bool>& getEveryLabels_states();
    bool getClassificationLabel_states();
    void printCheckBoxStates();
    // Get show or not bundle
    bool getBundleVisibility(std::string Key) const;
    // Setter pour la liste de labels
    void setBundleLabels(const std::set<std::string>& newLabels);
    // Getter pour la liste de labels
    const std::set<std::string>& getBundleLabels() const;


  private:

    bool classificationLabel;
    std::set<std::string> bundleLabels;
    mutable std::map<std::string, bool> rightLabels_states;
    mutable std::map<std::string, bool> leftLabels_states;
    mutable std::map<std::string, bool> interLabels_states;
    mutable std::map<std::string, bool> everyLabels_states;

    
  protected:



    void computeScalarFeatureVolumeBasedColors();

    RCPointer< BundleMap< std::string > > _bundleMap;
    RCPointer< std::vector< RGBComponent > > _bundleColors;
    BoundingBox< float > _boundingBox;

    int32_t _fiberSubSamplingFactor;
    float _fiberLineWidth;
    int32_t _fiberColorEncoding;
    std::string _fileNameScalarFeatureVolume;
    std::string _fileNameScalarFeatureVolumeToBundleTransform3d;
    double _lowerBound;
    double _upperBound;

    RCPointer< std::vector< float > > _scalarFeatureVolumeBasedColors;

};


}


#endif
