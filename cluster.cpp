#include "cluster.hpp"
#include "utilities.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;
cluster::cluster(){
	numOfClusters = 5000;
	string userString="";
	numOfUsers = 175000;
	//Open Files in INPUT STREAM	
}

void cluster::initializeCentriods(){

	//iObj.openCluster();
	iObj.clearClusterData();
	srand(time(NULL));
	for(int i=0;i<numOfClusters;i++){
		iObj.openFiles();
		int x = i + 1;
		centroids[i].nameCentroid=iObj.getString(x);
	
		centroids[i].nameCentroid=centroids[i].nameCentroid.substr(centroids[i].nameCentroid.find(',')+1);
        centroids[i].nameCentroid=centroids[i].nameCentroid.substr(centroids[i].nameCentroid.find(',')+2);
        centroids[i].nameCentroid=centroids[i].nameCentroid.substr(0,centroids[i].nameCentroid.find('"'));
        
      
        string value="cluster"+to_string(i+1)+" "+centroids[i].nameCentroid;
      	iObj.writeCluster(value);
        
		iObj.closeFiles();
	}
	//iObj.closeCluster();

}

void cluster::computeCentriods(){

	double mean =0;
	remove("clusterHeader.txt");
	for(int i=0;i<numOfClusters;i++){
		iObj.openCluster();
		iObj.readCluster(i+1,mean);
		iObj.closeCluster();
		//cout<<i+1<<" "<<mean<<endl;
		
		iObj.openCluster();
		string encoded = iObj.findUser(i+1,mean);
		iObj.closeCluster();
		iObj.writeToClusterAt(i+1,encoded);	
		centroids[i].nameCentroid=encoded;	
	}
	remove("clusterData.txt");
	string cmd = "cp clusterHeader.txt clusterData.txt";
	system(cmd.c_str());
}

void cluster::doKmeans(){

	bool somePointIsMoving=true;
	cout<<"Init"<<endl;
	iObj.openFiles();
	double d,max=0;
	int positionOfCluster=-1;
	string clusterString,clusterString2,userString2;
	
	initializeCentriods();	
	cout<<"Partition"<<endl;

	for(int i=0 ; i<numOfUsers ; i++){
		cout<<i<<endl;
		max=0;
		userString=iObj.readUserInterests_baseCase();
		userString2=userString;

		userString2.erase(remove_if(userString2.begin(), userString2.end(),[](char x){return ::isspace(x);}),userString2.end());
    	
    	userString=userString.substr(userString.find(',')+1);
   	    userString=userString.substr(userString.find(',')+2);
       	userString=userString.substr(0,userString.find('"'));
       	
       	for(int j=0 ; j<numOfClusters ; j++){
       		//iObj.openCluster();
			//clusterString=iObj.readCluster(j);
			//clusterString=clusterString.substr(clusterString.find(' ')+1);
			clusterString=centroids[j].nameCentroid;
			
			d=uObj.cosSim(userString,clusterString);
			//iObj.closeCluster();	
			if(d>max){
				max=d;
				positionOfCluster=j;
				clusterString2=clusterString;
				if(max>.9999999){
					break;
				}
			}
		}
		//cout<<positionOfCluster<<endl;
		iObj.openCluster();
		//My heart is not aching anymore
		iObj.writeToClusterAt(positionOfCluster+1,userString2,max);
		iObj.closeCluster();
		
	}
	iObj.openCluster();
	iObj.sortClusterFile();
	iObj.closeCluster();
	computeCentriods();
	
	iObj.closeFiles();
	int iterator =0;
	cout<<"Assignment"<<endl;
	while(somePointIsMoving && iterator<20)
	{
		iterator++;
		
		iObj.openFiles();	
		for(int i=0 ; i<numOfUsers ; i++){
			max=0;
			cout<<iterator<<'\t'<<i<<endl;
			userString=iObj.readUserInterests_baseCase();
			userString2=userString;
			userString2.erase(remove_if(userString2.begin(), userString2.end(),[](char x){return ::isspace(x);}),userString2.end());
    		
    		userString=userString.substr(userString.find(',')+1);
   		    userString=userString.substr(userString.find(',')+2);
       		userString=userString.substr(0,userString.find('\"'));
        	
        	iObj.openClusterHeader();
			
        	for(int j=0 ; j<numOfClusters ; j++){
				//clusterString=iObj.readClusterHeader();
				//clusterString=clusterString.substr(clusterString.find(' ')+1);
				clusterString=centroids[j].nameCentroid;
			
				d=uObj.cosSim(userString,clusterString);
			
				if(d>max){
					max=d;
					positionOfCluster=j;
					clusterString2=clusterString;
				}
			}
			iObj.closeClusterHeader();
			iObj.openCluster();
			iObj.writeToClusterAt(positionOfCluster+1,userString2,max);
			iObj.closeCluster();
		}
		iObj.openCluster();
		iObj.sortClusterFile();
		iObj.closeCluster();
		//iObj.returnGoAhead()
		if(iterator!=20){
			computeCentriods();
		}
		iObj.closeFiles();	
	}
}

void cluster::addNewUserCluster(string newUserInterests){
	string line=newUserInterests;
	//Get the encoded intrests of the new user
	string lineCopy=line;
    line=line.substr(line.find(',')+1);
    line=line.substr(line.find(',')+2);
    line=line.substr(0,line.find('"'));
    line=uObj.hashString(line);
    int positionOfCluster=-1;
	string clusterString,clusterString2;
	
    int i=0,counteR=0;
    while(true){
        if(lineCopy[i]==',')
            counteR++;
        if(counteR==2)
            break;
        i++;
    }
        
    lineCopy=lineCopy.substr(0,i);
    lineCopy=lineCopy+",\""+line+"\"\n";
    iObj.closeFiles();
    iObj.addToEncodedFile(lineCopy);
      
	iObj.openClusterHeader();

	double d,max=0;
	for (int i = 0; i < numOfClusters; ++i){
		clusterString=iObj.readClusterHeader();
		clusterString=clusterString.substr(clusterString.find(' ')+1);
		
		d=uObj.cosSim(line,clusterString);
		if(d>max){
			max=d;
			positionOfCluster=i;
			clusterString2=clusterString;
		}
	}
	iObj.closeClusterHeader();
	lineCopy.erase(remove_if(lineCopy.begin(), lineCopy.end(),[](char x){return ::isspace(x);}),lineCopy.end());

	iObj.openCluster();
	iObj.writeToClusterAt(positionOfCluster+1,lineCopy,max);
	iObj.closeCluster();
	iObj.openCluster();
	iObj.sortClusterFile();
	iObj.closeCluster();
}
