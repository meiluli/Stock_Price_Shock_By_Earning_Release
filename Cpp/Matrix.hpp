//
//  Matrix.hpp
//  TeamProject
//
//  Created by Lynn on 2020/5/7.
//  Copyright © 2020 NYU. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

typedef vector<double> Vector;
typedef vector<Vector> Matrix;

Vector operator+(const Vector& V,const Vector& W);
Vector operator/(const Vector& V,const double& a);
//Vector operator-(const Vector& V,const Vector& W);
//Vector operator*(const double& a,const Vector& V);
//Vector operator*(const Vector& V,const Vector& W);

 
Matrix operator+(const Matrix& C,const Matrix& V);
Matrix operator*(const double& a,const Matrix& V);
Matrix operator/(const Matrix& V,const double& a);
Matrix operator-(const Matrix& C,const Matrix& V);
Matrix operator*(const Matrix& C,const Matrix& V);

ostream & operator<<(ostream & out, Vector & V);
ostream & operator<<(ostream & out, Matrix & W);

#endif /* Matrix_hpp */
