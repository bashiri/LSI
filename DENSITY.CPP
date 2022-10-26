
// Author: Hassan Bashiri, April 2004, (Farvardin 1382)


#include <iostream> 
#include <fstream>
#include <conio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

using namespace std;

#define MAX 950000
#define NumofDoc 177089

int main(void)
{

	ofstream paramFile("CollectionParameters.txt",ios::out); 
	if (!paramFile){
		cerr << "Output file could not be created"  << endl;
		exit(1);
	}

	paramFile << "docID No.UniqueTerms average(tf)" << endl;
	
	ifstream itemsFile("Items.txt",ios::in);
	if (!itemsFile){
		cerr << "Items file could not be opened"  << endl;
		exit(1);
	}

	char line[MAX] = "";		   // reading a line from input file
	long int len = 0;
	double Pivot = 0;

	//Read Items.txt from Qavanin collection 

	cout << "Please wait ............." << endl;
	while ( !itemsFile.eof() ){
		itemsFile.getline(line, MAX, '\n'); 
		int len = strlen(line);
		if ( len < 2 ){
			cout << "Empty line is read. " << endl;
			continue;
		} // Line #lineNo is read.
			
		if ( len > MAX ){
			cout << "Very large is read. " << endl;
			exit(1);
		} 

		int nuwCounter = 0;
		float avgTF = 0;
		int sumoftf = 0;
		char *docID = line;                  //specify docID
		char *next = strchr(docID, ' ');     //find blank to seperate docID 
		*next = '\0';                        //close blank to fix docID as a string
		next++;

		char *temp = strchr(next, ' ');     // doc lenght
		if ( !temp){
			cerr << "Input format is wrong.\n";
			return 0;
		}
		*temp = '\0';
		temp++;

		double docLen = atof(next);
		Pivot =  Pivot + docLen;

		next = temp;
		
		char* num = next;                    //specify  n             
		next = strchr(num, ' ');
		if ( !next){
			cerr << "Input format is wrong.\n";
			return 0;
		}
		*next = '\0';
		next++;
		unsigned int n = atoi (num);

		for (unsigned int i = 0; i < n ; i++){		
			char *termID = next;             //specify termID
			if ( !(*termID) )
				break;
			next = strchr(termID, ' ');      //find blank to seperate termID    
			if ( !next){
				cerr << "Input format is wrong.\n";
				return 0;
			}
			*next = '\0';                    //close blank to fix termID as a string
			next++;
		
			char* tf = next;		    	 //specify term frequency
			next = strchr(tf, ' ');
			if ( !next){
				cerr << "Input format is wrong.\n";
				return 0;
			}
			*next = '\0';
			next++;

			int termf = atoi(tf);
			sumoftf = sumoftf + termf;
			if ( termf == 1 )
				nuwCounter++;								
		}// for

		if ( n != 0 )
			avgTF = (float)sumoftf / n;
		paramFile << docID << ' ' << nuwCounter << ' ' << avgTF << endl;
	}//while file reading

	Pivot = Pivot / NumofDoc;

	cout << endl;
	cout << "Pivot = " << Pivot << endl;
	itemsFile.close();		
	paramFile.close();

	cout << " Processing is finished." << endl;
	return 0;
}
