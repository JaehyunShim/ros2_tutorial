// Copyright 2020, Jaehyun Shim, ROBOTIS CO., LTD.
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

#include <pluginlib/class_list_macros.h>
#include <QStringList>
#include <memory>
#include <thread>

#include "rclcpp/rclcpp.hpp"

#include "rqt_example/rqt_example.hpp"

namespace rqt_example
{
RqtExample::RqtExample()
: rqt_gui_cpp::Plugin(),
  widget_(0)
{
  setObjectName("RQT Example");
}

void RqtExample::initPlugin(qt_gui_cpp::PluginContext & context)
{
  // Access standalone command line arguments
  QStringList argv = context.argv();
  // Create QWidget
  widget_ = new QWidget();
  // Extend the widget with all attributes and children from UI file
  ui_.setupUi(widget_);
  // Add widget to the user interface
  if (context.serialNumber() > 1) {
    widget_->setWindowTitle(
      widget_->windowTitle() + " (" + QString::number(context.serialNumber()) + ")");
  }
  context.addWidget(widget_);

  // Run a thread for ros_spin
  std::thread ros_thread(run_ros_thread);
  ros_thread.detach();

  // Set states
  connect(ui_.pub_on_button, SIGNAL(clicked(bool)), this, SLOT(set_pub_on()));
  connect(ui_.pub_off_button, SIGNAL(clicked(bool)), this, SLOT(set_pub_off()));
  connect(ui_.sub_on_button, SIGNAL(clicked(bool)), this, SLOT(set_sub_on()));
  connect(ui_.sub_off_button, SIGNAL(clicked(bool)), this, SLOT(set_sub_off()));

  // Get & Display states
  std::thread display_thread(run_display_thread, &ui_);
  display_thread.detach();
}

void RqtExample::shutdownPlugin()
{
  // TODO(my_username): unregister all publishers here
}

void RqtExample::saveSettings(
  qt_gui_cpp::Settings & plugin_settings,
  qt_gui_cpp::Settings & instance_settings) const
{
  // TODO(my_username): save intrinsic configuration, usually using:
  // instance_settings.setValue(k, v)
}

void RqtExample::restoreSettings(
  const qt_gui_cpp::Settings & plugin_settings,
  const qt_gui_cpp::Settings & instance_settings)
{
  // TODO(my_username): restore intrinsic configuration, usually using:
  // v = instance_settings.value(k)
}

// TODO(JaehyunShim): Find a better way to run ros_spin
auto rqt_node_ = std::make_shared<rqt_example::RqtNode>();
void RqtExample::run_ros_thread()
{
  rclcpp::spin(rqt_node_);
}

// TODO(JaehyunShim): Find a better way to run display_spin
void RqtExample::run_display_thread(Ui::RqtExampleWidget * ui)
{
  while (1) {
    ui->pub_onoff_state->setText(get_pub_onff());
    ui->sub_onoff_state->setText(get_sub_onff());
  }
}

QString RqtExample::get_pub_onff()
{
  QString q_str;
  if (rqt_node_->pub_onoff_ == true) {
    q_str = "on";
  } else {
    q_str = "off";
  }

  return q_str;
}

QString RqtExample::get_sub_onff()
{
  QString q_str;
  if (rqt_node_->sub_onoff_ == true) {
    q_str = "on";
  } else {
    q_str = "off";
  }

  return q_str;
}

void RqtExample::set_pub_on()
{
  rqt_node_->pub_onoff_ = true;
}

void RqtExample::set_pub_off()
{
  rqt_node_->pub_onoff_ = false;
}
void RqtExample::set_sub_on()
{
  rqt_node_->sub_onoff_ = true;
}

void RqtExample::set_sub_off()
{
  rqt_node_->sub_onoff_ = false;
}
}  // namespace rqt_example

PLUGINLIB_EXPORT_CLASS(rqt_example::RqtExample, rqt_gui_cpp::Plugin)
