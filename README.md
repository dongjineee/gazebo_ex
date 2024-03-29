lidar / depth_camer / odometry gazebo simulation 

roslaunch urdf_ex example_gazebo.launch
roslaunch urdf_ex controller.launch
rosrun teleop_twist_keyboard teleop_twist_keyboard.py cmd_vel:=/base_controller/cmd_vel
