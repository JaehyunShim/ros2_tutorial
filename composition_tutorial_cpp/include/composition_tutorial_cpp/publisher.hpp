// Copyright Open Source Robotics Foundation, Inc.
// Copyright 2021 Jaehyun Shim
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

#ifndef COMPOSITION_TUTORIAL_CPP__PUBLISHER_HPP_
#define COMPOSITION_TUTORIAL_CPP__PUBLISHER_HPP_

#include <chrono>
#include <memory>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include "interface_tutorial_cpp/msg/count.hpp"

namespace composition_tutorial_cpp
{
/**
 * @class Publisher
 * @brief Publishes "chatter"
 */
class Publisher : public rclcpp::Node
{
public:
  /**
   * @brief Constructor
   * @param options A pointer to the node options
   */
  explicit Publisher(const rclcpp::NodeOptions & options)  // options for what ???
  : Node("publisher", options)
  {
    // ROS Publisher
    rclcpp::QoS qos(rclcpp::KeepLast(10));
    count_pub_ = this->create_publisher<interface_tutorial_cpp::msg::Count>("chatter", qos);

    // ROS Timer
    auto timer_callback =
      [this]() -> void
      {
        msg_ = std::make_unique<interface_tutorial_cpp::msg::Count>();
        msg_->number = count_++;
        RCLCPP_INFO(this->get_logger(), "%d", msg_->number);
        count_pub_->publish(std::move(msg_));
        msg_published = true;
      };
    timer_ = this->create_wall_timer(std::chrono::seconds(1), timer_callback);

    RCLCPP_INFO(this->get_logger(), "Initialized publisher node");
  }

  /**
   * @brief Virtual destructor
   */
  ~Publisher()
  {
    RCLCPP_INFO(this->get_logger(), "Terminated publisher node");
  }

  bool msg_published = false;

private:
  uint16_t count_ = 0;
  std::unique_ptr<interface_tutorial_cpp::msg::Count> msg_;  // why unique pointer for this?
  rclcpp::Publisher<interface_tutorial_cpp::msg::Count>::SharedPtr count_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};
}  // namespace composition_tutorial_cpp

#endif  // COMPOSITION_TUTORIAL_CPP__PUBLISHER_HPP_
