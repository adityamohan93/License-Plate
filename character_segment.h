#ifndef character_segment_h
#define character_segment_h

#include <string.h>
#include <vector>

#include "Plate.h"

#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

using namespace std;
using namespace cv;


#define HORIZONTAL    1
#define VERTICAL    0

class Segment{
public:
    Segment();
    Segment(Mat i, Rect p);
    Mat img;
    Rect pos;
};

class character_segment{
    public:
        bool DEBUG;
        bool saveSegments;
        string filename;
        static const int numCharacters;
        Mat preprocessChar(Mat in);
        static const char strCharacters[];
        character_segment();
        string return_segmented_characters(Plate *input);
        int charSize;
    private:
        vector<Segment> segment(Plate input);
        bool validate(Mat r);
        void train();
        double br[37][25][25];
        char classify(Mat);
};

#endif

