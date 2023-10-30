#include <iostream>
#include <sstream> 
#include <opencv2/opencv.hpp> 

using namespace std;
using namespace cv;

int
main (int argc, char *argv[])
{
    // input
    if(argc < 2)
    {
        cerr << "Usage: " << argv[0] << "<input image> <color channel>\n";
        cerr << "Color channels: 0 (Blue), 1 (Green), 2 (Red), R, G, B or ALL\n";
        return 1;
    }

    double t = (double)getTickCount();
    
    // Read image
    Mat image = imread(argv[1], IMREAD_COLOR);
    if(image.empty())
    {
        cerr << "Error: Could not open or find the image.\n";
        return 1;
    }

    Mat blueChannel(image.rows, image.cols, CV_8U);
    Mat greenChannel(image.rows, image.cols, CV_8U);
    Mat redChannel(image.rows, image.cols, CV_8U);

    // Extract pixel-by-pixel
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            Vec3b pixel = image.at<Vec3b>(i, j);
            blueChannel.at<uchar>(i, j) = pixel[0];
            greenChannel.at<uchar>(i, j) = pixel[1];
            redChannel.at<uchar>(i, j) = pixel[2];
        }
    }

    // Select channel
    string channelInput = argv[2];
    if(channelInput == "0" || channelInput == "B" || channelInput == "b")
    {
        imshow("Blue Channel", blueChannel);
    }
    else if(channelInput == "1" || channelInput == "G" || channelInput == "g")
    {
        imshow("Green Channel", greenChannel);
    }
    else if(channelInput == "2" || channelInput == "R" || channelInput == "r")
    {
        imshow("Red Channel", redChannel);
    }
    else if(channelInput == "ALL" || channelInput == "all")
    {
        imshow("Blue Channel", blueChannel);
        imshow("Green Channel", greenChannel);
        imshow("Red Channel", redChannel);
    }
    else
    {
        cerr << "Error: Invalid color channel.";
        return 1;
    }

    t = ((double)getTickCount() - t)/getTickFrequency();
    cout << "Time elapsed: " << t << endl;
    waitKey(0);

    return 0;
}