/*
 * univr_turtle_node.cpp
 *
 * This file is part of univr_turtle and it is distributed under the terms of the
 * GNU Lesser General Public License (Lesser GPL)
 *
 * univr_turtle is included in the material of the course (in italian)
 * Laboratorio Ciberfisico
 * Robot Programming with ROS
 * A.Y. 2017/2018
 * University of Verona (Italy)
 * http://profs.scienze.univr.it/~bloisi/corsi/ciberfisico.html
 *
 * univr_turtle is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with hello_ros.
 * If not, see http://www.gnu.org/licenses/.
 * 
 * The files in hello_ros contain the ROS based implementation for a simple publisher/subscriber
 * mechanism.
 *
 * see this file for additional information (in italian)
 *
 * Please, report suggestions/comments/bugs to
 * domenico.bloisi@gmail.com
 *
 */

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"

const float BASE_LIN_VEL = 0.5, BASE_ANG_VEL = 0.2;

float lin_vel_ = BASE_LIN_VEL;
float ang_vel_ = BASE_ANG_VEL;

ros::WallTime last_command_time_;

// Topic messages callback
void poseCallback(const turtlesim::PoseConstPtr& msg)
{
    //ROS_INFO("x: %.2f, y: %.2f", msg->x, msg->y);
}

void velocityCallback(const geometry_msgs::Twist::ConstPtr& vel)
{
  last_command_time_ = ros::WallTime::now();
  lin_vel_ = vel->linear.x;
  ang_vel_ = vel->angular.z;

  ROS_INFO("keyboard: lin_vel: %.2f, ang_vel: %.2f", lin_vel_, ang_vel_);
}
 
int main(int argc, char **argv)
{ 
    // initialization
    ros::init(argc, argv, "univr_turtle");
    ros::NodeHandle node;
 
    // A publisher for the movement data
    ros::Publisher pub = node.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 10);
 
    // A listener for pose
    ros::Subscriber sub = node.subscribe("turtle1/pose", 10, poseCallback);

    // A listener for left, right, up, and down keyboard arrow commands
    ros::Subscriber velocity_sub = node.subscribe("keyboard/cmd_vel", 1, velocityCallback);

    // set the publishing rate at 10Hz
    ros::Rate rate(10);

    ROS_INFO("Starting to move in a circle");
    
    while (ros::ok()) {
        geometry_msgs::Twist msg;
    	msg.linear.x = lin_vel_;
    	msg.angular.z = ang_vel_;
        pub.publish(msg);
        ros::spinOnce(); // processing of incoming messages
        // When a message arrives, ROS pushes your subscriber callback onto a queue.
        // It does not call it immediately. ROS only processes your callbacks when
        // you tell it to with ros::spinOnce()
        rate.sleep();
        // if no messages are received from the keyboard for 0.5 seconds, then
        // the turtle starts moving in a circle
        if (ros::WallTime::now() - last_command_time_ > ros::WallDuration(0.5))
        {
            lin_vel_ = BASE_LIN_VEL;
            ang_vel_ = BASE_ANG_VEL;
        }
    }
}

