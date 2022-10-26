
// Author: Hassan Bashiri, April 2004, (Farvardin 1382)

#include <iostream.h> 
#include <fstream.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20
#define LASTBound 78896 //there are 78895 term in collection

long int Terms[LASTBound];

int main()
{
	ifstream dfFile("df-Sorted.txt",ios::in);
	if ( !dfFile ){
		cerr << "doc Frequency file could not be opened."  << endl;
		exit(1);
	}

	ofstream termDist("termDist.txt",ios::out); 
	if ( !termDist ){
		cerr << "Output file could not be created."  << endl;
		exit(1);
	}
	
	char line[MAX] = "";		       // reading a line from input file
	long int len;
	int counter[8] = {0};              // these counters count the terms in defined rang

	while ( !dfFile.eof() ){
		dfFile.getline(line, MAX, '\n'); 
		len = strlen(line);
		if ( len < 2 ){
			cout << "Empty line is read. " << endl;
			continue;
		} // Line #lineNo is read.

		char *termID = line;                  // specify termID
		char *docf = strchr(termID, ' ');     // find blank to seperate docID 
		*docf= '\0';                          // close blank to fix termID as a string
		docf++;
		
		//filling the array
		long int tID = atoi(termID);
		long int df = atoi(docf);

		if ( df >= 100000)
			counter[7]++;
		else if ( df >= 10000 && df < 100000 )
			counter[6]++;
		else if ( df >= 1000 && df < 10000 )
			counter[5]++;
		else if ( df > 100 && df < 1000 )
			counter[4]++;
		else if ( df >= 50 && df <= 100 )
			counter[3]++;
		else if ( df > 1 && df < 50 )
			counter[2]++;
		else if ( df == 1 )
			counter[1]++;
		else if ( df == 0 )
			counter[0]++;
		else {
			cerr << " There is no defined range for df " << tID << endl;
			exit(1);
		}// else
	}// while

	dfFile.close();

	termDist << " -------------- Term Disteribution in Qavanin Collection --------------- " << endl;
	termDist << " df >= 100,000          : " << counter[7] << endl;
	termDist << " 10,000 <= df < 100,000 : " << counter[6] << endl;
	termDist << " 1,000 <= df < 10,000   : " << counter[5] << endl;
	termDist << " 100 < df < 1,000       : " << counter[4] << endl;
	termDist << " 50 <= df <= 100        : " << counter[3] << endl;
	termDist << " 1 < df < 50            : " << counter[2] << endl;
	termDist << " df = 1                 : " << counter[1] << endl;
	termDist << " df = 0                 : " << counter[0] << endl;


	// I have to insert weighting method here*/

	dfFile.close();
	termDist.close();
	
	cout << "See termDist.txt file ......" <<endl;

	return 0;
}
