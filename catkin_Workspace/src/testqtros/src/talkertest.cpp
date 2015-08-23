#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"
#include <sstream>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc,argv,"talkerT1");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<std_msgs::Int32MultiArray>("chatter",1000);
	ros::Rate loop_rate(2);

	int count=1;
        std::vector<int> a(10,3333);
        //std::vector<int> a={1111,2222,3333,4444,5555};
	while(ros::ok())
	{
		std_msgs::Int32MultiArray msg;
		//a[0]=a[0]+count;
		//a[4]=a[4]+count+1;
		msg.data = a;
		
		ROS_INFO("%d %d %d %d %d", msg.data[0],msg.data[1],msg.data[2],msg.data[3],msg.data[4]);

		chatter_pub.publish(msg);
		loop_rate.sleep();
		//++count;
        //cout<<"Hello!"<<endl;
	}
	return 0;
}
		

