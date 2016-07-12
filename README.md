# License-Plate-Recognition-for-Traffic-Light-Violation

A stand-alone application based on digital image processing to detect license plate from a raw image input directly from live camera feed which included subject, usually a car or motorcycle. It also segments the various characters detected and identifies them using Optical Character Recognition.

How to run?

export OPENCV_DIR="<folder path>"
cmake -D OpenCV_DIR=$OpenCV_DIR .
make

