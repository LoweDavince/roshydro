#include<ros/ros.h>
#include<std_msgs/Float64.h>
#include<stdio.h>
#include<dynamixel_msgs/JointState.h>
using namespace std;




class Dynamixel
{
     public:
       ros::NodeHandle n;
       ros::Publisher pub_n;
       float m_goal;
       float m_current_pos;
    private:
       string type;
       
       
     public:
       Dynamixel();
       int moveMotor(double position);
       void JointStateCallback(const dynamixel_msgs::JointState::ConstPtr& msg);
};

Dynamixel::Dynamixel()
{
     pub_n = n.advertise<std_msgs::Float64>("/torque_controller/command",1);
     
     n.getParam("/param/test_two/type",type);
     cout<<"param_test"<<endl;
     string type2;
     ros::param::get("/param/test_two/type",type2);
     cout<<type<<" "<<type2<<endl;

     
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
     ros::init(argc, argv, "torque_test");
     Dynamixel motors;
     
     ros::Subscriber sub;
     ros::Rate loop_rate(1);
     while(ros::ok())
     {

	
	loop_rate.sleep();
     }
}
