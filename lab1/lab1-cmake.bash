
mkdir build
cd build
cmake \
 -D OSGUTIL_LIBRARY=/opt/liu/openscenegraph/3.6.5/lib/libosgUtil.so \
 -D OSGUTIL_INCLUDE_DIR=/opt/liu/openscenegraph/3.6.5/include   \
 -D OSGDB_LIBRARY=/opt/liu/openscenegraph/3.6.5/lib/libosgDB.so \
 -D OSGDB_INCLUDE_DIR=/opt/liu/openscenegraph/3.6.5/include   \
 -D OSGGA_LIBRARY=/opt/liu/openscenegraph/3.6.5/lib/libosgGA.so \
 -D OSGGA_INCLUDE_DIR=/opt/liu/openscenegraph/3.6.5/include   \
 -D OSGVIEWER_LIBRARY=/opt/liu/openscenegraph/3.6.5/lib/libosgViewer.so \
 -D OSGVIEWER_INCLUDE_DIR=/opt/liu/openscenegraph/3.6.5/include   \
 -D OSGWIDGET_LIBRARY=/opt/liu/openscenegraph/3.6.5/lib/libosgWidget.so \
 -D OSGWIDGET_INCLUDE_DIR=/opt/liu/openscenegraph/3.6.5/include   \
 -D OSG_LIBRARY=/opt/liu/openscenegraph/3.6.5/lib/libosg.so \
 -D OSG_INCLUDE_DIR=/opt/liu/openscenegraph/3.6.5/include   \
 -D OPENTHREADS_LIBRARY=/opt/liu/openscenegraph/3.6.5/lib/libOpenThreads.so \
 -D OPENTHREADS_INCLUDE_DIR=/opt/liu/openscenegraph/3.6.5/include \
 ..

