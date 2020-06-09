//
//  Calculation.cpp
//  TeamProject
//
//  Created by Lynn on 5/14/20.
//  Copyright © 2020 NYU. All rights reserved.
//

#include "Calculation.hpp"

void classification(vector<string> type, vector<map<string, stock>> & OldMap)
{
    cout << "Importing data and classifying . . ." << endl;
    string symbol, date, prd_end, est, act, surp;
    double upper = 0.066, lower = 0.01;//screening conditions for surprise (Beat:157, Meet:165, Miss:157) sort EPS.txt in excel;turn into 3 groups with relatively equivalent numbers of stocks
    stock oneStock;
    vector<string> multiSymbol;
    vector<stock> multiStock;
    OldMap.resize(type.size());//represent 3 groups
    
    //read EPS.txt
    ifstream fin;
    fin.open("EPS.txt");
    while (getline(fin, symbol, ','), getline(fin, date, ','), getline(fin, prd_end, ','),getline(fin, est, ','), getline(fin, act, ','), getline(fin, surp)){
        if (symbol == "Symbol")
        {
            continue;
        }
        else{
            multiSymbol.push_back(symbol);
            oneStock.setValue(symbol, date, stof(est), stof(act), stof(surp));
            multiStock.push_back(oneStock);
        }
        
    }
    fin.close();
    
    for (int i = 0; i < multiStock.size(); i++){
        if (multiStock[i].getSurprise() >= upper){
            multiStock[i].setType(type[0]);
            OldMap[0][multiSymbol[i]] = multiStock[i];//beat
        }else if (multiStock[i].getSurprise() <= lower){
            multiStock[i].setType(type[2]);
            OldMap[2][multiSymbol[i]] = multiStock[i];//miss
        }else{
            multiStock[i].setType(type[1]);
            OldMap[1][multiSymbol[i]] = multiStock[i];//meet
        }
    }
}

//matching and adding benchmark adj close
void benchmark(vector<map<string, stock>> & OldMap, vector<double> Benchmark){
    cout << "Benchmark with SPY and matching the dates . . ." << endl;
    map<string, stock>::iterator itr;
    for (int i = 0; i < OldMap.size(); i++)
        {
        for (itr = OldMap[i].begin(); itr != OldMap[i].end(); itr++)
        {
            itr->second.setValue(itr->first, OldMap[i], Benchmark);
        }
    }
}

void updateN(int N, vector<map<string, stock>> & OldMap, vector<double> Benchmark)
{
    cout << "Updating N . . ." << endl;
    map<string, stock>::iterator itr;
    for (int i = 0; i < OldMap.size(); i++)
    {
        for (itr = OldMap[i].begin(); itr != OldMap[i].end(); itr++)
        {
            itr->second.setN(N, itr->first, Benchmark);
        }
    }
}

//randomly select 30 stocks for every group
vector<map<string, stock>> randomSampling30(vector<string> type, vector<map<string, stock>> Stocks, vector<vector<string>> & Symbol30){
    //cout << "Randomly selecting 30 stocks for each type . . ." << endl;
    vector<map<string, stock>> Sampling;
    //vector<vector<string>> Symbol30(type.size()) //reference more efficient
    int samples = 30;

    map<string, stock>::iterator itr;
    for (int j = 0; j < type.size(); j++)//select for 3 groups
    {
        map<string, stock> temp;
        for (int i = 0; i < samples; i++)
        {
            itr = Stocks[j].begin();
            std::advance(itr, rand() % Stocks[j].size());//random select;Advances the iterator it by n element positions
            if (find(Symbol30[j].begin(), Symbol30[j].end(), itr->first) != Symbol30[j].end()){
                i--;//delect the repeated one
            }else{
                Symbol30[j].push_back(itr->first);
                temp[itr->first] = itr->second;
            }
        }
        Sampling.push_back(temp);
    }
    return Sampling;
}

//Calulating AAR for single sampling
//Calculate average daily abnormal returns for each group of stocks
//每个type里30个sample求平均
vector<vector<double>> calculateAAR(vector<map<string, stock>> Sampling){
    //cout << "Calculating AAR ing . . ." << endl;
    vector<vector<double>> AARMap;
    for (int i = 0; i < Sampling.size(); i++){
        map<string, stock>::iterator itr;
        itr = Sampling[i].begin();
        
        int n = itr->second.getDailyAR().size();//2N+1
        
        vector<double> temp(n);
        while (itr != Sampling[i].end()){
            temp = temp + itr->second.getDailyAR(); //sum of 30 samples' AR for n = 2N+1 days
            itr++;
        }
        temp = temp / Sampling[i].size(); //average of 30 samples
        AARMap.push_back(temp);
    }
    return AARMap; //calculate AAR for three types 3x(2N+1)
}

//Calculating CAAR for single sampling
vector<vector<double>> calculateCAAR(vector<vector<double>> AARMap){
    //cout << "Calculating CAAR ing . . ." << endl;
    vector<vector<double>> CAARMap;
    for (int i = 0; i < AARMap.size(); i++){
        double sum = 0.0;
        vector<double> temp(AARMap[i].size());
        for (int j = 0; j < AARMap[i].size(); j++)
        {
            sum += AARMap[i][j];
            temp[j] = sum;
        }
        CAARMap.push_back(temp);
    }
    return CAARMap;  //calculate CAAR for all three types stocks, 3x60
}//Cumulate the returns on the first T days 3x(2N+1)

