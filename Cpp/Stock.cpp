//
//  Stock.cpp
//  TeamProject
//
//  Created by Lynn on 5/14/20.
//  Copyright © 2020 NYU. All rights reserved.
//

#include "Stock.hpp"

void SPY::setValue(string Symbol_) //根据symbol，initialize stock的symbol, adjcloses, dates数据
{
    Symbol = Symbol_;
    
    string Date;
    double AdjClose;
    
    vector<string> Temp1;
    vector<double> Temp2;
    
    ifstream fin;
    fin.open(Symbol + ".dat");
    while (fin >> Date >> AdjClose)
    {
        Temp1.push_back(Date);
        Temp2.push_back(AdjClose);
    }
    fin.close();
    
    Dates = Temp1;
    AdjCloses = Temp2;
}

void stock::setValue(string Symbol_, string DateZero_, double EstEPS_, double ActEPS_, double Surprise_)
{
    Symbol = Symbol_;
    
    string Date;
    double AdjClose;
    
    vector<string> Temp1;
    vector<double> Temp2;
    
    ifstream fin;
    fin.open(Symbol + ".dat");
    while (fin >> Date >> AdjClose)
    {
        Temp1.push_back(Date);
        Temp2.push_back(AdjClose);
    }
    fin.close();
    
    Dates = Temp1;
    AdjCloses = Temp2;
    
    DateZero = DateZero_; EstEPS = EstEPS_; ActEPS = ActEPS_;Surprise = Surprise_;
}

void stock::setValue(string Symbol_, map<string, stock> OldMap, vector<double> Benchmark_)
{
    Symbol = Symbol_;
    DateZero = OldMap.find(Symbol)->second.getDateZero();
    EstEPS = OldMap.find(Symbol)->second.getEstEPS();
    ActEPS = OldMap.find(Symbol)->second.getActEPS();
    Type = OldMap.find(Symbol)->second.getType();
    Dates = OldMap.find(Symbol)->second.getDates();
    AdjCloses = OldMap.find(Symbol)->second.getAdjCloses();
    Benchmark = Benchmark_;
    arrangeDate();
    calculateDailyAR();
}

void stock::setN(int N_, string Symbol_, vector<double> Benchmark_)
{
    N = N_;
    Symbol = Symbol_;
    
    string Date;
    double AdjClose;
    
    vector<string> Temp1;
    vector<double> Temp2;
    
    ifstream fin;
    fin.open(Symbol + ".dat");
    while (fin >> Date >> AdjClose)
    {
        Temp1.push_back(Date);
        Temp2.push_back(AdjClose);
    }
    fin.close();
    
    Dates = Temp1;
    AdjCloses = Temp2;
    
    Benchmark = Benchmark_;
    arrangeDate();
    calculateDailyAR();
}

void stock::arrangeDate()
{
    int index = 0;
    vector<string> temp1;    //Date
    vector<double> temp2, temp3;    //AdjClose price, SPY Benchmark price
    
    for(int i = 0; i < Dates.size(); i++)
    {
        if(Dates[i] == DateZero) {index = i;}
    }
    
    if(((index - N) < 0) || ((index + N) > Dates.size()-1))
    {
        cout << Symbol << " warning: data is not sufficient OR release date is not trading date!" << endl;
    }else{
        for (int k = index - N; k <= index + N; k++)
        {
            
            temp1.push_back(Dates[k]);
            temp2.push_back(AdjCloses[k]);
            temp3.push_back(Benchmark[k]);
        }
        Dates = temp1;
        AdjCloses = temp2;
        Benchmark = temp3;
    }
}

void stock::displayOneStock()
{
    cout << endl;
    cout << "Stock Ticker: " << Symbol << endl;
    cout << "Stock Group : " << Type << endl;
    cout << "Release Date: " << DateZero << endl;
    cout << "Surprise: " << Surprise << endl;
    cout << "Estimated EPS: " << EstEPS << endl;
    cout << "Actual EPS: " << ActEPS << endl;

    cout << setiosflags(ios::fixed) << setprecision(3);

                      // right align    //set width
    cout << setiosflags(ios::right) << setw(10) << "Date" << " | " << setw(12) << "AdjClose" << " | " <<
        setw(7) << "Return" << " | " << setw(16) << "AdjClose (SPY)" << " | " << setw(16) << "Return (SPY)" << " | " << setw(7) << "Daily AR" << setw(7) << endl;

    //cout << setiosflags(ios::right) << setw(10) << Dates[0] << " | " << setw(12) << AdjCloses[0] << " | " << setw(7) << "" << " | " << setw(16) << Benchmark[0] << " | " << setw(16) << "" << " | " << setw(7) << "" << setw(7) << endl;

    for (int i = 0; i < DailyAR.size(); i++)
    {
        cout << setiosflags(ios::right) << setw(10) << Dates[i + 1] << " | " << setw(12) << AdjCloses[i + 1] << " | " <<
            setw(7) << DailyStockReturn[i] << " | " << setw(16) << Benchmark[i + 1] << " | " << setw(16) << DailySPYReturn[i] << " | " << setw(7) << DailyAR[i] << setw(7) << endl;
    }
}

void stock::calculateDailyAR()
{
    double stockReturn, SPYReturn, abnormalReturn;
    vector<double> temp_stock, temp_SPY, temp_abnormal;
    for (int i = 0; i < Dates.size() - 1; i++)
    {
        stockReturn = (AdjCloses[i+1] - AdjCloses[i])/AdjCloses[i];
        SPYReturn = (Benchmark[i+1] - Benchmark[i])/Benchmark[i];
        abnormalReturn = stockReturn - SPYReturn;
        temp_stock.push_back(stockReturn);
        temp_SPY.push_back(SPYReturn);
        temp_abnormal.push_back(abnormalReturn);
    }
    DailyStockReturn = temp_stock;
    DailySPYReturn = temp_SPY;
    DailyAR = temp_abnormal;
}
