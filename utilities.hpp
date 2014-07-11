#ifndef UTILITIES
#define UTILITIES

#include <string>
using namespace std;
class utilities {
	
	string dictionary[1000];
	string hashDictionary[1000];
	int dictionaryLength; //Length of bag of words
	char dictionaryIDF[1000];
	double valuesIDF[1000];
	int vectorLength; //Lenght of stored tf-idf

public:
	utilities();
	void readDictionary();
	void addToFile(const string value , const string hash);
	string hashValue(const string word);
	string hashString(string str);
	size_t levenshteinDistance(const string &lhs , const string &rhs);
	size_t find_min(size_t x,size_t y,size_t z);
	int tf(char);
	float idf(char);
	double cosSim(string int1 ="abc" , string int2 ="xyz" );
};

#endif