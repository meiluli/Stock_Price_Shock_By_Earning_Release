//
//  Stock.h
//  TeamProject
//
//  Created by Lynn on 2020/5/7.
//  Copyright © 2020 NYU. All rights reserved.
//

#ifndef Stock_h
#define Stock_h

#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <vector>

using namespace std;

class SPY
{
protected:
    string Symbol;
    vector<string> Dates;
    vector<double> AdjCloses;
    
public:
    void setValue(string Symbol_);
    vector<double> getAdjCloses() { return AdjCloses; }
    vector<string> getDates() { return Dates; }
    string getSymbol() { return Symbol; }
};

class stock: public SPY
{
private:
    string DateZero, Type;
    double EstEPS, ActEPS, Surprise;
    vector<double> Benchmark, DailyStockReturn, DailySPYReturn, DailyAR;
    int N;
    
public:
    stock(){N = 30;};
    void setValue(string Symbol_, string DateZero_, double EstEPS_, double ActEPS_, double Surprise_);
    void setValue(string Symbol_, map<string, stock> OldMap, vector<double> Benchmark_);
    void setN(int N_, string Symbol_, vector<double> Benchmark_);
    void setType(string Type_) { Type = Type_; };
    void arrangeDate();
    void displayOneStock();
    void calculateDailyAR();
    
    vector<double> getDailyStockReturn() { return DailyStockReturn; }    //get daily stocks return
    vector<double> getDailySPYReturn() { return DailySPYReturn; }    //get daily SPY ETF return
    vector<double> getDailyAR() { return DailyAR; }    //get daily AR
    vector<double> getBenchmark() { return Benchmark; }    //get daily benchmark index return
    string getDateZero() { return DateZero; }
    string getType() { return Type; }
    double getEstEPS() { return EstEPS; }
    double getActEPS() { return ActEPS; }
    double getSurprise() { return Surprise; }
    int getN() { return N; }
};

#endif /* Stock_h */
