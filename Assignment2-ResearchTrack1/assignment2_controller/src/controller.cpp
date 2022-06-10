#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "assignment2_srv/Velocity.h"
#include "std_srvs/Empty.h"
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

ros::ServiceClient client1;// defining the client as global
ros::Publisher publisher; // defining the publisher as global, pub is of type ros::Publisher (namespace ros)
ros::Subscriber subscriber;// defining the subscriber as global

float front_obstacle, frontLeft_obstacle, frontRight_obstacle;
float speed=0.5;

bool new_vel (assignment2_srv::Velocity::Request &req, assignment2_srv::Velocity::Response &res){
		assignment2_srv::Velocity velocity_srv;
		if (req.a == "U" || req.a == "u"){
			res.x = speed + 0.2*speed;	
		}
		else if (req.a == "D" || req.a == "d"){
			res.x = speed - 0.2*speed;
		}
		else if (req.a == "R" || req.a == "r"){
			ros::NodeHandle nh;
			ros::ServiceClient client1 = nh.serviceClient<std_srvs::Empty>("/reset_positions");
			std_srvs::Empty empty_srv;
 			client1.waitForExistence();
			client1.call(empty_srv);
		}
		if (res.x > 1.3 || res.x < 0.3){ // safety
			cout << endl << "Velocity exceeded the safety limit by being too large or too small! Resetting the speed to the initial value..." << endl;
			res.x=0.5;
		}
		speed=res.x;
		return res.x;
}

void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg) {
// The obstacles ranges will decomposed into 5 ranges as follows: 720/5 = 144 
	for (int i=145;i<288;i++) // front right
	{
		frontRight_obstacle=msg-> ranges[i];
		if (frontRight_obstacle > msg-> ranges[i])
		frontRight_obstacle = msg-> ranges[i];
	}
	for (int i=289;i<432;i++) // front 
	{
		front_obstacle=msg-> ranges[i];
		if (front_obstacle > msg-> ranges[i])
		front_obstacle = msg-> ranges[i];
	}
	for (int i=433;i<576;i++) // front left
	{
		frontLeft_obstacle=msg-> ranges[i];
		if (frontLeft_obstacle > msg-> ranges[i])
		frontLeft_obstacle = msg-> ranges[i];
	}

	geometry_msgs::Twist vel;
	vel.linear.x = speed;
	vel.angular.z = 0.0;
	// Checking the obstacles' distances and acting accordingly to avoid them
	if (frontLeft_obstacle > 1.2 && front_obstacle > 1.2 && frontRight_obstacle > 1.2){
		vel.linear.x = speed;
		vel.angular.z = 0.0;
	} 
	else if (frontLeft_obstacle > 1.2 && front_obstacle< 1.2 && frontRight_obstacle > 1.2){
		vel.linear.x = speed;
		vel.angular.z = 1.5;
	}
	else if (frontLeft_obstacle < 1.2 && front_obstacle < 1.2 && frontRight_obstacle > 1.2 ){
		vel.linear.x = speed;
		vel.angular.z =-1.5;
	}
	else if (frontLeft_obstacle < 1.2 && front_obstacle < 1.2 && frontRight_obstacle < 1.2){
		vel.linear.x = speed;
		vel.angular.z =1.5;
	}
	else if ( frontLeft_obstacle < 1.2 && front_obstacle > 1.2 && frontRight_obstacle <1.2){
		vel.linear.x = speed;
		vel.angular.z =0.0;
	}
	else if (frontLeft_obstacle < 1.2 && front_obstacle > 1.2 && frontRight_obstacle > 1.2){
		vel.linear.x = speed;
		vel.angular.z =-1.5;
	}
	else if (frontLeft_obstacle > 1.2 && front_obstacle > 1.2 && frontRight_obstacle < 1.2){
		vel.linear.x = speed;
		vel.angular.z = 1.5;
	}
	else if (frontLeft_obstacle > 1.2 && front_obstacle < 1.2 && frontRight_obstacle < 1.2){
		vel.linear.x = speed;
		vel.angular.z =1.5;
	}
	else{
		vel.linear.x = speed;
		vel.angular.z =0.0;
	}
	publisher.publish(vel);
}

	int main (int argc, char **argv) {
// Initialize the node, setup the NodeHandle for handling the communication with the ROS
//system
	ros::init(argc, argv, "stage_subscriber");
	ros::NodeHandle nh;
	ros::NodeHandle n;
	ros::ServiceServer service= n.advertiseService("/velocity", new_vel);
	publisher = nh.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);
	ros::Subscriber subscriber = nh.subscribe("/base_scan", 1, laserCallback);
	ros::spin();
	return 0;
}
