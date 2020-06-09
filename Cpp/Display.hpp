//
//  Display.hpp
//  TeamProject
//
//  Created by Lynn on 5/14/20.
//  Copyright © 2020 NYU. All rights reserved.
//

#ifndef Display_hpp
#define Display_hpp

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

void displayHistoricalPrice(vector<map<string, stock>> symbolMap_);
// Display stock names by group
void displayInformation(vector<map<string,stock>> symbolMap_);
// AAR-Ave AAR-SD CAAR-Ave CAAR-SD are stored in thrDMatrix_
void showAARandCAAR(vector<vector<vector<double>>> &thrDMatrix_);
int menuChoice();
int getValidInput(int limit);
void outputToFile(vector<map<string, stock>> symbolMap_);

void outputCAAR(vector<double>& line1, vector<double>& line2, vector<double>& line3, int &N);
void plotResults(double* xData, double* yData, double* yData2, double* yData3, int dataSize);

#endif /* Display_hpp */
