#include <osg/Version>

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


int main(int argc, char *argv[]){
  
  // New group
  
  osg::ref_ptr<osg::Group> root = new osg::Group;

  osg::ref_ptr<osg::StateSet> state = root->getOrCreateStateSet();
  state->setMode( GL_LIGHTING,
    osg::StateAttribute::ON |
    osg::StateAttribute::PROTECTED);
  state->setMode(GL_LIGHT0, osg::StateAttribute::ON);
  state->setMode(GL_LIGHT1, osg::StateAttribute::ON);

  osg::ref_ptr<osg::Light> light1 = new osg::Light;
  light1->setLightNum(0);
  light1->setAmbient(osg::Vec4(.0f, 1.f, .0f,1.f));
  light1->setPosition(osg::Vec4(500,800,1000,1));
  
  osg::ref_ptr<osg::LightSource> ls1 = new osg::LightSource;
  ls1->setLight(light1.get());

  root->addChild(ls1);


  osg::ref_ptr<osg::Light> light2 = new osg::Light;
  light2->setLightNum(1);
  light2->setAmbient(osg::Vec4(.0f, .0f, 1.f,1.f));
  
  osg::ref_ptr<osg::LightSource> ls2 = new osg::LightSource;
  ls2->setLight(light2.get());
  root->addChild(ls2);
  // Line

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


  
  // Add your stuff to the root node here...

  // Set texture to heightfield
  osg::ref_ptr<osg::Texture2D> groundTexture = new osg::Texture2D(osgDB::readImageFile("ground.png"));

  // Set wrapping
  groundTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
  groundTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

  osg::ref_ptr<osg::HeightField> ground = new osg::HeightField;
  
  unsigned int rows = 256;
  unsigned int cols = 256;
  double sizeX = 10.0;
  double sizeY = 10.0;
  osg::Vec3 origin (0,-3,-3);
  
  ground->allocate(rows,cols);
  ground->setXInterval(5.0f);
  ground->setYInterval(5.0f);
  //ground->setHeight(0.5f,0.5f,0.5f);

  for (int i = 0; i < rows-10; ++i) {
    for (int j = 0; j < cols-10; ++j) {
      double ix = double(j)/double(rows-1);
      double iy = double(i)/double(cols-1);

      ground->setHeight(j,i, osg::absolute(50*sin(ix*iy*4*osg::PI)));
    }
  }
/*
  for (int i = rows-10; i < rows; ++i) {
    for (int j = cols-10; j < cols; ++j) {
      double ix = double(j)/double(rows-1);
      double iy = double(i)/double(cols-1);

      ground->setHeight(j,i, osg:)
    }
  }
*/
  osg::ref_ptr<osg::Geode> groundGeode = new osg::Geode();
  groundGeode->addDrawable(new osg::ShapeDrawable(ground));
  osg::ref_ptr<osg::StateSet> groundState = groundGeode->getOrCreateStateSet();
  groundState->setMode(GL_NORMALIZE,osg::StateAttribute::ON);
  groundState->setTextureAttributeAndModes(0, groundTexture);

  // Sphere for reference
  osg::ref_ptr<osg::Geode> sphere = new osg::Geode();
  osg::TessellationHints* hints = new osg::TessellationHints;
  sphere->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),30.0f),hints));

  // Loading Cessna airplane

  osg::ref_ptr<osg::Node> airplane = osgDB::readNodeFile( "cessna.osg" );
  osgUtil::Simplifier simplifier(20);
  airplane->accept(simplifier);
  osg::ref_ptr<osg::PositionAttitudeTransform> airplaneTransform = new osg::PositionAttitudeTransform();
  airplaneTransform->setScale(osg::Vec3(10,10,10));
  airplaneTransform->setPosition(osg::Vec3(500,0,800));
  airplaneTransform->addChild(airplane);

  // Loading Cessna airplane

  osg::ref_ptr<osg::Node> spaceship = osgDB::readNodeFile( "spaceship.osg" );
  osg::ref_ptr<osg::PositionAttitudeTransform> spaceshipTransform = new osg::PositionAttitudeTransform();
  spaceshipTransform->setScale(osg::Vec3(10,10,10));
  spaceshipTransform->setPosition(osg::Vec3(500,800,30));
  spaceshipTransform->addChild(spaceship);

  // Simplifying models


  // Doing LOD-stuff

  osg::ref_ptr<osg::LOD> airplaneLOD = new osg::LOD;
  airplaneLOD->addChild(spaceshipTransform, 0.f, 1000.f);




  // Adding elements to root
  root->addChild(airplaneTransform);
  //root->addChild(spaceshipTransform);
  root->addChild(airplaneLOD);
  root->addChild(sphere);
  root->addChild(groundGeode);

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

  //Setting clear color
  viewer.getCamera()->setClearColor(osg::Vec4(0.6,0.,0.,1.));

  return viewer.run();
}
