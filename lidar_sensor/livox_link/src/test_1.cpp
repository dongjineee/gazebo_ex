#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

// scan topic을 받아 새로운 link에 scan 데이터 pub 코드

class Re_Scan{
    
private:
    ros::NodeHandle nh;

    ros::Subscriber scan_sub = nh.subscribe<sensor_msgs::PointCloud>("/scan", 1, scanCallback);

    ros::Publisher new_scan = nh.advertise<sensor_msgs::PointCloud>("/new_scan", 1);

    static tf2_ros::TransformBroadcaster br;
    geometry_msgs::TransformStamped transformStamped; // 새로운 link 준비 because 새로운 link를 생성하기 위해

    sensor_msgs::PointCloud pc_msg;
    sensor_msgs::PointCloud2 pd_msg;


public:

    Re_Scan()
    {
        transformStamped.header.frame_id = "livox"; // 부모 link, 밑 link와 연결이 되는 부분 
        transformStamped.child_frame_id = "laser_frame"; // link에 대한 이름 
        transformStamped.header.stamp = ros::Time::now(); 

        // 적절한 위치와 자세 정보를 설정
        transformStamped.transform.translation.x = 0.0;  // X 위치 설정
        transformStamped.transform.translation.y = 0.0;  // Y 위치 설정
        transformStamped.transform.translation.z = 0.0;  // Z 위치 설정
        transformStamped.transform.rotation.x = 0.0;     // X 회전 설정
        transformStamped.transform.rotation.y = 0.0;     // Y 회전 설정
        transformStamped.transform.rotation.z = 0.0;     // Z 회전 설정
        transformStamped.transform.rotation.w = 1.0;     // 회전 quaternion 설정

        br.sendTransform(transformStamped); // 새로운 link에 대한 발행
    }

    void scanCallback(const sensor_msgs::PointCloud::ConstPtr& scan_msg) {
        

        pcl::PointCloud<pcl::PointXYZ> cloud_src;

        for (const auto& point : scan_msg->points) {
            pcl::PointXYZ pcl_point;
            pcl_point.x = point.x;
            pcl_point.y = point.y;
            pcl_point.z = point.z;
            cloud_src.push_back(pcl_point);
        }

        sensor_msgs::PointCloud2 cloudmsg;
        pcl::toROSMsg(cloud_src, cloudmsg); // cloud_to_cloud2
        
        cloudmsg.header.stamp = scan_msg->header.stamp;
        cloudmsg.header.frame_id = "laser_frame";

        pc_msg.header.stamp = scan_msg->header.stamp;
        pc_msg.header.frame_id = "laser_frame";  // 기준 프레임
        pc_msg.points = scan_msg->points; // 다른 link와 연동 시켜서 scan 데이터 pub

        new_scan_pointcloud_2.publish(cloudmsg);
        new_scan.publish(pc_msg);

    }


};

ros::Publisher new_scan;ros::Publisher new_scan_pointcloud_2;

int main(int argc, char** argv) {
    ros::init(argc, argv, "scan_to_base_link_tf");
    ros::NodeHandle nh;

    new_scan_pointcloud_2 = nh.advertise<sensor_msgs::PointCloud2>("/new_scan_point2", 1);


    ros::Rate rate(10.0);  // 예: 10Hz
    while (ros::ok()) {

    ros::spinOnce();  // ROS 이벤트 큐 처리
    transformStamped.header.stamp = ros::Time::now();
        br.sendTransform(transformStamped);   
        rate.sleep();
    }

    return 0;
}
