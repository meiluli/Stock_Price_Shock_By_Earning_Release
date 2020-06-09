//
//  getData.cpp
//  TeamProject
//
//  Created by Lynn on 5/14/20.
//  Copyright © 2020 NYU. All rights reserved.
//

#include <stdio.h>
#include "getData.hpp"

const char* cSP500SymbolFile = "SP500_component_stocks.csv";
// pointer of char

void populateSymbolVector(vector<string>& symbols)
{
    ifstream fin;
    fin.open(cSP500SymbolFile, ios::in); //ios::in allows input (read operations) from a stream
    string line, name, symbol;
    while (!fin.eof())// end of file(eof)
    {
        getline(fin, line);
        stringstream sin(line); //主要用在将一个字符串分开
        getline(sin, name, ',');//','is a delimiter, ','前的是name变量
        getline(sin, symbol); //'\n'前的是symbol
        symbols.push_back(symbol);
    }
}

// write to file
int write_data(void* ptr, int size, int nmemb, FILE* stream)
//ptr is a pointer to data
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);//把 ptr 所指向的数组中的数据写入到给定流 stream 中
    /*
     ptr -- 这是指向要被写入的元素数组的指针。
     size -- 这是要被写入的每个元素的大小，以字节为单位。
     nmemb -- 这是元素的个数，每个元素的大小为 size 字节。
     stream -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输出流。
     */
    return (int)written;
}
struct MemoryStruct
{
    char* memory;//get data from libcurl;write to memory not file；faster
    size_t size;//bytes of dynamic allocation memory
};

void* myrealloc(void* ptr, size_t size)
{
    if (ptr)
        return realloc(ptr, size);//尝试重新调整之前调用 malloc 或 calloc 所分配的 ptr 所指向的内存块的大小。该函数返回一个指针 ，指向重新分配大小的内存。如果请求失败，则返回 NULL。
    else
        return malloc(size);
    //C 库函数 void *malloc(size_t size) 分配所需的内存空间，并返回一个指向它的指针。返回一个指针 ，指向已分配大小的内存。如果请求失败，则返回 NULL。
}

// use memory allocation so that c++ program could use the data
int write_data2(void* ptr, size_t size, size_t nmemb, void* data) //symbolList中每个stock会有一个新的data，循环结束再free data
{
    size_t realsize = size * nmemb; //新的数据的大小
    struct MemoryStruct* mem = (struct MemoryStruct*)data; //首先把之前存好的data复制给mem
    mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1); // 给mem分配可以储存新数据的内存
    //If not +1, would have an off-by-one error a few lines later
    if (mem->memory)
    {
        memcpy(&(mem->memory[mem->size]), ptr, realsize); //将新的传来的数据复制到memory中
        //void *memcpy(void *str1, const void *str2, size_t n) 从存储区 str2 复制 n 个字节到存储区 str1
        mem->size += realsize; //mem的size也需要增加
        mem->memory[mem->size] = 0;
    }
    return (int)realsize;
}
int getData(void)
{
    remove("Results.txt");
    vector<string> symbolList;
    populateSymbolVector(symbolList);
    symbolList.push_back("SPY");
    
    // file pointer to create file that store the data
    FILE* fp;

    const char resultfilename[FILENAME_MAX] = "Results.txt";

    // declaration of an object CURL
    CURL* handle;

    CURLcode result;

    // set up the program environment that libcurl needs
    curl_global_init(CURL_GLOBAL_ALL);

    // curl_easy_init() returns a CURL easy handle
    handle = curl_easy_init();

    // if everything's all right with the easy handle...
    if (handle)
    {
        string url_common = "https://eodhistoricaldata.com/api/eod/";
        string start_date = "2019-04-30";
        string end_date = "2020-04-30";
        string api_token = "5ba84ea974ab42.45160048";

        vector<string>::iterator itr = symbolList.begin();
        for (; itr != symbolList.end(); itr++)
        {
            struct MemoryStruct data;
            data.memory = NULL;
            data.size = 0;

            string symbol = *itr;
            if (symbol.back() == '\r')//has moved to the leftmost column,'\r' is a carriage return
                symbol.pop_back();
            string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
            curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
            // return the address of first character of the string

            //adding a user agent
            curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            //不验证ssl证书
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
            //设置libcurl不验证host, 忽略host验
            fp = fopen(resultfilename, "ab");
            //打开或新建一个二进制文件，只允许在文件末尾追写
            fprintf(fp, "%s\n", symbol.c_str());
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
            result = curl_easy_perform(handle);
            fprintf(fp, "%c", '\n');
            fclose(fp);

            // check for errors
            if (result != CURLE_OK)
            {
                //if errors have occurred, tell us what is wrong with result
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }

            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

            // perform, then store the expected code in 'result'
            result = curl_easy_perform(handle);

            if (result != CURLE_OK)
            {
                // if errors have occured, tell us what is wrong with result
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }

            stringstream sData; //streamstring在调用str()时，会返回临时的string对象。而因为是临时的对象，所以它在整个表达式结束后将会被析构
            sData.str(data.memory);
            string sValue, sDate;
            double dValue = 0;
            
            string line;
//            cout << symbol << endl;;
            
            ofstream fout;
            if (getline(sData, line) && (line.substr(0, 4) == "Date"))
            {
                fout.close();
                fout.open(symbol + ".dat");//storage single stock data into .dat file
                cout << symbol << " creates file successfully" <<endl;
            }
            else if (! getline(sData, line))
            {
                cout << symbol <<" does not have data" << endl;
            }
            
            while (getline(sData, line))
            {
                size_t found = line.find('-');//return the position of the first character of the first match.
                //If no matches were found, the function returns string::npos.
                if (found != std::string::npos) //until the end of the string
                {
                    sDate = line.substr(0, line.find_first_of(','));
                    // find_first_of:for one single character of the sequence to match
                    // sDate is date
                    line.erase(line.find_last_of(','));//delete after the last ',', including ','
                    sValue = line.substr(line.find_last_of(',') + 1);
                    // adj close
                    dValue = strtod(sValue.c_str(), NULL);
                     // double strtod(const char *str, char **endptr) 把参数 str 所指向的字符串转换为一个浮点数（类型为 double 型）。如果 endptr 不为空，则指向转换中最后一个字符后的字符的指针会存储在 endptr 引用的位置。
//                    cout << sDate << " " << std::fixed << ::setprecision(2) << dValue << endl;
                    fout << sDate << " " << std::fixed << ::setprecision(6) << dValue << endl;
                }
            }

            free(data.memory);
            data.size = 0;
        }
        
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }

    // cleanup since you've used curl_easy_init
    curl_easy_cleanup(handle);

    // release resources acquired by curl_global_init()
    curl_global_cleanup();

    return 0;
}
