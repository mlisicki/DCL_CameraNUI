DCL_CameraNUI - DisCODe Component Library
=========================================

Description
-----------

Provides communication with Kinect sensor.

Available parameters:

- <lib> can be set to 'freenect' (default) or 'opencv'

- <depth_mode> 'rawMap'(default) - which gives unmodified data and - normalized, disparityMap, dM32f, pointCloud, valid - dM32f states for disparity map 32bit float and valid yields a mask of valid pixels

- <camera_mode> 'rgb' or 'gray'

Example is provided with the code

Dependencies
------------

- OpenKinect/libfreenect with all its dependencies as described here http://openkinect.org/wiki/Getting_Started 

- OpenCv (tested with version 2.3.1) (compiled with WITH_OPENNI option if you want to aquire signal via OpenCv highgui functions)

- Check WITH_OPENNI option in cmake accordingly here

Maintainer
----------

Michal Lisicki
