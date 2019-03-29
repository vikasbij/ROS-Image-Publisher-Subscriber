#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "opencv2/videoio.hpp"

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    cv::Mat src = cv_bridge::toCvShare(msg, "bgr8")->image;
    cv::Mat dst;
    cv::Point2f pc(src.cols/2., src.rows/2.);
    cv::Mat r = cv::getRotationMatrix2D(pc, -180, 1.0);
    cv::warpAffine(src, dst, r, src.size());
    cv::imshow("view", dst); // displaying image rotated by 180 degrees.
    //cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image); //- simply show/display publisher node's data
    ros::Timer();
    //cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
 }

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
  ros::spin();
  cv::destroyWindow("view");
}   
