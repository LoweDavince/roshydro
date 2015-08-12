#include<ros/ros.h>
#include<std_msgs/Float64.h>
#include<stdio.h>
#include<dynamixel_msgs/JointState.h>
#include<config/config.h>
#include<line.hpp>
using namespace std;




class Dynamixel
{
     public:
       ros::NodeHandle n;
       ros::Publisher pub_n;
       float m_goal;
       float m_current_pos;
       class robotStatus *m_status;
       
       
     public:
       Dynamixel();
       int moveMotor(double position);
       void JointStateCallback(const dynamixel_msgs::JointState::ConstPtr& msg);
};

Dynamixel::Dynamixel()
{
     //m_status = robotStatus::getInstance();
     pub_n = n.advertise<std_msgs::Float64>("/torque_controller/command",1);
     //m_status->isSerial();
     
}


void Dynamixel::JointStateCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{
     std::cout<<msg->current_pos<<std::endl;
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
     class Config *config;
     //config = Config::getInstance();
     class Line *a = new Line(1,2,3);
     a->m_a = 0;
     cout<<a->m_a<<endl;
     ros::init(argc, argv, "torque_test");
     Dynamixel motors;
     double AnkleH_mid_l;
     //AnkleH_mid_l = config->getRobotConfig().ankleh_mid_l_low();
     ros::Subscriber sub;
     sub = motors.n.subscribe("/torque_controller/state", 1, &Dynamixel::JointStateCallback,&motors);
     float counter = -180;
     ros::Rate loop_rate(1);
     while(ros::ok())
     {
	cout<<"2"<<endl;

	ros::spinOnce();
        

	
	loop_rate.sleep();
     }
}
