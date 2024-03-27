# 운영체제에 설치된 cmake의 최소 요구 버전
cmake_minimum_required(VERSION 3.0.2)

# 패키지의 이름으로, package.xml에서 입력한 패키지 이름을 그대로 사용
project(urdf_ex)

find_package(catkin REQUIRED COMPONENTS
  urdf
)

find_package(Boost REQUIRED COMPONENTS system)


include_directories(
  ${catkin_INCLUDE_DIRS}
)

