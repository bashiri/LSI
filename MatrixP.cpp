
// Author: Hassan Bashiri, April 2004, (Farvardin 1382)

#include <iostream> 
#include <fstream>
#include <stdlib.h>
#include <string>
#include <math.h>

using namespace std;

#define MAX   1000000
#define TERMS 78896 // there are 78895 term in collection
#define DOCS  177090 // there are 177090 documents in collection

const double Pivot  = 35.2735;
const double Slope  = 0.25;
const char blank[2] = " ";

struct CollectionParameters{
	int NoUW;
	double logAvgtf;
};

int main()
{
	ifstream dfFile("docFreq.txt",ios::in);
	if ( !dfFile ){
		cerr << "doc Frequency file could not be opened."  << endl;
		exit(1);
	}

	ifstream collParam("collectionParameters.txt",ios::in);
	if ( !collParam ){
		cerr << "Collection Parameters file could not be opened."  << endl;
		exit(1);
	}

	char collLine[50] = "";		       // reading a line from input file
	long int len;
	long int *Terms = new long int[TERMS];

	while ( !dfFile.eof() ){				// filling Terms array with df file
		dfFile.getline(collLine, 20, '\n'); 
		len = strlen(collLine);
		if ( len < 2 ){
			cout << "Empty line is read. (df file)" << endl;
			continue;
		} // Line #lineNo is read.

		char *termID = collLine;                  // specify termID
		char *docf = strchr(termID, ' ');     // find blank to seperate docID 
		*docf= '\0';                          // close blank to fix termID as a string
		docf++;
		
		//filling the array
		long int tID = atoi(termID);
		long int df = atoi(docf);
		Terms[tID] = df;
	}// while
	dfFile.close();
   
	struct CollectionParameters *collPArray = new struct CollectionParameters[DOCS];

	while ( !collParam.eof() ){			   	  // filling collPArray array with collParam file
		collParam.getline(collLine, 50, '\n'); 
		len = strlen(collLine);
		if ( len < 2 ){
			cout << "Empty line is read. (collParam file)" << endl;
			continue;
		} // Line #lineNo is read.

		char *docID = collLine;                     // specify docID
		char *nuwTerm = strchr(docID, ' ');     // find blank to seperate nuwTerm 
		*nuwTerm= '\0';                         // close blank to fix docID as a string
		nuwTerm++; 

		long int dID = atol(docID);

		char *avgTF = strchr(nuwTerm, ' ');          // specify average tf
		*avgTF = '\0';                          // close blank to fix docID as a string
		avgTF++;
		
		//filling the array
		int NoUW = atoi(nuwTerm);
		double averageTF = atof(avgTF);

		collPArray[dID].NoUW = NoUW;
		collPArray[dID].logAvgtf = log(averageTF);

	}// while
	collParam.close();

	cout << " We are going to matrix preparation. "<< endl;
	cout << " It might take a long time." << endl;

	ifstream index("Index.txt",ios::in);
	if ( !index ){
		cerr << "Index file could not be opened."  << endl;
		exit(1);
	}

/*	ofstream termDoc("termDoc.mtx",ios::out); 
	if ( !termDoc ){
		cerr << "Errors file could not be created."  << endl;
		exit(1);
	}*/

	ofstream sddTermDoc("termDocSDD.txt", ios::out);
	if ( !sddTermDoc ){
		cerr << "Errors file could not be created."  << endl;
		exit(1);
	}
	
	char line[MAX] = "";		       // reading a line from input file

	while ( !index.eof() ){
		index.getline(line, MAX, '\n'); 
		len = strlen(line);
		if ( len < 2 ){
			cout << "Empty line is read. " << endl;
			continue;
		} // Line #lineNo is read.

		char *termID = line;
		char *restofLine = strchr(termID, ' ');
		*restofLine = '\0';
		restofLine++;            // specify termID

		if ( len > MAX - 5 )
			cerr << "Very large line is read. termID = " << termID << " Lenght = " << len << endl;

		int tID = atoi(termID);
		if ( Terms[tID] == 0 )
			continue;

//		termDoc << termID;

		char *docID = restofLine;
		if ( !docID ){
			cerr << "Error: TermID is read but there is no rest of line.";
			cout << "termID = " << termID <<endl;
			exit(1);
		}

		for ( ; *docID != '\n' && *docID != '\0'; ){

			char *tf = strchr(docID, ' ');
			*tf = '\0';
			tf++;
			
			long int dID = atoi(docID);

			sddTermDoc << termID << ' ' << docID;
			
			docID = strchr(tf, ' ');
			*docID = '\0';
			docID++;

			int termf = atoi(tf);

			// Weighting schema is Lnu for collection terms
			double termWeight = ( (1 + log(termf)) / (1 + collPArray[dID].logAvgtf) )
				                    / ( (1 - Slope) * Pivot + (Slope * collPArray[dID].NoUW) );
			char buffer[20];
			_gcvt( termWeight, 8, buffer );
			
			sddTermDoc << ' ' << buffer << endl;
		}//for
	}//while

	index.close();
	sddTermDoc.close();
	
	cout << "Matrix Preparation component is finished." << endl;
	cout << "See termDocSDD.mtx file ......" <<endl;

	return 0;
}
