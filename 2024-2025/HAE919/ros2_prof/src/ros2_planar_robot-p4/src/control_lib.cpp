#include "control_lib.h"
#include <iostream>
Controller::Controller()
{
}

Eigen::Vector2d Controller::Dqd ( 
          const Eigen::Vector2d & x,
          const Eigen::Vector2d & xd,
          const Eigen::Vector2d & Dxd_ff,
          const Eigen::Matrix2d & J
                      )
{
  dX_desired     = Dxd_ff + (xd - x)*kp;
  return J.inverse() * dX_desired;
}

