//
//  Calculation.h
//  TeamProject
//
//  Created by Lynn on 2020/5/7.
//  Copyright © 2020 NYU. All rights reserved.
//

#ifndef Calculation_h
#define Calculation_h

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <ctime>
#include <locale>
#include <algorithm>
#include "Stock.hpp"
#include "Matrix.hpp"
#include "math.h"

using namespace std;

void classification(vector<string> type, vector<map<string, stock>> & OldMap);
//matching and adding benchmark adj close
void benchmark(vector<map<string, stock>> & OldMap, vector<double> Benchmark);
void updateN(int N, vector<map<string, stock>> & OldMap, vector<double> Benchmark);

//randomly select 30 stocks for every group
vector<map<string, stock>> randomSampling30(vector<string> type, vector<map<string, stock>> Stocks, vector<vector<string>> & Symbol30);
//Calulating AAR for single sampling
//Calculate average daily abnormal returns for each group of stocks
vector<vector<double>> calculateAAR(vector<map<string, stock>> Sampling);
//Calculating CAAR for single sampling
vector<vector<double>> calculateCAAR(vector<vector<double>> AARMap);
//Repeat sampling to get average AAR and CAAR and their SD.
void bootstrapAvgSd(int repeattimes, vector<string> type, vector<map<string, stock>> & symbolMap, Matrix &avgAAR, Matrix &sdAAR, Matrix &avgCAAR, Matrix &sdCAAR);
//4x3x2N
void Matrix_3D(vector<vector<double>>&avgAAR, vector<vector<double>> &sdAAR, vector<vector<double>>&avgCAAR, vector<vector<double>> &sdCAAR, vector<vector<vector<double>>> & thrDMatrix);

#endif /* Calculation_h */
