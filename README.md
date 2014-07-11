#Socialize 

###A clustering project that clusters users into groups on the basis of their interests. Clustering is implemented using k-means algorithm


###Credits : Me and Anshit Agarwal (a.anshit@gmail.com)


###Instructions
An already built result is in clusterData.txt.tar.bz2

First extract all .tar.bz2s using any archiving utility.

Then 

> g++ -c inputStream.cpp 
> g++ -c utilites.cpp 
> g++ -c cluster.cpp 

Finally 
> g++ test.cpp cluster.o inputStream.o utilites.o -o test
>./test

