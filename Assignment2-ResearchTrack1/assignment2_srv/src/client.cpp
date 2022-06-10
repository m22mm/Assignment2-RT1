#include "ros/ros.h"
#include "assignment2_srv/Velocity.h"
#include <cstdlib>
#include <ostream>
#include <string>
using namespace std;
char velocity_choice;
	
int main(int argc, char **argv) {
	ros::init(argc, argv, "choice_client");	
	while (1){
		ros::NodeHandle n;
		ros::ServiceClient client = n.serviceClient<assignment2_srv::Velocity>("/velocity");
		assignment2_srv::Velocity velocity_srv; 
		cout << "You can control the robot by entering: "<< endl << " U to increase its speed"<<endl<<" D to decrease its speed"<<endl<<" R to reset its position"<<endl<<"	";
		cin>>velocity_choice; // getting the user's choice
		velocity_srv.request.a = velocity_choice;
		if (client.call(velocity_srv)) {
			cout<<endl<<"Velocity = "<< velocity_srv.response.x << endl;
		} 	
		else {
			ROS_ERROR("Failed to call the velocity service");
			return 1;
		}
		
	}
		return 0;
}
