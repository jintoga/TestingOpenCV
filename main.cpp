#include <QCoreApplication>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>

using namespace cv;
using namespace std;

vector<DMatch> filteringMatches(vector<DMatch> matches,
                      const Mat& descriptor1);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Mat image1, image2;
    image1 = imread("/Users/Dat/Desktop/lena1.jpg");
    image2 = imread("/Users/Dat/Desktop/lena4.jpg");
    if(!image1.data || !image2.data){
        printf("file not found!\n");
        return -1;
    }

    SiftFeatureDetector detector(1600);

    std::vector<KeyPoint> keypoints1, keypoints2;
    detector.detect(image1, keypoints1);
    detector.detect(image2, keypoints2);

    SiftDescriptorExtractor extractor;

    Mat descriptor1, descriptor2;
    extractor.compute(image1, keypoints1, descriptor1);
    extractor.compute(image2, keypoints2, descriptor2);

    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptor1, descriptor2, matches);

    matches = filteringMatches(matches, descriptor1);

    Mat output;
    drawMatches(image1, keypoints1,
                image2, keypoints2,
                matches, output,
                Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    //-- Show detected matches
    imshow( "Output:", output);
    waitKey(0);
    return 0;
}

vector<DMatch> filteringMatches(vector<DMatch> matches, const Mat& descriptor1)
{
    double max_dist = 0; double min_dist = 100;

      //-- Quick calculation of max and min distances between keypoints
      for( int i = 0; i < descriptor1.rows; i++ )
      { double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
      }

      printf("-- Max dist : %f \n", max_dist );
      printf("-- Min dist : %f \n", min_dist );

      //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
      //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
      //-- small)
      //-- PS.- radiusMatch can also be used here.
      std::vector< DMatch > good_matches;

      for( int i = 0; i < descriptor1.rows; i++ )
      { if( matches[i].distance <= max(2*min_dist, 0.02) )
        { good_matches.push_back( matches[i]); }
      }
      return good_matches;
}

