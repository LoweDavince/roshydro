#include<ros/ros.h>
#include<std_msgs/Float64.h>
#include<stdio.h>
#include<dynamixel_msgs/JointState.h>

using namespace std;

// pub /upper_controller/command std_msgs/Float64 -- 1.0



class Dynamixel
{
     public:
       ros::NodeHandle n;
       ros::Publisher pub_n;
       float m_goal;
       float m_current_pos;
       
       
     public:
       Dynamixel();
       int moveMotor(double position);
       void upperStateCallback(const dynamixel_msgs::JointState::ConstPtr& msg);
       void lowerStateCallback(const dynamixel_msgs::JointState::ConstPtr& msg);
};

Dynamixel::Dynamixel()
{
     pub_n = n.advertise<std_msgs::Float64>("/torque_controller/command",1);
     
}


void Dynamixel::upperStateCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{
     std::cout<<"upper "<<msg->current_pos<<std::endl;
     m_goal = msg->goal_pos;
}

void Dynamixel::lowerStateCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{
     std::cout<<"lower "<<msg->current_pos<<std::endl;
     m_goal = msg->goal_pos;
}


int Dynamixel::moveMotor(double position)
{
     std_msgs::Float64 aux;
     aux.data = position;
     pub_n.publish(aux);


     return 1;
}



int main(int argc,char** argv)
{
     
     ros::init(argc, argv, "acrobot");
     Dynamixel motors;
     
     ros::Subscriber sub,sub2;
     sub = motors.n.subscribe("/upper_controller/state", 1, &Dynamixel::upperStateCallback,&motors);
     sub2 = motors.n.subscribe("/lower_controller/state", 1, &Dynamixel::lowerStateCallback,&motors);
     float counter = -180;
     ros::Rate loop_rate(100);
     while(ros::ok())
     {
	cout<<"2"<<endl;

	ros::spinOnce();
        
	
	loop_rate.sleep();
     }
}
