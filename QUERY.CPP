#include <fstream>
#include <iostream>

using namespace std;

#define QUERYNO 37 

int main()
{
	ifstream in("queryRel.txt", ios::in);
	ofstream out("Relevants.txt",ios::app);

	struct RelevantDocument{
		int docID;
		int rank;
	}relDoc[50000];

	out << "------------------------------ Relevants for queryNo = " << QUERYNO << " ------------ " << endl;
	
	int i = 0, counter = 0;
	while ( !in.eof() ){
		int queryNo, docNo, rank;
		
		in >> queryNo >> docNo >> rank;
		if ( queryNo != QUERYNO ){
			cout << " Relevance for queryNo " << QUERYNO << " is processed. " << endl;
			break;
		}
	    if ( rank == 0 )
			continue;
		
		relDoc[i].docID = docNo;
		relDoc[i].rank  = rank;
		i++;
	}// end while

	int relCounter = i;

	for ( int j = 0; j < i; j++ )
		for ( int k = j + 1; k < i; k++){
			if ( relDoc[j].rank < relDoc[k].rank ){
				
				struct RelevantDocument temp = relDoc[j];
				relDoc[j] = relDoc[k];
				relDoc[k] = temp;
			}
			else if ( relDoc[j].rank == relDoc[k].rank ){
				if ( relDoc[j].docID > relDoc[k].docID ){
				
					struct RelevantDocument temp = relDoc[j];
					relDoc[j] = relDoc[k];
					relDoc[k] = temp;
				}//if
			}//else
		}//for

		out << QUERYNO << " ";
		for ( int j = 0; j < i; j++)
			out << relDoc[j].docID << " " << relDoc[j].rank << " ";

		out << endl << "The number of relevant document for query " << QUERYNO << " is: " << relCounter;
		out << endl << endl;
		

	return 0;
}