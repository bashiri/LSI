#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>

int main()
{

	ifstream docfreq("docFreq.txt", ios::in);
	if ( !docfreq ){
		cout << " Error: document frequency file couldn't open." << endl;
		return 0;
	}

	int dfArray[78896];       // there are 78895 terms in collection
	char str[20];
	while ( !docfreq.eof() ){
		docfreq.getline(str, 20, '\n');
		if ( strlen(str) < 2 ){
			cout << "There is empty line in document frequency file. " << endl;
			continue;
		}

		char *termIDstr = str;
		char *dfStr = strchr( termIDstr, ' ');
		*dfStr = '\0';
		dfStr++;

		int tID = atoi(termIDstr);
		int df = atoi(dfStr);
		dfArray[tID] = df;
	}// this have done for n parameter in query term weighting
	docfreq.close();

    char queryString[1000];    //max 1000 characters for query 
	
	ifstream queryfile("queries.txt", ios::in);
	if ( !queryfile ){
		cout << " Error: Queries.txt couldn't open." << endl;
		return 0;
	}

	ofstream queryTerms("dfquery.txt", ios::out);
	if ( !queryTerms ){
		cout << " Error: dfQureries.txt couldn't open." << endl;
		return 0;
	}

	while ( !queryfile.eof() ){
		int queryNo = 0;
		queryfile >> queryNo;
		queryfile.getline(queryString, 1000, '\n');
		if ( strlen(queryString) < 2  ){
			cout << " Entered query is empty." << endl;
			continue;
		}

		queryTerms << queryNo ;

		for ( unsigned int i = 0; i <= strlen(queryString); i++)     // Ignore space at the first of query
			queryString[i] = queryString[i + 1];
		
		char* tokenPtr = strtok(queryString, " ");   //used to parse query terms
		while( tokenPtr != NULL ){             //while there are tokens in queryString
			int termID = atoi(tokenPtr);
			if (termID > 0 && termID <= 77895){
				queryTerms << " (" << termID << "," << dfArray[termID];
				if ( dfArray[termID] > 100 )
					queryTerms << "-SW";
				queryTerms << ")";
			}
		    else
			    cout << " Term with ID: " << termID << " not exist in collection." << endl;
	        tokenPtr = strtok(NULL, " ");
		}// end of while

		queryTerms << endl;
	}

	queryTerms.close();
	queryfile.close();

	return 0;
}
