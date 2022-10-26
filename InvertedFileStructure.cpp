
// Author: Hassan Bashiri, April 2004, (Farvardin 1382)

#include <iostream> 
#include <fstream>
#include <conio.h>
#include <stdlib.h>
#include <string>
//#include <time.h>

using namespace std;


#define IMAX 35000
#define OMAX 950000

int main(void)
{

	ofstream indexFile("Indexes.txt",ios::out); 
	if (!indexFile){
		cerr << "Index file could not be created"  << endl;
		exit(1);
	}

	int termCounter = 42141;
	const char blank[2] = " ";
//  char lastTmpbuf[128], nextTmpbuf[128];


/*	_tzset();
	_strtime( lastTmpbuf );             //	Display operating system-style date and time.
	cout << "OS time:\t" << lastTmpbuf << endl;*/

   
	while ( termCounter < 50000 ){     // there are 78895 terms in collection
	
		char line[IMAX] = "";		   // reading a line from input file
		int  lineNo = 0;			   // line counter and term counter
		char index[OMAX] = "";         // output string for output file
	    char buffer[10];
	
		_itoa( termCounter, buffer, 10 );

		strcat(index, buffer);
		strcat(index, blank);
		
		//Read Items.txt from Qavanin collection to create "Inverted File" structure
		ifstream itemsFile("Items.txt",ios::in);
		if (!itemsFile){
			cerr << "Items file could not be opened"  << endl;
			exit(1);
		}

		while ( !itemsFile.eof() ){
			itemsFile.getline(line, IMAX, '\n'); 
			lineNo++;
			int len = strlen(line);
			if ( len < 2 ){
				cout << "Empty line is read. " << endl;
				continue;
			} // Line #lineNo is read.
			
			char *docID = line;                  //specify docID
			char *next = strchr(docID, ' ');     //find blank to seperate docID 
			if ( !next){
				cerr << "Input format is wrong.\n";
				return 0;
			}
			*next = '\0';                        //close blank to fix docID as a string
			next++;

			char *temp = strchr(next, ' ');     //reject doc lenght
			if ( !temp){
				cerr << "Input format is wrong.\n";
				return 0;
			}
			*temp = '\0';
			temp++;
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
								
				if ( !strcmp(termID, buffer) ){
					strcat(index, docID);
					strcat(index, blank);
					strcat(index, tf);
					strcat(index, blank);
					break;
				}
			}//for
		}//while file reading

		itemsFile.close();		
		indexFile << index << endl;
		cout << "Term " << termCounter << " Processed." << endl;
		termCounter++;

/*		_strtime( nextTmpbuf );
		cout << "OS time:\t" << nextTmpbuf  << endl;*/

	}//while, Term Processing

	cout << " Processing is finished." << endl;
	indexFile.close();
	cout << " Index File closed." << endl;

	return 0;
}