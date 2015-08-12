#!/usr/bin/env python

import rospy
import sys
from std_msgs.msg import Float64
k = -180*4096/360/3.1415926
JOINTNUM = 19
filename = '/home/d/dancer_ros/src/zjudancerR/zjudancer_motion/scripts/PCTransmit_leftKick.txt'
f = open(filename, 'r')
line = f.readline()
a = map(float,line.split(' '))
print a


length = len(line.split(' '))
print length
result = [[] for row in range(length)]
# print result[4]

f = open(filename, 'r')
for line in f.readlines():
    l_line = map(float,line.split(' '))
    for i in range(len(l_line)): 
        result[i].append(l_line[i])







def talker():
    joints    = []
    pubs_arm  = []
    pubs_leg  = []
    leg_joint = ['r2','r3','r5','r6','r7','l2','l3','l5','l6','r7']
    arm_joint = ['r1','r18','l1','l18']
    arm_id    = [1,18,8,6]
    leg_id = [2,3,5,6,7,9,10,12,13,14]
    leg_k  = [k,k,k,-k,-k,k,k,k,-k,-k]
    print leg_k


    for joint in leg_joint:
        joint_leg = '/zjudancer/legj_'+joint+'_position_controller/command' 
        joints.append(joint_leg)
        pubs_leg.append(rospy.Publisher(joint_leg, Float64, queue_size=1))
    for joint in arm_joint:
        joint_arm = '/zjudancer/armj_'+joint+'_position_controller/command'
        joints.append(joint_arm)
        pubs_arm.append(rospy.Publisher(joint_arm, Float64, queue_size=1))
    print(joints)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(100) # 10hz
    a = 0.0
    while not rospy.is_shutdown():
        rospy.loginfo(a)
        for j in range(length):
            for i in range(4):
                pubs_arm[i].publish(a)
            for i in range(10):
                pubs_leg[i].publish(result[j][leg_id[i]-1]/leg_k[i])
            a = a + 0.01
            rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass