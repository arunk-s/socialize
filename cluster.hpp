#ifndef CLUSTER
#define CLUSTER
#include "inputStream.hpp"
#include "utilities.hpp"
#include <vector>
#include <fstream>

using namespace std;

struct centroid {
	string nameCentroid ;
};


class cluster {

	string userString;
	double cosineSimilarity;
	int numOfClusters ;
	long int numOfUsers;	
	
	struct centroid centroids[6000];
	ofstream outputCluster ;
	inputData iObj ;
	utilities uObj;
public:
	cluster();
	void initializeCentriods();
	void computeCentriods();
	void doKmeans() ;
	void dumpOutput();
	void suggestFriends(string);
	void addNewUserCluster(string);
};

#endif