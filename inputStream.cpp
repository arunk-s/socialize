#include <iostream>
#include <string>
#include <cstdio>
#include "inputStream.hpp"
#include <cstdlib>
#include <algorithm>
#include <cmath>
using namespace std;

inputData::inputData(){
	userDetails= "";
	userInterests = "";
	userFriends= "";
	goAhead=true;
}


void inputData::openFiles(){
	fuserDetails.open("userdetails.csv",ios::in);
	fuserInterests.open("userinterestsEncoded.csv",ios::in);
	//fuserFriends.open("userdetails.csv",ifstream::in);
}

void inputData::getStrings(){

	if(fuserFriends.good()){
		seekFileEnd(fuserFriends,userFriends);
	}
	if(fuserInterests.good()){
		seekFileEnd(fuserInterests,userInterests);
	}
	if(fuserDetails.good()){
		seekFileEnd(fuserDetails,userDetails);
	}
}

void inputData::seekFileEnd(ifstream & fin,string & line){
   if(fin.is_open()) {
        fin.seekg(-1,ios_base::end);                // go to one spot before the EOF

        bool keepLooping = true;
        while(keepLooping) {
            char ch;
            fin.get(ch);                            // Get current byte's data

            if((int)fin.tellg() <= 1) {             // If the data was at or before the 0th byte
                fin.seekg(0);                       // The first line is the last line
                keepLooping = false;                // So stop there
            }
            else if(ch == '\n') {                   // If the data was a newline
                keepLooping = false;                // Stop at the current position.
            }
            else {                                  // If the data was neither a newline nor at the 0 byte
                fin.seekg(-2,ios_base::cur);        // Move to the front of that data, then to the front of the data before it
            }
        }

        getline(fin,line);                      // Read the current line
	}
}

string inputData::readString_baseCase(ifstream & file){
	string s1 ;
	if(!(file>>s1))
	 	return string("");
	else
		return s1;

}

string inputData::getUserInterests(){
	return userInterests;
}

string inputData::getUserDetails(){
	return userDetails ;
}

string inputData::readUserInterests_baseCase(){
	if(!(getline(fuserInterests,userInterests)))
	 	return string("");
	else{
		return userInterests;
	}

}


void inputData::closeFiles(){
	fuserInterests.close();
	fuserDetails.close();
	//fuserFriends.close();
}

void inputData::clearClusterData(){
	remove("clusterData.txt");
}

string inputData::getString(const int x){
	int counter = 0; 
	string line ;
	while(counter!=x){
		if(!(getline(fuserInterests,line))){
			return string("");
		}
		counter++;
	}
	return line;
}

void inputData::openCluster(){
	fcluster.open("clusterData.txt",ios::in);	
}
void inputData::closeCluster(){
	fcluster.close();
}
void inputData::openClusterHeader(){
	fclusterheader.open("clusterHeader.txt",ios::in);
}
void inputData::closeClusterHeader(){
	fclusterheader.close();
}

void inputData::writeCluster(string str){
	ofstream fclusterData;
	fclusterData.open("clusterData.txt",ios::app);	
	str=str+"\n";
	fclusterData<<str;	
	fclusterData.close();
}

void inputData::addToEncodedFile(string linecopy){
	ofstream fuserInterestsEncoded("userinterestsEncoded.csv", ios::app);
	fuserInterestsEncoded<<linecopy;
	fuserInterestsEncoded.close();
}

string inputData::readCluster(int position){ 
	string line ;
	int counter=-1;

	while(true){
		if(!(getline(fcluster,line)))
				return string("");
			else{
				if(line.substr(0,7)=="cluster")
					counter++;
				if(counter==position)
					return string(line);
		}
	}
}

string inputData::readClusterHeader(){ 
	string line;
	if(!(getline(fclusterheader,line)))
		return string("");
	else
		return line;
}

void inputData::readCluster(int positionOfCluster,double & mean){
	string line ;
	int counter=0;
	int countUser=0;
	double sum = 0;
	while(true){
		if(!(getline(fcluster,line))){
			mean=0;
			return;
		}
		else{
				if(line.substr(0,7)=="cluster")
					counter++;
				if(counter==positionOfCluster){
					countUser=0;
					while(getline(fcluster,line)){
						if(line.substr(0,7)=="cluster"){
							mean=sum/countUser;
							mean=std::isnan(mean)?0:mean;
							if(countUser<15)
								goAhead=false;
							return;
						}
						else{
							countUser++;
							line=line.substr(line.find(" "));
							double d = atof(line.c_str());
							sum+=d;
						}
					}
				} 
		}
	}
}

