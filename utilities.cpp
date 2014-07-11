#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>
#include "utilities.hpp"
#include <vector>

using namespace std;

utilities::utilities(){
    dictionaryLength=0;
    vectorLength=0;
//    cout<<cosSim("arunkumarsori","anshitaggarwal");
}

int utilities::tf( char term )
{
    return 1;
}

float utilities::idf( char term )
{
    for(int i=0 ; i<vectorLength ; i++)
    {
        if( term == dictionaryIDF[i])
        {
            return valuesIDF[i];
        }
    }

    ifstream myfile ("userinterestsEncoded.csv");
    string line;
    long int counterWord = 0,counterFile = 0;

    while (! myfile.eof() ) //while the end of file is NOT reached
    {
        if(!getline (myfile,line))
            break; //get one line from the file

        counterFile++;

        if( line.find( term ) != line.npos)
        {
            counterWord++;
        }
    }
    
    dictionaryIDF[vectorLength] = term;
    valuesIDF[vectorLength] = log((counterFile*1.0)/counterWord);
    vectorLength++;

    return log((counterFile*1.0)/counterWord);
}

double utilities::cosSim( string int1 , string int2)
{
    double dotp=0, maga=0, magb=0;
    int l1=int1.length();
    int l2=int2.length();
    l1=min(l1,l2);
    for(int i=0 ; i<l1 ; i++ )
    {
        int loc = int2.find(int1[i]);
        if(loc == -1)
        {
            dotp+=0;
            maga+=pow( tf(int1[i])*idf(int1[i]) ,2 );
            magb+=pow( 0 , 2 );
        }
        else
        {
            dotp+=tf(int1[i])*idf(int1[i]) * tf(int2[i])*idf(int2[i]);
            maga+=pow(tf(int1[i])*idf(int1[i]),2);
            magb+=pow(tf(int2[i])*idf(int2[i]),2);
        }

    }
    //cout<<"Out";
    maga = sqrt(maga);
    magb = sqrt(magb);
    double d = dotp / (maga * magb);
    return (std::isnan(d)?0:d);
}

//To add old data to the dictionary in the file
void utilities::readDictionary()
{
    int i=0;
    ifstream file;
    file.open("dictionary.txt",ios::in);

    while(true)
    {
        if(!(file>>dictionary[i]))
                break;
        if(!(file>>hashDictionary[i]))
            break;

      //To print the entire dictionary
      //  cout<<dictionary[i]<<"  "<<hashDictionary[i]<<endl;
        i++;
    }

    file.close();
    dictionaryLength=i;

}

//To add new data to the file on the file system
void utilities::addToFile(string value , string hash)
{
    ofstream file;
    file.open("dictionary.txt",ios::app);

    file<<value<<" "<<hash<<" ";
    file.close();
}

//Create a HashValue for a new Word
string utilities::hashValue(string word)
{
    char _value=(char)(dictionaryLength+48);
    //string value=static_cast<string> _value;
    string value=string(&_value,1);
    return value;
}

//Assign hashes to complete strings
string utilities::hashString(string str)
{
    readDictionary();
    string word;
    int location1,location2;
    string finalHash;

    location1=0;
    location2=str.find(" ");

    //Take up each word and assign its hash value
    while(location2!=-1)
    {
        int flag=0;
        location2=str.find(" ");
        word=str.substr(location1,location2);
        str=str.substr(location2+1);

       for(int i=0;i<dictionaryLength;i++)
       {
           //If the word already exists, assign it the hash value defined previously
           if(dictionary[i]==word)
                {
                    flag=1;
                    finalHash+=hashDictionary[i];
                    break;
                }
       }
       //If the word does not exist in dictionary
       //assign it a new hash value, then add the value
       if(flag==0)
            {
                dictionary[dictionaryLength]=word;
                hashDictionary[dictionaryLength]=hashValue(word);
                //To add the word to the Dictionary on File System
                addToFile(dictionary[dictionaryLength] , hashDictionary[dictionaryLength]);

                finalHash+=hashDictionary[dictionaryLength];
                //Increment the length of the dictionary
                dictionaryLength++;
            }

    }
    //Sort the string by size
    sort(finalHash.begin(), finalHash.end());
    return finalHash;
}

size_t utilities::find_min(size_t x,size_t y,size_t z){
    if( x < y )
        return x < z ? x : z ;
    else
        return y < z ? y : z ;
}

size_t utilities::levenshteinDistance(const std::string &lhs , const std::string &rhs){
    size_t l_size = lhs.size();
    size_t r_size = rhs.size();

    if(l_size == 0)
        return r_size;
    if(r_size == 0)
        return l_size;
    std::vector<std::vector<size_t> > matrix(l_size+1,std::vector<size_t>(r_size+1));

    matrix[0][0] = 0;
    for (size_t i=1; i < l_size; ++i){
        matrix[i][0]=i;
    }
    for (size_t i = 0; i < r_size; ++i){
        matrix[0][i]=i;
    }
    for(size_t i =1 ;i<= l_size ;++i)
        for (size_t j = 1; j <=r_size ; ++j){
            size_t x = matrix[i-1][j] + 1;
            size_t y = matrix[i][j-1] + 1;
            size_t z = matrix[i-1][j-1] + (lhs[i-1] == rhs[j-1] ? 0 : 1);
            matrix[i][j] = find_min(x,y,z);
        }

    return matrix[l_size][r_size];
}
