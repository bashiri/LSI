// File:	 Preparation.cpp
//
// Purpose:  Generates term-document matrix files for Latent Semantic Indexing (SDD) Engine.       
// Input: 
//       1. Items.txt (file exist in Qavanin collection.
// Input file structure:   docID docLen n wordList
//       docID = document id number
//       docLen = document length but this number is incorrect!
//       n = number of words in the document	
//       wordList = a list of n pairs, each pair has (word_id, tf)
// Outputs:  
//       1. TERMDOC.MTX (the term doc matrix in 'MatrixMarket matrix coordinate real 
//							 general format')
// Author: Hassan Bashiri, April 2004, (Farvardin 1382)

#include <iostream.h> 
#include <fstream.h>
#include <conio.h>
#include "list.h"    // Linked List class

int main(void)
{

	//Read Items.txt from Qavanin collection to create "Inverted File" structure
	ifstream itemsFile("Items.txt",ios::in);
	if (!itemsFile){
		cerr << "Items file could not be opened"  << endl;
		exit(1);
	}

	char line[500] = "";
	unsigned int termID;
	List TermsList;
    int lineNo = 0; // counter

	while ( lineNo < 5 ){ //!itemsFile.eof() ){
		itemsFile.getline(line, 500, '\n');
		lineNo++;
		int len = strlen(line);
		if (line[len-1] == '\n')
			line[len-1] = '\0';
		char *docID = line;
		char *temp = strchr(docID, ' ');
		*temp = '\0';
		for ( ; *temp != ' '; temp++ );
		*temp = '\0';
		temp++;
		char* num = temp;
		temp = strchr(num, ' ');
		*temp = '\0';
		temp++;
		unsigned int n = atoi (num);
        for (int i = 0; i < n; i++){
			char *term = temp;
			temp = strchr(term, ' ');
			*temp = '\0';
			temp++;
			termID = atoi (term);
			char *tf = temp;
			temp = strchr(tf, ' ');
			*temp = '\0';
			temp++;

			char *docIDtf = "";
			strcpy(docIDtf, docID);
			strcat(docIDtf," ");
			strcat(docIDtf, tf);
			strcat(docIDtf, " ");
			TermsList.insertNode (termID, docIDtf);
		}
    }			
	itemsFile.close();

    //Output file to save Index term as an inverted file structure
	ofstream indexFile("Index.txt",ios::out); 
	if (!indexFile){
		cerr << "Index file could not be created"  << endl;
		exit(1);
	}

	TermsList.printoFile(indexFile);
	return 0;
}