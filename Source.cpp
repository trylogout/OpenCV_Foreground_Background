#include<opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <string>
#include <fstream>

using namespace std;
using namespace cv;

inline bool exists_test(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

int main(int argc, char* argv[])
{
    bool show = false;
	VideoCapture cap;
    if (exists_test("C:\\repos\\PlayGround\\PlayGround\\small.mp4")) {
        printf("FILE EXIST");
        cap.open("C:\\case.mkv");
    }
    
    //init MOG2 BackgroundSubstractor
    Ptr<BackgroundSubtractor> BackgroundSubstractor;
    BackgroundSubstractor = createBackgroundSubtractorMOG2().dynamicCast<BackgroundSubtractor>();;

    //make simple frame and MOG2 mask for frame
    Mat frame, MOGMask, foregroundImg, backgroundImage, BackgroundImg;

    int i_count = 0;
    while (true) {
        i_count++;
        cap >> frame;
        if (frame.empty())
            break;

        //update the background model
        BackgroundSubstractor->apply(frame, MOGMask);

        GaussianBlur(MOGMask, MOGMask, Size(11, 11), 3.5, 3.5);
        threshold(MOGMask, MOGMask, 10, 255, THRESH_BINARY);

        //get the frame number and write it on the current frame
        rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
            cv::Scalar(255, 255, 255), -1);

        stringstream ss;
        ss << cap.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();

        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

        foregroundImg = Scalar::all(0);
        BackgroundImg = Scalar::all(0);
        frame.copyTo(foregroundImg, MOGMask);

        backgroundImage = 255 - MOGMask;
        frame.copyTo(BackgroundImg, backgroundImage);

        if (show){
            imshow("Frame", frame);
            imshow("BackGround", BackgroundImg);
            imshow("ForeGround", foregroundImg);
            imshow("MOG2 Mask", MOGMask);
        }


        if (i_count != 1) {
            std::string counter = std::to_string(i_count);

            imwrite("C:/images/original/Frame" + counter + ".jpg", frame);
            imwrite("C:/images/background/Frame" + counter + ".jpg", BackgroundImg);
            imwrite("C:/images/foreground/Frame" + counter + ".jpg", foregroundImg);
        }

        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }

	return 0;
}