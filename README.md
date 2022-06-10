# Assignment 2 - Research Track 2
---------------------------------
A robot driving in a race track where it avoids collapsing into the boundaries while the user can change its speed
## Installating and Running
---------------------------
This project simulation requires a ROS installation.
After the project is installed or cloned, the workspace should be built by running the following command in the root of the workspace:
```
$ catkin_make
```
After a successful build process, the project can be run by the following steps in the command line:

```
$ roscore
```
```
$ rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
```
```
$ rosrun assignment2_controller assignment2_controller_node
```
```
$ rosrun assignment2_srv client
```
In case the any of the above packages were not found by the system, apply the following sourcing command:
```
source devel/setup.bash
```
## Robot Motion Flowchart
-------------------------
![Assignment 2 - Flowchart](https://user-images.githubusercontent.com/79665691/173125089-fac2f92f-72bd-4178-81b3-f103ce540f9d.png)

The above flowchart describes the logic based on which the robot motion algorithm is implemented to be able to navigate while avoiding the boundaries' obstacles.\
First, the robot will drive forward while checking for the obstacles' distances as it receives the laserscan output and divides it into 5 ranges, while considering the front left, front and front right directions. After detecting one or more boundaries with a distance less than the specified threshold, it will act accordingly to avoid it by changing its orientation while maintaining its speed.

## Topics Used
--------------
The main topic used for the considered simulation is the base_Scan topic, which provides the robot with obstacles distances in a wide reach of 180 degrees. These 720 provided ranges are further divided into 5 directions from which the main 3 front directions are considered. Topics are used for this purpose as they consist of a publish/subscrive method for unidirectional communcation purposes. 

## Services Used
----------------
Furthermore, the user can increase or decrease the robot's speed and even reset the robot position respectively through the "U", "D" or "R" inputs. This user choice feature is implementend via a service communication. Moreover, the robot has safety upper and lower limits, which upon reaching them, the speed will be re-initialized to the first value. Services presents an additional communcation type enabling data transfer between nodes in ROS through synchronous remote calls as they allow one node to call a function that executes in another node. In order to create a new service, the first step consists of defining the service call inputs and outputs in the  custom service file 'Velocity.srv'. The next step is the implementation of the server part, which in this project is included in the controller code and which specifies a callback function that will be triggered when the service call occurs, afterwards, it waits to receive the requests. Considering the client part, whenever the service is called, the user will be asked through the terminal to enter his command choice in order for this request to be sent to the server controlling the robot's speed.

## Possible Improvements
------------------------
For the optimization purposes of the race line that the robot should follow to cover the track with the least possible time, genetic algorithms can provide candidate solutions. First, genetic algorithms are methods aiming to solve constrained and unconstrained optimization problems based on natural selection. The genetic algorithm repeatedly changes a population of individual solutions until reaching the optimized one. For this application, the race track should be decomposed into several segments as the genetic algorithm should be applied to search for the best trade-off between the minimization of the length and the curvature of the race line.
