#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream>
#include "opencv2/videoio.hpp"

int main (int argc, char** argv)
{
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  
  image_transport::ImageTransport it(nh);
  
  image_transport::Publisher pub = it.advertise("camera/image", 1);
  
  cv::VideoCapture cap(CV_CAP_ANY);
  ros::Rate loop_rate(5); 

  cv::namedWindow("WebCamera",CV_WINDOW_AUTOSIZE);
  while (nh.ok()) 
  {
    cv::Mat frame;
    cap >> frame;
    cv::imshow("WebCamera", frame);
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg(); 
    pub.publish(msg);
    ROS_INFO("Publishing to node ");

    if (cv::waitKey(30) == 27) 
	{
	  std::cout << "Closing the program because esc pressed";
	  break;
        }
    ros::spinOnce();
    loop_rate.sleep();
  } 

  
}
