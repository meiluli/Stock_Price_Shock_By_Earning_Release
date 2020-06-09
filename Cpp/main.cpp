//
//  main.cpp
//  TeamProject
//
//  Created by Lynn on 2020/5/7.
//  Copyright © 2020 NYU. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include "curl/curl.h"

#include "Calculation.hpp"
#include "getData.hpp"
#include "Stock.hpp"
#include "Display.hpp"

using namespace std;

int main()
{
//    getData();
    srand(time(NULL));
    cout << "----- 6883 Final Project -----" << endl;
    
    vector<string> type = { "Beat", "Meet", "Miss" };
    vector<map<string, stock>> symbolMap;
    classification(type, symbolMap);
    
    SPY SP500ETF;
    SP500ETF.setValue("SPY");
    
    benchmark(symbolMap, SP500ETF.getAdjCloses());
    
    int repeatTimes = 30;
    
    vector<vector<double>> avgAAR, sdAAR, avgCAAR, sdCAAR;
    bootstrapAvgSd(repeatTimes, type, symbolMap, avgAAR, sdAAR, avgCAAR, sdCAAR);
    cout << "Sampling " << repeatTimes << " times" << endl;
    vector<vector<vector<double>>> AARandCAAR;
    Matrix_3D(avgAAR, sdAAR, avgCAAR, sdCAAR, AARandCAAR);
    vector<double> line_beat;
    vector<double> line_meet;
    vector<double> line_miss;
    
    bool execute = true;
    int N = 30;
    while (execute) {
        //display menu and get choice
        int choice = menuChoice();
        
        switch(choice){
            case 1:
                cout << "Enter times of sampling (>=30): ";
                repeatTimes = getValidInput(30);
                bootstrapAvgSd(repeatTimes, type, symbolMap, avgAAR, sdAAR, avgCAAR, sdCAAR);
                cout << "Sampling " << repeatTimes << " times" << endl;
                Matrix_3D(avgAAR, sdAAR, avgCAAR, sdCAAR, AARandCAAR);
                continue;
            case 2:
                cout << "Please enter N (>=30):  ";
                N = getValidInput(30);
                updateN(N, symbolMap, SP500ETF.getAdjCloses());
                bootstrapAvgSd(repeatTimes, type, symbolMap, avgAAR, sdAAR, avgCAAR, sdCAAR);
                Matrix_3D(avgAAR, sdAAR, avgCAAR, sdCAAR, AARandCAAR);
                
                displayHistoricalPrice(symbolMap);
                
                char outChoice;
                cout << "Ouput to file? [Y/N]: "; cin >> outChoice;
                if(outChoice == 'Y') outputToFile(symbolMap);
                continue;
            case 3:
                displayInformation(symbolMap);
                continue;
            case 4:
                showAARandCAAR(AARandCAAR);
                continue;
            case 5:
                //for first index：0 AAR-Ave 1 AAR-SD 2 CAAR-Ave 3 CAAR-SD
                line_beat = AARandCAAR[2][0];
                line_meet = AARandCAAR[2][1];
                line_miss = AARandCAAR[2][2];
                outputCAAR(line_beat, line_meet, line_miss, N);
                cout << "Finish Plotting." << endl;
                continue;
            case 6:
                execute = false;
                cout << "Program ends. BYE!" << endl << endl;
                break;
        }
    }
    return 0;
}
