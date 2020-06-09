//
//  getData.h
//  TeamProject
//
//  Created by Lynn on 5/14/20.
//  Copyright © 2020 NYU. All rights reserved.
//

#ifndef getData_h
#define getData_h

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include <fstream>
#include "curl/curl.h"

using namespace std;

void populateSymbolVector(vector<string>& symbols);

// write to file
int write_data(void* ptr, int size, int nmemb, FILE* stream);

void* myrealloc(void* ptr, size_t size);

// use memory allocation so that c++ program could use the data
int write_data2(void* ptr, size_t size, size_t nmemb, void* data);
int getData(void);

#endif /* getData_h */
