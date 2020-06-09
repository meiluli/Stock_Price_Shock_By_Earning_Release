//
//  Display.cpp
//  TeamProject
//
//  Created by Lynn on 5/14/20.
//  Copyright © 2020 NYU. All rights reserved.
//

#include "Display.hpp"

void displayHistoricalPrice(vector<map<string, stock>> symbolMap_){
    cout << setiosflags(ios::fixed) << setprecision(4);//保留四位小数

    for (int n = 0; n < symbolMap_.size(); n++){
        map<string, stock>::iterator itr;
        itr = symbolMap_[n].begin();

        while (itr != symbolMap_[n].end()){
            cout << "Stock Name : " << itr->second.getSymbol() << endl;
            cout << setiosflags(ios::internal) << setw(10) << "Date" << " | "
                << setw(12) << "AdjClose" << "  " << setw(7) << endl;

            for (int i = 0; i < itr->second.getDates().size(); i++){
                cout << setiosflags(ios::internal) << setw(10) << itr->second.getDates()[i] << " | "
                    << setw(12) << itr->second.getAdjCloses()[i] << setw(7) << endl;
            }
            itr++;
        }
    }
}

void displayInformation(vector<map<string,stock>> symbolMap_){
    map<string, stock>::iterator itr;
    const string types[3] = {"BEAT","MEET","MISS"};
    for (int i = 0; i < symbolMap_.size(); i++){
        itr = symbolMap_[i].begin();
        cout << "----------------------------" << types[i] << "----------------------------";
        int j = 0;
        while (itr != symbolMap_[i].end()){
            if(j % 10 == 0) cout << endl;//display 10 symbols in a line
            cout << setiosflags(ios::right) << setw(6) << itr->first;
            itr++;
            j++;
        }
        cout << endl;
    }

    cout << "Please choose a ticker symbol (Enter EXIT to return to main menu): ";
    string ticker;
    while(ticker != "EXIT"){
        cin >> ticker;
        
        bool test = false;
        int i = 0;
        while(!test && i < symbolMap_.size()){
            if(symbolMap_[i].find(ticker) != symbolMap_[i].end())
            {
                symbolMap_[i].find(ticker)->second.displayOneStock();
                //map.find() returns an iterator or a constant iterator which refers to the position where the key is present in the map.
                cout << endl;
                cout << "Please choose another ticker symble or enter EXIT to return to main menu: ";
                test = true;
            }
            i++;
        }
        if(!test && ticker != "EXIT")
        {
            cout << "TICKER DOES NOT EXIST! Please choose another ticker or enter EXIT to return to main menu: ";
        }
    }
}

// AAR-Ave AAR-SD CAAR-Ave CAAR-SD are stored in thrDMatrix_
void showAARandCAAR(vector<vector<vector<double>>> &thrDMatrix_){
    string type;
    cout << "Please enter the group name (BEAT, MEET, MISS) or EXIT to return to the main menu: ";
    cin >> type;
    while(type != "EXIT"){
        while(!(type == "BEAT" || type == "MEET" || type == "MISS" || type == "EXIT")){
            cout << "GROUP DOES NOT EXIST. " << endl;
            cout << "Please enter another group name (BEAT, MEET, MISS) or enter EXIT to return to main menu: ";
            cin >> type;
        }
        if(type == "EXIT") break;
        int typeIndex = 0;
        if(type == "MISS") typeIndex = 1;
        else typeIndex = 2;
        
        cout << setiosflags(ios::fixed) << setprecision(4);
        cout << "\n Type: " << type << endl;
        cout << setiosflags(ios::right) << setw(6) << " Date |" << setw(10) << "AAR-Ave" << setw(10) << "AAR-SD" << setw(10) << "CAAR-Ave" << setw(10) << "CAAR-SD" << endl;
        
        int N = thrDMatrix_[0][typeIndex].size()/2;
        
        for (int i = 0; i < thrDMatrix_[0][typeIndex].size(); i++){
            cout << setiosflags(ios::right) << setw(6) << i - N + 1 << "|";// display date
            cout << setw(10) << thrDMatrix_[0][typeIndex][i] << setw(10) << thrDMatrix_[1][typeIndex][i] << setw(10) << thrDMatrix_[2][typeIndex][i] << setw(10) << thrDMatrix_[3][typeIndex][i] << endl;
        }// 0，1，2，3 represent AAR-Ave AAR-SD CAAR-Ave CAAR-SD separately
        cout << "\nEnter another group name (BEAT, MEET, MISS) or EXIT to return to the main menu: ";
        cin >> type;
    }
}

int menuChoice(){
    int choice = -1;
    cout << "\n--------------------------- Menu ---------------------------" << endl;
    cout << "1) Change times of Sampling" << endl;
    cout << "2) Enter N and retrieve historical price data for all stocks" << endl;
    cout << "3) Pull information for one stock from one group" << endl;
    cout << "4) Show AAR, AAR-SD, CAAR and CAAR-SD for one group" << endl;
    cout << "5) Show the gnuplot graph with CAAR for all 3 groups" << endl;
    cout << "6) Exit the project" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    while(choice <= 0 || choice > 6){
        if(cin.fail()){
            cin.clear();
            cin.ignore(1024,'\n');
        }
        cout << "INPUT IS INVALID! Please enter another choice: ";
        cin >> choice;
    }
    cout << endl;
    return choice;
}

