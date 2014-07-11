#ifndef INPUT_STREAM
#define INPUT_STREAM

#include <fstream>

using namespace std;

class inputData {

	ifstream fuserDetails ;
	ifstream fuserInterests ;
	ifstream fuserFriends ;
	ifstream fclusterheader;
	ifstream fcluster;
	string userDetails ;
	string userInterests;
	string userFriends;
	bool goAhead;
public:
	inputData();
	/* Open Files for Userdetails and UserInterests*/
	void openFiles();

	/* */
	void getStrings();

	void seekFileEnd(ifstream & , string &);

	string refineString(string &);

	string readString_baseCase(ifstream &);
	string readUserInterests_baseCase();
	string getUserInterests() ;
	string getUserDetails();
	void closeFiles();
	string getString(const int x);
	void openCluster();
	void closeCluster();

	void writeCluster(string str);
	string readCluster(int position);
	void readCluster(int positionOfCluster,double & mean);
	void writeToClusterAt(int positionOfCluster,string str,double val);
	void writeToClusterAt( int ithCluster , string encoded );
	void sortClusterFile();
	string findUser(int,double);
	void openClusterHeader();
	void closeClusterHeader();
	void clearClusterData();
	string readClusterHeader(); 
	bool returnGoAhead();
	void addToEncodedFile(string);
};

#endif