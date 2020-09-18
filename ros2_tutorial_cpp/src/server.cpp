// Copyright 2014 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* Authors: Ryan Shim */

#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include "ros2_tutorial_cpp/srv/inquiry.hpp"
#include "ros2_tutorial_cpp/visibility_control.h"

using namespace std::chrono_literals;

namespace ros2_tutorial_cpp
{
class Server : public rclcpp::Node
{
public:
  explicit Server(const rclcpp::NodeOptions & options)
  : Node("Server", options)
  {
    // Force flush of the stdout buffer
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    // ROS Client
    auto inquiry_callback =
      [this](const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<ros2_tutorial_cpp::srv::Inquiry::Request> request,
        std::shared_ptr<ros2_tutorial_cpp::srv::Inquiry::Response> response) -> void
      {
        (void)request_header;
        RCLCPP_INFO(this->get_logger(), "Received Request: %s", request->question.c_str());
        response->answer = "n...yes";
      };
    inquiry_srv_ = create_service<ros2_tutorial_cpp::srv::Inquiry>("inquiry", inquiry_callback);

    RCLCPP_INFO(this->get_logger(), "Initialized server node");
  }

  ~Server()
  {
    RCLCPP_INFO(this->get_logger(), "Terminated server node");
  }

private:
  rclcpp::Service<ros2_tutorial_cpp::srv::Inquiry>::SharedPtr inquiry_srv_;
};
}  // namespace ros2_tutorial_cpp

RCLCPP_COMPONENTS_REGISTER_NODE(ros2_tutorial_cpp::Server)
