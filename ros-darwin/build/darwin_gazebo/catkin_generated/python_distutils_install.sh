#!/bin/sh -x

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

cd "/home/d/ros-darwin/src/darwin_gazebo"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
/usr/bin/env \
    PYTHONPATH="/home/d/ros-darwin/install/lib/python2.7/dist-packages:/home/d/ros-darwin/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/d/ros-darwin/build" \
    "/usr/bin/python" \
    "/home/d/ros-darwin/src/darwin_gazebo/setup.py" \
    build --build-base "/home/d/ros-darwin/build/darwin_gazebo" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/d/ros-darwin/install" --install-scripts="/home/d/ros-darwin/install/bin"
