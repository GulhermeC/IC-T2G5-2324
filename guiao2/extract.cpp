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
        cerr << "Color channels: R, G, B or ALL\n";
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

    // Split the image into color channels
    vector<Mat> channels;
    split(image, channels);

    // Select channel
    if(string(argv[2]) == "B" || string(argv[2]) == "b")
    {
        imshow("Blue Channel", channels[0]);
    }
    else if(string(argv[2]) == "G" || string(argv[2]) == "g")
    {
        imshow("Green Channel", channels[1]);
    }
    else if(string(argv[2]) == "R" || string(argv[2]) == "r")
    {
        imshow("Red Channel", channels[2]);
    }
    else if(string(argv[2]) == "ALL" || string(argv[2]) == "all")
    {
        imshow("Blue Channel", channels[0]);
        imshow("Green Channel", channels[1]);
        imshow("Red Channel", channels[2]);
    }
    else
    {
        cerr << "Error: Invalid color channel.";
        return 1;
    }

    t = ((double)getTickCount() - t)/getTickFrequency();
    cout << "Time passed in seconds: " << t << endl;
    waitKey(0);
}