bool inputData::returnGoAhead(){
	return goAhead;
}

void inputData::writeToClusterAt(int positionOfCluster,string str,double val){
	string line ;
	int counter=0;
	ofstream file("clusterData2.txt", ios::out);

	while(true){
		if(!(getline(fcluster,line)))
				break;
			else{
			line+="\n";
			file<<line;
			if(line.substr(0,7)=="cluster"){
				counter++;
				if(counter==positionOfCluster){
				//	cout<<str<<" "<<val<<"\n";
					file<<str<<" "<<val<<"\n";
				}
			}
		}
	}
	file.close();
	remove("clusterData.txt");
	rename("clusterData2.txt","clusterData.txt");
}
//For Updating the cluster ...
void inputData::writeToClusterAt( int ithCluster , string encoded ){
	ofstream file("clusterHeader.txt", ios::app);
	file<<"cluster"<<ithCluster<<" "<<encoded<<"\n";
	file.close();
}			

void inputData::sortClusterFile(){					
	string line;
	remove("cluster2.txt");
	while(true){
		int length=0;

		if(!(getline(fcluster,line)))
				break;
			else{
				if(line.substr(0,7)=="cluster"){
					ofstream newClusterFile("cluster2.txt",ios::app);
					newClusterFile<<line<<"\n";			
					ofstream file("ToBeSorted.txt",ios::out);

					while(getline(fcluster,line)){
						
						if(line.substr(0,7)=="cluster"){
							file.close();
							length=line.length()+1;
							
							string cmd = "sort -r -nk2 ToBeSorted.txt > ToBeSorted2.txt";
							system(cmd.c_str());
							
							ifstream afile("ToBeSorted2.txt",ios::in);
							string sortedString;			
							
							while(true){
								if(!getline(afile,sortedString))
									break;
								else{
									newClusterFile<<sortedString<<"\n";
								}
							}
							fcluster.seekg(-length,ios::cur);

							afile.close();
							newClusterFile.close();
							break;
						}
						else
							file<<line<<"\n";
					}
					file.close();
				}
			}
	}

	string cmd = "sort -r -nk2 ToBeSorted.txt > ToBeSorted2.txt";
	system(cmd.c_str());
	
	ofstream newClusterFile("cluster2.txt",ios::app);
	ifstream afile("ToBeSorted2.txt",ios::in);
	string sortedString;
	while(true){		
		if(!getline(afile,sortedString))
			break;
		else
			newClusterFile<<sortedString<<"\n";
	}
	afile.close();
	newClusterFile.close();
	remove("clusterData.txt");
	rename("cluster2.txt","clusterData.txt");

}
	
string inputData::findUser(int position,double mean){
	string line,lineCopy ;
	int counter=0;
	while(true){
		if(!(getline(fcluster,line))){
			return string("");
		}
		else{
				if(line.substr(0,7)=="cluster"){
					lineCopy=line;
					counter++;
				}
				if(counter==position){
					while(getline(fcluster,line)){
						if(line.substr(0,7)=="cluster"){
							lineCopy=lineCopy.substr(lineCopy.find(" ")+1);
							return lineCopy;
						}
						lineCopy=line;
						line=line.substr(line.find(" "));
						double d = atof(line.c_str());

						if(mean>=d){
							lineCopy=lineCopy.substr(0,lineCopy.find(" "));
							lineCopy=lineCopy.substr(lineCopy.find(",")+1);
							lineCopy=lineCopy.substr(lineCopy.find(",")+2);
							lineCopy=lineCopy.substr(0,lineCopy.length()-1);
							return lineCopy;
						}
					}
					cout<<"er2";
					break;
				}
		} 
	}
	/*
	lineCopy=lineCopy.substr(0,line.find(" "));
	lineCopy=lineCopy.substr(line.find(","));
	lineCopy=lineCopy.substr(line.find(","));
	lineCopy=lineCopy.substr(line.find("\""));
	lineCopy=lineCopy.substr(0,line.length()-1);
	return lineCopy;*/
	lineCopy=lineCopy.substr(lineCopy.find(" ")+1);
	return lineCopy;					
}

