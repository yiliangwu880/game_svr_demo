cmake_minimum_required (VERSION 2.8)
set(project_name "team")
# 项目信息
project (${project_name} CXX)


# include directories
INCLUDE_DIRECTORIES(system ../external/)
INCLUDE_DIRECTORIES(after  ./cpp_cfg/com/)


# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
file(GLOB_RECURSE SRCS  ${PROJECT_SOURCE_DIR} "../proto_cpp/*.cc")
file(GLOB_RECURSE SRCS2  ${PROJECT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE SRCS3  ${PROJECT_SOURCE_DIR} "../com/*.cpp")
file(GLOB_RECURSE SRCS4  ${PROJECT_SOURCE_DIR} "../external/cpp_cfg/com/*.cpp")

# 指定生成目标
ADD_EXECUTABLE(${project_name} ${SRCS} ${SRCS2} ${SRCS3} ${SRCS4})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin/)

ADD_DEPENDENCIES(${project_name} mf_driver mf_proto protobuf acc_driver acc_proto cevent_cpp svr_util)
#未明：cevent_cpp必须放后面
target_link_libraries(${project_name} mf_driver mf_proto protobuf acc_driver acc_proto cevent_cpp svr_util)

