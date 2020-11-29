#include <osg/Version>
#include <iostream>
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
#include <osg/AnimationPath>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/LineSegmentIntersector>

osg::ref_ptr<osg::LightSource> ls1 = new osg::LightSource();
osg::ref_ptr<osg::LightSource> ls2 = new osg::LightSource();


class IntersectionCallback : public osg::NodeCallback {
  public:
    virtual void operator() ( osg::Node* node, osg::NodeVisitor* nodeVisit ) {

        // Intersection line, same coords as rendered line
        osg::Vec3 line_p0 (-300, -50, 0);
        osg::Vec3 line_p1 ( 300, -50, 0);

        osg::ref_ptr<osgUtil::LineSegmentIntersector> lineIntersector = 
          new osgUtil::LineSegmentIntersector(line_p0, line_p1);
          osgUtil::IntersectionVisitor visitor (lineIntersector.get());
          node->accept(visitor);

          if(lineIntersector->containsIntersections()) {
            ls1->getLight()->setAmbient( osg::Vec4(1.f, 0.f, 0.f, 1) ); 
            ls2->getLight()->setAmbient( osg::Vec4(0.f, 1.f, 0.f, 1) );
            //std::cout << "Intersect!";
          }
          else{
            ls1->getLight()->setAmbient( osg::Vec4(0.f, 1.f, 0.f, 1) ); 
            ls2->getLight()->setAmbient( osg::Vec4(1.f, 0.f, 0.f, 1) );
            //std::cout << "No intersect!";
          }
          traverse(node, nodeVisit);
    }
};


