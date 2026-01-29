#include "trajectory_generation_lib.h"

Polynomial::Polynomial(){};
Polynomial::Polynomial(const double &piIn, const double &pfIn, const double & DtIn){
  pi  = piIn;
  pf  = pfIn;
  a   = { pi            , 
          0.            ,
          0.            ,
          10. *(pf-pi)  ,
          -15.*(pf-pi)  ,
          6.  *(pf-pi)  };
  dt  = DtIn;
};
void          Polynomial::update(const double &piIn, const double &pfIn, const double & DtIn){
  pi  = piIn;
  pf  = pfIn;  
  a   =  {pi          , 
          0.          ,
          0.          ,
          10. *(pf-pi),
          -15.*(pf-pi),
          6.  *(pf-pi)};
  dt  = DtIn;
};
double  Polynomial::p     (const double &t){
  tb      = t/dt;
  if (t<0){
    return pi;
  }else if(t>dt){
    return pf;
  }else{
    return a[0] + a[3]*pow(tb,3) + a[4]*pow(tb,4) + a[5]*pow(tb,5) ;
  }
};
double  Polynomial::dp    (const double &t){
  tb      = t/dt;
  if (t<0){
    return 0;
  }else if(t>dt){
    return 0;
  }else{
    return (3.*a[3]*pow(tb,2) + 4.*a[4]*pow(tb,3) + 5.*a[5]*pow(tb,4)) / dt ;
  }
};

Point2Point::Point2Point(const Eigen::Vector2d & xi, const Eigen::Vector2d & xf, const double & DtIn){
  dt  = DtIn;
  polx.update(xi(0),xf(0),dt);
  poly.update(xi(1),xf(1),dt);
}
void Point2Point::update(const Eigen::Vector2d & xi, const Eigen::Vector2d & xf, const double & DtIn){
  dt  = DtIn;
  polx.update(xi(0),xf(0),dt);
  poly.update(xi(1),xf(1),dt);
}
Eigen::Vector2d Point2Point::X(const double & time){
  Eigen::Vector2d x   { Eigen::Vector2d::Zero() };
  x               <<  polx.p(time), poly.p(time);
  return x;
}

Eigen::Vector2d Point2Point::dX(const double & time){
  Eigen::Vector2d Dx   { Eigen::Vector2d::Zero() };
  Dx              <<  polx.dp(time), poly.dp(time);
  return Dx;
}