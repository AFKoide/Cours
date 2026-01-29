#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Function {
protected:
  double A, B, C, D; // Paramètres communs pour toutes les fonctions dérivées

public:
  Function(double A, double B, double C, double D) : 
    A(A), B(B), C(C), D(D) {}

  virtual double evaluate(double x) const;
  virtual void display() const;


  ~Function() {};
};


double Function::evaluate(double x) const {
  return A * exp(-(pow(x - C, 2) / pow(B, 2)) + D);
}

void Function::display() const {
  cout << "f(x) = " << A << " * exp(-((x - " << C << ")^2) / (" << B << "^2)) + " << D << endl;
  cout << "f(8.5) = " << evaluate(8.5) << endl;
}


class Gaussian : public Function {
public:
  Gaussian(double A, double B, double C, double D) :
    Function(A, B, C, D) {}
  ~Gaussian() {};
};



int main() 
{
  Gaussian g(2, 3, 1.5, 7);

  double result = g.evaluate(8.5);

  g.display();


}