//Repeat sampling to get average AAR and CAAR and their SD.
void bootstrapAvgSd(int repeattimes, vector<string> type, vector<map<string, stock>> & symbolMap, Matrix &avgAAR, Matrix &sdAAR, Matrix &avgCAAR, Matrix &sdCAAR)
{
    
    vector<vector<string>> Symbols(type.size());
    vector<map<string, stock>> selectedSample;

    vector<vector<vector<double>>> multiAAR;
    vector<vector<vector<double>>> multiCAAR;
    
    //get avgAAR(3 x 2N+1), avgCAAR(3 x 2N+1), multiAAR(repeattime x 3 x 2N+1), multiCAAR(repeattime x 3 x 2N+1)
    //Average AAR and CAAR for 30 samplings for each group
    for (int i = 0; i < repeattimes; i++)
    {
        Symbols.clear(); Symbols.resize(type.size()); selectedSample.clear();

        vector<vector<double>> sampleAAR;
        vector<vector<double>> sampleCAAR;

        selectedSample = randomSampling30(type, symbolMap, Symbols);
        
        sampleAAR = calculateAAR(selectedSample);  //3x60
        multiAAR.push_back(sampleAAR);
        
        avgAAR.resize(sampleAAR.size()); // sampleARR.size() = 3
        for (int m = 0; m < sampleAAR.size(); m++)
        {
            int s = sampleAAR[m].size(); //s = 2N + 1
            avgAAR[m].resize(s);
        }
        
        avgAAR = (i*avgAAR + sampleAAR) / (i + 1.0);  //operator overloading

        sampleCAAR = calculateCAAR(sampleAAR); //3x60
        multiCAAR.push_back(sampleCAAR);
        
        // same as for avgAAR
        avgCAAR.resize(sampleCAAR.size());
        for (int m = 0; m < sampleCAAR.size(); m++){
            int s = sampleCAAR[m].size();
            avgCAAR[m].resize(s);
        }
        avgCAAR = (i*avgCAAR + sampleCAAR) / (i + 1.0);
    }
    
    vector<vector<double>> varAAR;
    vector<vector<double>> varCAAR;
    
    sdAAR.resize(avgAAR.size()); // avgAAR.size()=3
    for (int m = 0; m < avgAAR.size(); m++)
    {
        int s = avgAAR[m].size();
        sdAAR[m].resize(s);
    }

    vector<vector<vector<double>>> diffSquare1;
    for (int i = 0; i < multiAAR.size(); i++) //multiAAR.size()=repeattimes
    {
        diffSquare1.push_back((multiAAR[i] - avgAAR)*(multiAAR[i] - avgAAR)); // operator overloading (minus + dot multiply)
    }
    vector<vector<double>> sumSquare1; // (3 x 2N+1)
    sumSquare1 = diffSquare1[0];
    for (int i = 1; i < diffSquare1.size(); i++) //diffSquare1.size()=repeattimes
    {
        sumSquare1 = sumSquare1 + diffSquare1[i];// operator overloading plus
    }
    
    varAAR.resize(sdAAR.size());
    for (int m = 0; m < sdAAR.size(); m++)  // sdAAR.size()=3
    {
        int s = sdAAR[m].size();
        varAAR[m].resize(s);
    }
    for (int i = 0; i < sumSquare1.size(); i++)
    {
        for (int j = 0; j < sumSquare1[i].size(); j++)
        {
            varAAR[i][j] = sumSquare1[i][j]/repeattimes;
        }
    }

    for (int i = 0; i < varAAR.size(); i++)
    {
        for (int j = 0; j < varAAR[i].size(); j++){
            sdAAR[i][j] = sqrt(varAAR[i][j]);
        }
    }
//the same as sdAAR varAAR
    sdCAAR.resize(avgCAAR.size());
    for (int m = 0; m < avgCAAR.size(); m++)  // avgCAAR.size()=3
    {
        int s = avgCAAR[m].size();
        sdCAAR[m].resize(s);
    }

    vector<vector<vector<double>>> diffSquare2;
    for (int i = 0; i < multiCAAR.size(); i++) //multiCAAR.size()=repeattimes
    {
        diffSquare2.push_back((multiCAAR[i] - avgCAAR)*(multiCAAR[i] - avgCAAR));// operator overloading (minus + dot multiply)
    }
    vector<vector<double>> sumSquare2;
    sumSquare2 = diffSquare2[0];
    for (int i = 1; i < diffSquare2.size(); i++)
    {
        sumSquare2 = sumSquare2 + diffSquare2[i];
    }
    
    varCAAR.resize(sdCAAR.size());
    for (int m = 0; m < sdCAAR.size(); m++)  // SampleAAR.size()=3
    {
        int s = avgCAAR[m].size();
        varCAAR[m].resize(s);
    }
    for (int i = 0; i < sumSquare2.size(); i++)
    {
        for (int j = 0; j < sumSquare2[i].size(); j++)
        {
            varCAAR[i][j] = sumSquare2[i][j]/repeattimes;
        }
    }
    for (int i = 0; i < varCAAR.size(); i++)
    {
        for (int j = 0; j < varCAAR[i].size(); j++){
            sdCAAR[i][j] = sqrt(varCAAR[i][j]);
        }
    }
}

void Matrix_3D(vector<vector<double>>&avgAAR, vector<vector<double>> &sdAAR, vector<vector<double>>&avgCAAR, vector<vector<double>> &sdCAAR, vector<vector<vector<double>>> & thrDMatrix)//4x3x2N
{
    thrDMatrix.clear();
    //cout << "Storing AAR, AAR-SD, CAAR and CAAR-SD for all 3 groups into a 3D matrix . . ." << endl;
    thrDMatrix.push_back(avgAAR);
    thrDMatrix.push_back(sdAAR);
    thrDMatrix.push_back(avgCAAR);
    thrDMatrix.push_back(sdCAAR);
}