int getValidInput(int limit){
    int result;
    cin >> result;
    while(result < limit){
        if(cin.fail()){
            cin.clear();
            cin.ignore(1024,'\n');
        }
        cout << "INPUT IS INVALID! Please enter another number: ";
        cin >> result;
    }
    return result;
}

void outputToFile(vector<map<string, stock>> symbolMap_){
    string fileName;
    cout << "Enter file name: "; cin >> fileName;
    ofstream outFile;
    outFile.open(fileName + ".txt");
    
    outFile << setiosflags(ios::fixed) << setprecision(4);
    for (int n = 0; n < symbolMap_.size(); n++){
        map<string, stock>::iterator itr;
        itr = symbolMap_[n].begin();

        while (itr != symbolMap_[n].end()){
            outFile << "Stock Name : " << itr->second.getSymbol() << endl;
            outFile << setiosflags(ios::internal) << setw(10) << "Date" << " | "
                << setw(12) << "AdjClose" << "  " << setw(7) << endl;

            for (int i = 0; i < itr->second.getDates().size(); i++){
                outFile << setiosflags(ios::internal) << setw(10) << itr->second.getDates()[i] << " | "
                    << setw(12) << itr->second.getAdjCloses()[i] << setw(7) << endl;
            }
            itr++;
        }
    }
    cout << "Finish writing to file." << endl;
    outFile.close();
}

void outputCAAR(vector<double>& line1, vector<double>& line2, vector<double>& line3, int &N){
    int i = 0;
    int nIntervals = 2*N-1;// 2N return
    double* xData = (double*)malloc((nIntervals + 1) * sizeof(double)); //Date
    double* yBeat = (double*)malloc((nIntervals + 1) * sizeof(double));
    double* yMeet = (double*)malloc((nIntervals + 1) * sizeof(double));
    double* yMiss = (double*)malloc((nIntervals + 1) * sizeof(double));
    
    xData[0] = -N+1;
    double x0 = 0.0;
    for (i = 0; i < nIntervals; i++){
        x0 = xData[i];
        xData[i + 1] = x0 + 1;
    }
    
    for (i = 0; i <= nIntervals; i++){
        yBeat[i] = line1[i];
        yMeet[i] = line2[i];
        yMiss[i] = line3[i];
    }
    plotResults(xData, yBeat, yMeet, yMiss, nIntervals);
    
    free(xData);
    free(yBeat);
    free(yMeet);
    free(yMiss);
    
    xData = NULL;
    yBeat = NULL;
    yMeet = NULL;
    yMiss = NULL;
}


void plotResults(double* xData, double* yData1, double* yData2, double* yData3, int dataSize){
    FILE *gnuplotPipe, *tempDataFile; //pointer to file
    
    double x1, y1, x2, y2, x3, y3;
    int i;
    const char *tempDataFileName1 = "BEAT"; //pointer to a char
    const char *tempDataFileName2 = "MEET";
    const char *tempDataFileName3 = "MISS";
    const char* labels[] = {"set title \"CAAR for 3 Groups\"",
                            "set xlabel \"DATE\"",
                            "set ylabel \"RETURN\""}; //pointer to a char array
    
    gnuplotPipe = popen("/opt/local/bin/gnuplot", "w");//write
    if (gnuplotPipe){
        for(i = 0; i < 3; i++ ) fprintf(gnuplotPipe, "%s \n", labels[i]); //title & labels
        
        fprintf(gnuplotPipe, "plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n", tempDataFileName1, tempDataFileName2, tempDataFileName3);
        fflush(gnuplotPipe);// flushing or repositioning required 清空文件缓冲区，如果文件是以写的方式打开 的，则把缓冲区内容写入文件
        
        tempDataFile = fopen(tempDataFileName1, "w");
        for (i = 0; i <= dataSize; i++){
            x1 = xData[i];
            y1 = yData1[i];
            fprintf(tempDataFile, "%lf %lf\n", x1, y1);
        }//Write data to tempDataFile %lf double
        fclose(tempDataFile);
        
        tempDataFile = fopen(tempDataFileName2, "w");
        for (i = 0; i <= dataSize; i++){
            x2 = xData[i];
            y2 = yData2[i];
            fprintf(tempDataFile, "%lf %lf\n", x2, y2);
        }
        fclose(tempDataFile);
        
        tempDataFile = fopen(tempDataFileName3, "w");
        for (i = 0; i <= dataSize; i++){
            x3 = xData[i];
            y3 = yData3[i];
            fprintf(tempDataFile, "%lf %lf\n", x3, y3);
        }
        fclose(tempDataFile);
        
        cin.ignore();//clear all
        //remove(tempDataFileName);
        //remove(tempDataFileName2);
        //remove(tempDataFileName3);
        fprintf(gnuplotPipe, "exit \n");
    }else{
        printf("gnuplot not found...");
    }
}
