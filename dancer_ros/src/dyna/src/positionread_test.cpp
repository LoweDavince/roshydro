#include<ros/ros.h>
#include<std_msgs/Float64.h>
#include<stdio.h>
#include<dynamixel_msgs/JointState.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include<sensor_msgs/JointState.h>

#define JOINTNUM 19
#define PI 3.1415926
using namespace std;

// pub /upper_controller/command std_msgs/Float64 -- 1.0
std::ofstream m_data("./data.log");


class Dynamixel
{
     public:
       ros::NodeHandle n;
       ros::Publisher pub1,pub2,pub3,pub4;
       float m_goal;
       float m_current_pos;
       double data_rec[JOINTNUM];
       
       
     public:
       Dynamixel();
       int moveMotor(double position);
       void motorRec();
       void id2StateCallback(const dynamixel_msgs::JointState::ConstPtr& msg);
       void id5StateCallback(const dynamixel_msgs::JointState::ConstPtr& msg);
       void id9StateCallback(const dynamixel_msgs::JointState::ConstPtr& msg);
       void id12StateCallback(const dynamixel_msgs::JointState::ConstPtr& msg);
       void StateCallback(const sensor_msgs::JointState::ConstPtr& msg);
     public:
       bool loadFile(const char* filename, int** t_data);
       bool loadData(int** t_data);
       bool loadGaitFile(void);
       void doTxTask(int *motionData /* = NULL */);

     public:
        fstream file;
        int length;
        int* data[JOINTNUM];
        string fileAddress;
};

Dynamixel::Dynamixel()
{
     pub1 = n.advertise<std_msgs::Float64>("/id2_controller/command",1);
     pub2 = n.advertise<std_msgs::Float64>("/id5_controller/command",1);
     pub3 = n.advertise<std_msgs::Float64>("/id9_controller/command",1);
     pub4 = n.advertise<std_msgs::Float64>("/id12_controller/command",1);
     for(int i = 0;i<JOINTNUM;i++)
	   data[i] = 0;
     loadGaitFile();
     //cout<<length<<endl;
}

bool Dynamixel::loadGaitFile()
{

    if(!loadFile(("PCTransmit_leftKick.txt"),data))
    {
	cout<<"loadfile failed"<<endl;
    }

    return true;
}

bool Dynamixel::loadData(int** t_data)
{
    int temp;

    length = 0;
    while(file >> temp)
    {
        length++;
    }
    file.clear();

    length = length / JOINTNUM;

    for(int i = 0; i < JOINTNUM; i++)
    {
        if(t_data[i])
            // delete []t_data[i];
            t_data[i] = NULL;
        t_data[i] = new int[length];
    }

    file.seekg(ios_base::beg);

    for(int i = 0; i<JOINTNUM; i++)
    {
        for(int j = 0; j<length; j++)
        {
            file>>temp;
            t_data[i][j] = temp;
        }
    }
    return true;
}

bool Dynamixel::loadFile(const char* filename, int** t_data)
{
    file.clear();
    file.open(filename, ios_base::in);
        
    if(file.fail())
    {
        cout<<"file.fail"<<endl;
        return false;
    }
    if(!loadData(t_data))
        return false;
    if (file.is_open())
    {
        file.close();
    }
    return true;
}

void Dynamixel::StateCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
     //cout<<msg->position[1]<<endl;
	data_rec[2-1] = msg->position[1];
    data_rec[5-1] = msg->position[0];
    data_rec[9-1] = msg->position[2];

}
void Dynamixel::id2StateCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{
     //m_data<<"id2 "<<msg->current_pos<<std::endl;
     data_rec[2-1] = msg->current_pos;
}
void Dynamixel::id5StateCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{
     //m_data<<"id5 "<<msg->current_pos<<std::endl;
     data_rec[5-1] = msg->current_pos;
}
void Dynamixel::id9StateCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{
     //m_data<<"id7 "<<msg->current_pos<<std::endl;
     data_rec[9-1] = msg->current_pos;
}
void Dynamixel::id12StateCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{
     //m_data<<"id12 "<<msg->current_pos<<std::endl;
     data_rec[12-1] = msg->current_pos;
}


int Dynamixel::moveMotor(double position)
{
     std_msgs::Float64 aux;
     aux.data = position;

     pub1.publish(aux);
     pub2.publish(aux);
     pub3.publish(aux);
     pub4.publish(aux);

     return 1;
}

void Dynamixel::doTxTask(int *motionData /* = NULL */)
{
    double k0  =  180*4096/360/PI;
    double send_data[JOINTNUM];
    std_msgs::Float64 aux2,aux5,aux9,aux12;
    for (int i = 0;i < JOINTNUM; i++)
     {
         send_data[i] = (motionData[i]+2048)/k0;
         //cout<<send_data[i]<<" ";
     }
    //cout<<endl;
    aux2.data  = send_data[5-1];
    aux5.data  = send_data[5-1];
    aux9.data  = send_data[5-1];
    aux12.data = send_data[5-1];
    pub1.publish(aux2);
    pub2.publish(aux5);
    pub3.publish(aux9);
    pub4.publish(aux12);
    
}


void Dynamixel::motorRec()
{
  for(int i = 0;i<JOINTNUM;i++)
      m_data<<data_rec[i]<<" ";
  m_data<<endl;
}









int main(int argc,char** argv)
{
     
     ros::init(argc, argv, "zjudancer");
     Dynamixel motors;
     
     ros::Subscriber sub,sub2,sub3,sub4;
     sub = motors.n.subscribe("/joint_states", 1, &Dynamixel::StateCallback,&motors);
     //sub2 = motors.n.subscribe("/id5_controller/state", 1, &Dynamixel::id5StateCallback,&motors);
     //sub3 = motors.n.subscribe("/id9_controller/state", 1, &Dynamixel::id9StateCallback,&motors);
     //sub4 = motors.n.subscribe("/id12_controller/state", 1, &Dynamixel::id12StateCallback,&motors);
     int timePoint = 0;
     int dataArray[JOINTNUM];
     for(int i = 0; i < JOINTNUM; i++)
     dataArray[i] = 0;
     ros::Rate loop_rate(83);
     while(ros::ok())
     {
	
	   //motors.moveMotor(-2.0);//sin(counter*2*3.1415926/10000));
		
	   if(timePoint < motors.length) // length is step point for each joint
   	    {
                for(int i = 0; i < JOINTNUM; i++)
                dataArray[i] = motors.data[i][timePoint];
                motors.doTxTask(dataArray);
                timePoint++;
        }

        motors.motorRec();
        ros::spinOnce();
        loop_rate.sleep();
	
     }
}
