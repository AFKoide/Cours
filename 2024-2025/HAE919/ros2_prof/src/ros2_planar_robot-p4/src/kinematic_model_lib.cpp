#include "kinematic_model_lib.h"

RobotModel::RobotModel(const double &l1In, const double &l2In):
  l1  (l1In),
  l2  (l2In)
{};

void RobotModel::FwdKin(Eigen::Vector2d &xOut, Eigen::Matrix2d &JOut, const Eigen::Vector2d & qIn){
  xOut <<   l1*cos(qIn(0)) + l2*cos(qIn(1) + qIn(0)),
            l1*sin(qIn(0)) + l2*sin(qIn(1) + qIn(0));
  JOut <<  -l1*sin(qIn(0)) - l2*sin(qIn(1)+qIn(0))  ,-l2*sin(qIn(1) + qIn(0)),
            l1*cos(qIn(0)) + l2*cos(qIn(1)+qIn(0))  , l2*cos(qIn(1) + qIn(0));
}