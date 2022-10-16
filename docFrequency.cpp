
// Author: Hassan Bashiri, April 2004, (Farvardin 1382)

#include <iostream.h> 
#include <fstream.h>
#include <stdlib.h>
#include <string.h>

#define MAX 951000
#define TER 949000


int main()
{

	ifstream termsFile("Index.txt",ios::in);
	if (!termsFile){
		cerr << "Items file could not be opened."  << endl;
		exit(1);
	}

	ofstream dfFile("docFreq.txt",ios::out); 
	if (!dfFile){
		cerr << "Document frequency file could not be created."  << endl;
		exit(1);
	}

	ofstream errorFile("errorList.txt",ios::out); 
	if (!errorFile){
		cerr << "Errors file could not be created."  << endl;
		exit(1);
	}
	
	char blank[2] = " ";
	long int  lineNo = 0;		       // line counter and term counter
	char line[MAX] = "";		       // reading a line from input file

	//Read Items.txt from Qavanin collection to create "Inverted File" structure
	while ( !termsFile.eof() ){
		termsFile.getline(line, MAX, '\n'); 
		lineNo++;
		long int len = strlen(line);
		if ( len < 2 ){
			cout << "Empty line is read. " << endl;
			continue;
		} // Line #lineNo is read.

		char *termID = line;                  // specify termID
		char *temp1 = strchr(termID, ' ');    // find blank to seperate docID 
		*temp1 = '\0';                        // close blank to fix termID as a string
		temp1++;

		if ( atoi(termID) != lineNo){         // this shows that we forgot one term
			cerr << "Alram, there is no match between termID: " << termID << " and termNo: " << lineNo << endl;
			exit(0);
		}

		int counter = 0;

		char dfStr[20];
		
		strcpy(dfStr, termID);
		strcat(dfStr, blank);

		for (unsigned int i = 0; *temp1 ; i++){
			char *temp2 = strchr(temp1, ' ');
			temp2++;
			temp1 = strchr(temp2, ' ');
			temp1++;
			counter++;
		}// for

		char buffer[10];
		_itoa(counter, buffer, 10);

		if ( len > TER ){
			errorFile << termID << ' ' << len << ' ' << buffer << endl;
		}

		strcat(dfStr, buffer);

		dfFile << dfStr << endl;
		
	}// while
	
	dfFile.close();
	errorFile.close();
	termsFile.close();
	
	cout << " See errorList.txt and decFreq.txt for details ....... " << endl;

	return 0;
}
