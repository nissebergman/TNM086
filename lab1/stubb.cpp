#include <osg/Version>


#include <osgUtil/Optimizer>
#include <osg/Node>
#include <osg/Program>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgUtil/Optimizer>
#include <osg/LOD>
#include <osgUtil/Simplifier>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osg/StateAttribute>
#include <osg/Math>
#include <osg/CopyOp>

int main(int argc, char *argv[]){
    
  // New group
  
  osg::ref_ptr<osg::Group> root = new osg::Group;

  
  osg::ref_ptr<osg::Light> light1 = new osg::Light;
  light1->setLightNum(0);
  light1->setAmbient(osg::Vec4(.0f, 1.f, .0f,1.f));
  light1->setPosition(osg::Vec4(500,800,1000,1));
  
  // Line

  /*
  osg::Vec3 line_p0 (-300, 0, 0);
  osg::Vec3 line_p1 ( 300, 0, 0);
  
  osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
  vertices->push_back(line_p0);
  vertices->push_back(line_p1);
  
  osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
  colors->push_back(osg::Vec4(0.9f,0.2f,0.3f,1.0f));

  osg::ref_ptr<osg::Geometry> linesGeom = new osg::Geometry();
  linesGeom->setVertexArray(vertices);
  linesGeom->setColorArray(colors, osg::Array::BIND_OVERALL);
  
  linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
  
  osg::ref_ptr<osg::Geode> lineGeode = new osg::Geode();
  lineGeode->addDrawable(linesGeom);
  lineGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
  
  root->addChild(lineGeode);
*/

  
  // Add your stuff to the root node here...


  //Create heightfield
  osg::ref_ptr<osg::HeightField> ground = new osg::HeightField(); 
  //Allocate and set interval
  ground->allocate(256,256);
  ground->setXInterval(5.0f);
  ground->setYInterval(5.0f);

  //simple ground
  for(int x = 0; x < ground->getNumRows(); x++) {
    for(int y = 0; y < ground->getNumColumns(); y++) {
        ground->setHeight(x,y, cos(x)+sin(y));
    }
  }
  osg::ref_ptr<osg::Geode> geoGround = new osg::Geode();
  geoGround->addDrawable(new osg::ShapeDrawable(ground));
  root->addChild(geoGround);

  // Loading Cessna airplane

  osg::ref_ptr<osg::Node> airplane = osgDB::readNodeFile( "cessna.osg" );
  osg::ref_ptr<osg::PositionAttitudeTransform> airplaneTransform = new osg::PositionAttitudeTransform();
  airplaneTransform->setScale(osg::Vec3(10,10,10));
  airplaneTransform->setPosition(osg::Vec3(500,0,800));
  airplaneTransform->addChild(airplane);

  // Loading spaceship

  osg::ref_ptr<osg::Node> spaceship = osgDB::readNodeFile( "spaceship.osg" );
  osg::ref_ptr<osg::Node> spaceshipLowerQual = (osg::Node*)spaceship->clone(osg::CopyOp::DEEP_COPY_ALL);
  osgUtil::Simplifier simplify;
  simplify.setSampleRatio(0.5f);
  spaceshipLowerQual->accept(simplify);
  osg::ref_ptr<osg::Node> spaceshipLowestQual = (osg::Node*)spaceship->clone(osg::CopyOp::DEEP_COPY_ALL);
  simplify.setSampleRatio(.01f);
  spaceshipLowestQual->accept(simplify);

  // Doing LOD-stuff
  osg::ref_ptr<osg::LOD> spaceshipLOD = new osg::LOD;
  spaceshipLOD->addChild(spaceship, 0.f, 100.f); //Full quality model
  spaceshipLOD->addChild(spaceshipLowerQual, 100.f, 500.f);
  spaceshipLOD->addChild(spaceshipLowestQual,500.f,10000.f);


  osg::ref_ptr<osg::PositionAttitudeTransform> spaceshipTransform = new osg::PositionAttitudeTransform();
  spaceshipTransform->setScale(osg::Vec3(10,10,10));
  spaceshipTransform->setPosition(osg::Vec3(500,100,100));
  spaceshipTransform->addChild(spaceshipLOD);

  // Simplifying models





  // Optimizes the scene-graph
  //osgUtil::Optimizer optimizer;
  //optimizer.optimize(root);
  
  // Set up the viewer and add the scene-graph root
  osgViewer::Viewer viewer;
  viewer.setSceneData(root);
 

  osg::ref_ptr<osg::Camera> camera = new osg::Camera;
  camera->setProjectionMatrixAsPerspective(60.0, 1.0, 0.1, 100.0);
  camera->setViewMatrixAsLookAt (osg::Vec3d(0.0, 0.0, 2.0),
                                 osg::Vec3d(0.0, 0.0, 0.0),
                                 osg::Vec3d(0.0, 1.0, 0.0));
  camera->getOrCreateStateSet()->setGlobalDefaults();

  viewer.setCamera(camera);
  
  // Optimizes the scene-graph
  osgUtil::Optimizer optimizer;
  optimizer.optimize(root);

  //Setting clear color
  viewer.getCamera()->setClearColor(osg::Vec4(0.6,0.,0.,1.));
  
  

  return viewer.run();
}