int main(int argc, char *argv[]){
  
  // New group
  
  osg::ref_ptr<osg::Group> root = new osg::Group;

  osg::ref_ptr<osg::StateSet> state = root->getOrCreateStateSet();
  state->setMode( GL_LIGHTING,
    osg::StateAttribute::ON |
    osg::StateAttribute::PROTECTED);
  state->setMode(GL_LIGHT0, osg::StateAttribute::ON);
  state->setMode(GL_LIGHT1, osg::StateAttribute::ON);
  state->setMode(GL_LIGHT3, osg::StateAttribute::ON);

  // Spaceship spotlight
  osg::ref_ptr<osg::Light> light1 = new osg::Light;
  light1->setLightNum(0);
  light1->setPosition(osg::Vec4(500.f,100.f,220.f,1.f));
  light1->setSpotCutoff(28.f);
  light1->setSpotExponent(5.f);
  light1->setDirection(osg::Vec3(0.f, 0.f, -1.f));
  // Spaceship lightsphere
  osg::ref_ptr<osg::Geode> greenSphereGeode = new osg::Geode();
  greenSphereGeode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(500.f,100.f,200.f),5.0f)));
  root->addChild(greenSphereGeode);
  ls1->setLight(light1.get());
  root->addChild(ls1);

  // Cessna spotlight
  osg::ref_ptr<osg::Light> light2 = new osg::Light;
  light2->setLightNum(1);
  light2->setPosition(osg::Vec4(0.f,0.f,220.f,1.f));
  light2->setSpotCutoff(28.f);
  light2->setSpotExponent(5.f);
  light2->setDirection(osg::Vec3(0.f, 0.f, -1.f));
  // Cessna lightsphere
  osg::ref_ptr<osg::Geode> redSphereGeode = new osg::Geode();
  redSphereGeode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.f,0.f,200.f),5.0f)));
  root->addChild(redSphereGeode);
  ls2->setLight(light2.get());
  root->addChild(ls2);

  // Add ambient light to scene
  osg::ref_ptr<osg::Light> light3 = new osg::Light;
  light3->setLightNum(3);
  light3->setAmbient(osg::Vec4(1.f,1.f,1.f,1.f));
  light3->setPosition(osg::Vec4(500.f,300.f,500.f,1.f));
  light3->setLinearAttenuation(1.0f/128.0f);
  osg::ref_ptr<osg::LightSource> ls3 = new osg::LightSource;
  ls3->setLight(light3.get());
  root->addChild(ls3);
  
  
  // Intersection Line
  osg::Vec3 line_p0 (-300, -50, 0);
  osg::Vec3 line_p1 ( 300, -50, 0);
  
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

  root->setUpdateCallback(new IntersectionCallback);

  // Add your stuff to the root node here...

  // Set texture to heightfield
  osg::ref_ptr<osg::Texture2D> groundTexture = new osg::Texture2D(osgDB::readImageFile("ground.png"));

  // Set wrapping
  groundTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
  groundTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

  osg::HeightField *ground = new osg::HeightField;
  
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

  osg::ref_ptr<osg::Geode> groundGeode = new osg::Geode();
  groundGeode->addDrawable(new osg::ShapeDrawable(ground));
  osg::ref_ptr<osg::StateSet> groundState = groundGeode->getOrCreateStateSet();
  groundState->setMode(GL_NORMALIZE,osg::StateAttribute::ON);
  groundState->setTextureAttributeAndModes(0, groundTexture);

  // Loading Cessna airplane, give animation
  osg::ref_ptr<osg::Node> airplane = osgDB::readNodeFile( "cessna.osg" );
  osg::ref_ptr<osg::PositionAttitudeTransform> airplaneTransform = new osg::PositionAttitudeTransform();  
  airplaneTransform->addChild(airplane);

  osg::ref_ptr<osg::AnimationPath> planeAnimation = new osg::AnimationPath();
  osg::AnimationPath::ControlPoint animationP1 (osg::Vec3(0,0,100));
  animationP1.setScale(osg::Vec3(10,10,10));
  osg::AnimationPath::ControlPoint animationP2 (osg::Vec3(0,0,-100));
  animationP2.setScale(osg::Vec3(10,10,10));
  planeAnimation->insert(0.0f,animationP1);
  planeAnimation->insert(1.0f,animationP2);
  planeAnimation->setLoopMode(osg::AnimationPath::SWING);
  osg::ref_ptr<osg::AnimationPathCallback> planeAnimationCallback = new osg::AnimationPathCallback(planeAnimation);
  airplaneTransform->setUpdateCallback(planeAnimationCallback); //<- Animation

  // Loading spaceship
  osg::ref_ptr<osg::Node> spaceship = osgDB::readNodeFile( "spaceship.osg" );
  osg::ref_ptr<osg::Node> spaceshipLowerQual = (osg::Node*)spaceship->clone(osg::CopyOp::DEEP_COPY_ALL);
  osgUtil::Simplifier simplify;
  simplify.setSampleRatio(2.f);
  spaceshipLowerQual->accept(simplify);
  osg::ref_ptr<osg::Node> spaceshipLowestQual = (osg::Node*)spaceship->clone(osg::CopyOp::DEEP_COPY_ALL);
  simplify.setSampleRatio(.01f);
  spaceshipLowestQual->accept(simplify);

  // Doing LOD-stuff
  osg::ref_ptr<osg::LOD> spaceshipLOD = new osg::LOD;
  spaceshipLOD->addChild(spaceship, 0.f, 60.f); //Full quality model
  spaceshipLOD->addChild(spaceshipLowerQual, 60.f, 100.f);
  spaceshipLOD->addChild(spaceshipLowestQual, 100.f,10000.f);

  osg::ref_ptr<osg::PositionAttitudeTransform> spaceshipTransform = new osg::PositionAttitudeTransform();
  spaceshipTransform->setScale(osg::Vec3(10,10,10));
  spaceshipTransform->setPosition(osg::Vec3(500,100,100));
  spaceshipTransform->addChild(spaceshipLOD);

  // Adding elements to root
  root->addChild(airplaneTransform);
  root->addChild(spaceshipTransform);
  root->addChild(groundGeode);

  // Optimizes the scene-graph
  osgUtil::Optimizer optimizer;
  optimizer.optimize(root);
  
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

  //Setting clear color, debug reasons
  //viewer.getCamera()->setClearColor(osg::Vec4(0.6,0.,0.,1.));

  return viewer.run();
}
