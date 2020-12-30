//获取turtle1的实时位置
//将实时位置上传个TF工具


#include <iostream>
#include <ros/ros.h>
#include <turtlesim/Pose.h>

#include <tf/transform_broadcaster.h>

#include <vector>


using namespace std;


void poseCallback(const turtlesim::Pose::ConstPtr &msg, tf::TransformBroadcaster broadcaster) {
    //获取turtle1的实时位置 给定的坐标是相对于显示面板(世界坐标系)

    float x = msg->x;
    float y = msg->y;
    float theta = msg->theta;

//    ROS_INFO_STREAM(x << " " << y << " " << theta);

    //实时上传
    // 位置和姿态信息
    tf::Transform input;
    //设置位置 (x, y, z)
    input.setOrigin(tf::Vector3(x, y, 0));
    //设置姿态(以四元素建立姿态描述) （欧拉角（roll, pitch, yaw）） 四元素(x, y, z, w) https://quaternions.online/
    tf::Quaternion quat;
    quat.setRPY(0, 0, theta);
    input.setRotation(quat);

    tf::StampedTransform transform(input, ros::Time::now(), "world", "turtle1");
    broadcaster.sendTransform(transform);
}


// tf使用步骤:
// 1. 映入依赖 tf
// 2. 添加头文件
// 3. 同broadcaster.sendTransform来上传

int main(int argc, char **argv) {
    // 初始化节点
    string nodeName = "demo01_turtle1_node";
    ros::init(argc, argv, nodeName);

    ros::NodeHandle node;

    //将实时位置上传个TF工具(broadcaster)
    tf::TransformBroadcaster broadcaster;

    //获取turtle1的实时位置
    string pose_topic_name = "/turtle1/pose";
    const ros::Subscriber &subscriber = node.subscribe<turtlesim::Pose>(
            pose_topic_name,
            1000,
            boost::bind(poseCallback, _1, broadcaster)
            );

    //阻塞
    ros::spin();

    return 0;
}