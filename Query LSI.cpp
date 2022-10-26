//	File: QueryLSI.cpp
//	Purpose: LSI SDD project Query Processor  
//	Date written: September 2004
//	Author: Hassan Bashiri (hbashiri@acm.org)
//
//  Inputs: termDoc.sdd (output from SDDPACK) 
//          termCount.dat (output from Preparation program)
//			termIndex.dat (output from Preparation program)
//
//	Outputs:
//			Displays ranked list of matching documents and precision measures
//	

#include <time.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctype.h>
#include <iomanip>
#include "string1.h" // String class  
#include "matrix.h"  // Matrix class 	 
#include "list.h"    // Linked List class
#include "math.h"

using namespace std;

#define slope 0.256
#define Pivot 35.2735

int main()
{

	int intNoDocs      = 0; //total documents this collection
	int intTermCount   = 0;	//Counter variable for looping
	int intCurrentTerm = 0;	//Counter variable for looping
    int intCurrentRank = 0;	//Counter variable for looping
	int intValue       = 0; //Holds each element of integer matices Xk, Yk 
	int intCurrentQuery= 0;	//Counter variable for looping
	int intCurrentDoc  = 0;	//Counter variable for looping


	ifstream inSDDFile("TermDoc.sdd",ios::in);
	if (!inSDDFile){
		cerr << "TermDoc.sdd File could not be opened"  << endl;
		exit(1);
	}

	cout << "Qavanin SDD LSI engine results.  Hassan Bashiri 2004." << endl;
	cout << "Reading in term document matrix (may take a few seconds)..." << endl;
	char string[1000];
	
	//ignore the first two comment lines in the sdd file
	inSDDFile.getline(string,999,'\n');
	inSDDFile.getline(string,999,'\n');
	
	int intRank = 0;
	int intNoTerms = 0;
	int intExp = 0;
	double dblValue = 0;
	char tempDVal[27];
	char charExp[3];
	

	intNoDocs = 0; 
	//the third sdd file line contains rank, terms and documents
	inSDDFile >> intRank >> intNoTerms >> intNoDocs;
  	cout << endl << "This matrix has: rank: " << intRank << " m:" << intNoTerms << " n:" << intNoDocs << endl;

	Matrix<double> SDD_X(intNoTerms,intRank);
	Matrix<double> SDD_D(intRank,intRank);
	Matrix<double> SDD_Y(intNoDocs,intRank);
	Matrix<double> SDD_Y_t(intRank, intNoDocs);
	Matrix<double> SDD_X_t(intRank,intNoTerms);

	int i, j = 1;
	int intCheckValues = 0;

	//read in the values for the Dk matrix.  These values are formatted
	//in scientific notation, and require a little processing.  Note the
	//Dk matrix entries are inserted diagonally.
	i = 1;
	cout << "Reading in Dk matrix" << endl;
	for (i; i <= intRank; i++){
		inSDDFile >> string;
		for (int j=0;j<27;j++)
			tempDVal[j] = string[j];
		//extract exponent for this value
		charExp[0] = string[29];
		charExp[1] = string[30];
		charExp[2] = string[31];
		intExp = atoi(charExp);
		dblValue = atof(tempDVal);
		if ( intExp > 0 ) 
			dblValue = dblValue / pow(10, intExp);
		SDD_D(i,i) = dblValue;
		intCheckValues++;
		dblValue = 0;
		intExp = 0;
	} //end of for
	
	cout << "Read in " << intCheckValues << " values for Dk matrix. "  << endl;
	intCheckValues = 0;

	//Read in values for the Xk matrix - (Term x Rank)
	//Each line is one column of entries in the matrix
	intValue = 0;
	cout << "Reading in X matrix" << endl;
	for (i=1; i <= intRank; i++)
		for (j=1; j<=intNoTerms; j++){
			inSDDFile >> intValue;
			SDD_X(j,i) = intValue;	
			intCheckValues++;	
		}// end of for

	cout << "Read in " << intCheckValues << " values for Xk matrix."  << endl;
	intCheckValues = 0;

	//Read in values for the Y matrix  (Rank * Docs)
	//Each line is one column of entries in the matrix
	cout << "Reading in Y matrix" << endl;
	intValue = 0;
	for (i=1; i <= intRank; i++)
		for (j=1; j<=intNoDocs; j++){
			inSDDFile >> intValue;
			SDD_Y(j,i) = intValue;
			intCheckValues++;	
		}// end of for

	cout << "Read in " << intCheckValues << " values for Yk matrix."  << endl;
	inSDDFile.close();	
	
	//Transpose X to get X_t which is required for query processing. 
	for (i=1; i <= intRank; i++) 
		for (j=1; j<=intNoTerms; j++)
			if 	(SDD_X(j,i) != 0)
				SDD_X_t(i,j) = SDD_X(j,i);

	//transpose Y to get Y_t which is required for query processing. 
	for (i=1; i <= intRank; i++) 
		for (j=1; j<=intNoDocs; j++)
			if 	(SDD_Y(j,i) != 0)
				SDD_Y_t(i,j) = SDD_Y(j,i);

	//The following three lines can be used to output the reconstructed
	//rank k matrix A_k.  This can be useful for testing and comparing the 
    //approximation with the original term document matrix (or SVD):
	//Matrix<double> SDD_All(intNoTerms,intNoDocs);
	//SDD_All = SDD_X * SDD_D * SDD_Y_t;
	//SDD_All.saveSparseMatrix("testout.sdd");

	//--------------------------------------------------------------------
	//Read in the term index and insert values into linked list
	//--------------------------------------------------------------------
	
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

    char queryString[10000];    //max 1000 characters for query 

	ifstream queryfile("queries.txt", ios::in);
	if ( !queryfile ){
		cout << " Error: Queries.txt couldn't open." << endl;
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

		for ( i = 0; i <= strlen(queryString); i++)     // Ignore space at the first of query
			queryString[i] = queryString[i + 1];           

	    cout << endl << "The entered query is: " << queryString << endl;
		
		Matrix<double> SDD_Q(intNoTerms,1);    //used as a weighted query vector
		Matrix<double> rawQuery(intNoTerms,1); //query vector with raw term freq only.	
		
		char* tokenPtr = strtok(queryString, " ");   //used to parse query terms
		while( tokenPtr != NULL ){             //while there are tokens in queryString
			int termID = atoi(tokenPtr);
			if (termID > 0 && termID <= intNoTerms)
				//if ( dfArray[termID] < 100 && dfArray[termID] > 0 )  
					rawQuery(termID, 1) += 1;
				//else
					//cout << " Term with ID: " << termID << " is a stop word and has been ignored." << endl;
		    else
			    cout << " Term with ID: " << termID << " not exist in collection." << endl;
	        tokenPtr = strtok(NULL, " ");
		}// end of while

		int NUT = 0;      //number of unique terms in query
		for (i = 1; i <= intNoTerms; i++){
			if ( rawQuery(i, 1) == 1 )
				NUT++;
		}
	
		for (intCurrentTerm = 1; intCurrentTerm <= intNoTerms; intCurrentTerm++) // query term weighting
			if ( rawQuery( intCurrentTerm, 1 ) != 0){
				  double qTermWeight = (log(rawQuery (intCurrentTerm, 1)) + 1) * //ln(tf)+1
					                         log( intNoDocs / dfArray[intCurrentTerm] ) / //ln(N/n)
											 ( (slope * NUT) + ((1 - slope) * Pivot) );
				  SDD_Q(intCurrentTerm, 1) = qTermWeight;
			}

		// query term weighting have been done
        // now we are going to calculate similarity between query and documents

		ifstream relevance("Relevants.txt", ios::in);

		int qNo, flag = 0;
		while ( !relevance.eof() ){
			relevance >> qNo;
			relevance.getline(queryString, 10000, '\n');

			if (qNo == queryNo){
				flag = 1;
				break;
			}
		}// while

		if ( !flag ){
			cout << "There are no relevance documents in RELEVANCE.TXT.";
			cout << "Rest of the process couldn't be continue.";
			relevance.close();
			return 0;
		}

		for ( i = 0; i <= strlen(queryString); i++)     // Ignore space at the first of query
			queryString[i] = queryString[i + 1];

		int counter = 0;
		char* docIDPtr = queryString;
		int relevantList[10000];

		for ( ; ; ){
			char* rankPtr = strchr(docIDPtr, ' ');
			if ( rankPtr == NULL)
				break;
			*rankPtr = '\0';
			rankPtr++;

			int docID = atoi(docIDPtr);
			relevantList[counter++] = docID;
			docIDPtr = strchr(rankPtr, ' ');
			docIDPtr++;
		}
		relevantList[counter] = 0;         // close relevant list
		relevance.close();

		double cosine = 0.0;                          //similarity result
		int intRankSize = 100;                        //how many documents do we want in query results
		double* dblResults = new double[intRankSize]; //ranked list of top cosine values
		int* intResultDoc = new int[intRankSize];     //ranked list of top documents
		double dblThreshold = -100.0;                 //any cosine less than this value will not be displayed
		int intMatch = 0;                             //contains array index where new value needs to be entered

		//initialise ranking arrays
		for (i = 0; i < ( intRankSize - 1 ); i++){
			dblResults[i] = 0.000000;
			intResultDoc[i] = 0;
		}
   
		Matrix<double> SDD_A(intRank, intNoDocs);
		Matrix<double> SDD_newQ(intRank,1);	
		Matrix<double> SDD_newQ_t(1,intRank);
		Matrix<double> SDD_Cosine(intNoDocs,1);
		Matrix<double> SDD_Result(1,1);

		//first matrix required to compute similarity with query vector
   		SDD_A =  SDD_D * SDD_Y_t;

   	    //normalize Matrix SDD_A (intRank x intNoDocs)

		/*double dblSumTermFreq = 0;
		double dblNormValue = 0;
		for (intCurrentDoc = 1; intCurrentDoc <= intNoDocs; intCurrentDoc++){
			dblSumTermFreq = 0;
			dblNormValue = 0;
			for (intCurrentRank = 1; intCurrentRank <= intRank; intCurrentRank++)
				dblSumTermFreq += (SDD_A(intCurrentRank,intCurrentDoc) * SDD_A(intCurrentRank,intCurrentDoc));

			dblNormValue = sqrt(dblSumTermFreq);
	    	for (intCurrentRank = 1; intCurrentRank <= intRank; intCurrentRank++)
				if (SDD_A(intCurrentRank,intCurrentDoc) != 0)
					SDD_A(intCurrentRank,intCurrentDoc) = (SDD_A(intCurrentRank,intCurrentDoc) / dblNormValue);
		}// end of for*/

		//project query vector in the same space as term-doc matrix
		SDD_newQ = SDD_X_t * SDD_Q;

		//transpose SDD_newQ vector
		for (intCurrentRank = 1; intCurrentRank <= intRank; intCurrentRank++)
			SDD_newQ_t(1,intCurrentRank) = SDD_newQ(intCurrentRank,1);

		//calculate similarity for each document
		SDD_Cosine = SDD_newQ_t * SDD_A;

		ofstream out("similarity.txt", ios::app);
		if ( !out ){
			cerr << "Similarity file couldn't open. " << endl;
			exit(1);
		}
	
		
	    //insert cosine into result list in rank order
		if (cosine > dblThreshold){  
			intMatch = -1;
			for (i = 0; i < intRankSize; i++){
				if (cosine >= dblResults[i]){
					intMatch = i;
					break; 
				}// end of if
			}// end of for
			if (intMatch >= 0){				
				//shift elements to the right and insert the new value
				for (j = intRankSize - 1; j >= intMatch; j--){
					dblResults[j+1] = dblResults[j];
					intResultDoc[j+1] = intResultDoc[j];	
				}// end of for
				//insert new value
				dblResults[intMatch] = cosine;
				intResultDoc[intMatch] = intCurrentDoc;	
			}// end of if
		}// end of if

		//The following code is used to calculate various precision measures.
		int intRelID = 0;    
		int intHitCount= 0;  //counter for the number of correct docs retrieved
		double dblRecall = 0.0; 
    
		double* dblPrecisionPercent = new double[11];  //for 11 point int-ave-precision
		double* dbl25_50_75Percent = new double[3];    //for 25/50/75 average precision

		//initialise arrays and local variables
		for (i = 0; i < 11; i++)
			dblPrecisionPercent[i] = 0;

		for (i = 0; i < 3; i++)
			dbl25_50_75Percent[i] = 0;

		double dblPrecision = 0.0;
		double dblAvePrecision = 0.0;
		double dblSumPrecision = 0.0;
		double dblRPrecision = 0.0;

		int intRelDocTotal = 0;
		List < int > relList;


		//Output details for each document in descending
		//order of similarity.  
		for (i = 0; i < intRankSize  ; i++){
			cout << "Rank : " << i + 1 << " Document: " << intResultDoc[i] <<  " Cosine: " << dblResults[i];  
			intRelID = relList.find(intResultDoc[i]);
			if (intRelID > 0)
				intHitCount++;

			if (intHitCount> 0 && intHitCount <= intRelDocTotal){
				dblRecall = (double)intHitCount / (double)intRelDocTotal;
				dblPrecision = (double)intHitCount / ((double)i+1);
				if (intRelID > 0)
					dblSumPrecision += dblPrecision;
			}//end of if

			cout << " P = " << dblPrecision;
			cout << " R = " << dblRecall ;
			cout << " Hit total: " << intHitCount	<< endl;

			if (dblRecall <= 0.25)
				dbl25_50_75Percent[0] = dblPrecision;
			else if (dblRecall <= 0.50)
				dbl25_50_75Percent[1] = dblPrecision;
			else if (dblRecall <= 0.75)
				dbl25_50_75Percent[2] = dblPrecision;

			//100% recall  used for R-Precision
			if (intHitCount == intRelDocTotal)
				if ( dblPrecision > dblPrecisionPercent[10] )
					dblPrecisionPercent[10] = dblPrecision;

			//set 11 point interpolated average components
			if (dblRecall <= 0.10)
				if ( dblPrecision > dblPrecisionPercent[0] )
					dblPrecisionPercent[0] = dblPrecision;
			else if (dblRecall > 0.10 && dblRecall <= 0.20) 
				if ( dblPrecision > dblPrecisionPercent[1] )
						dblPrecisionPercent[1] = dblPrecision;
			else if (dblRecall > 0.20 && dblRecall <= 0.30)
				if ( dblPrecision > dblPrecisionPercent[2] )
						dblPrecisionPercent[2] = dblPrecision;
			else if (dblRecall > 0.30 && dblRecall <= 0.40)
				if ( dblPrecision > dblPrecisionPercent[3] )
						dblPrecisionPercent[3] = dblPrecision;
			else if (dblRecall > 0.40 && dblRecall <= 0.50)
				if ( dblPrecision > dblPrecisionPercent[4] )
						dblPrecisionPercent[4] = dblPrecision;
			else if (dblRecall > 0.50 && dblRecall <= 0.60)
				if ( dblPrecision > dblPrecisionPercent[5] )
						dblPrecisionPercent[5] = dblPrecision;
			else if (dblRecall > 0.60 && dblRecall <= 0.70)
				if ( dblPrecision > dblPrecisionPercent[6] )
						dblPrecisionPercent[6] = dblPrecision;
			else if (dblRecall > 0.70 && dblRecall <= 0.80)
				if ( dblPrecision > dblPrecisionPercent[7] )
						dblPrecisionPercent[7] = dblPrecision;
			else if (dblRecall > 0.80 && dblRecall <= 0.90)
				if ( dblPrecision > dblPrecisionPercent[8] )
						dblPrecisionPercent[8] = dblPrecision;
			else if(dblRecall > 0.90 && dblRecall <= 1)
				if ( dblPrecision > dblPrecisionPercent[9] )
					dblPrecisionPercent[9] = dblPrecision;
		}// end of for

		//calculate interpolated average precision and output results
		//for this query.
		for (i = 0; i < 11; i++)
			dblAvePrecision = dblAvePrecision + dblPrecisionPercent[i];
		dblAvePrecision = dblAvePrecision / 11.0;
		cout << "Interpolated Average Precision for Query " << intCurrentQuery << " = " << dblAvePrecision << endl;	
		cout << "Non-InterpolatedPrecision for for Query " << intCurrentQuery << " = " << dblSumPrecision / (double) intRelDocTotal << endl;
		cout << "R_Precision = " << dblPrecisionPercent[10] << endl;
		cout << "Average based on mean of 25,50 and 75 percent recall: " <<  ((dbl25_50_75Percent[0]  + dbl25_50_75Percent[1] + dbl25_50_75Percent[2])/3.0) << endl;
		cout << "Precision at Recall levels 0 to 10" << endl;
		cout << "******************************************" << endl;
		for (i = 0; i < 11; i++)
			cout << dblPrecisionPercent[i] << endl;
		cout << "******************************************" << endl;
	}
	queryfile.close();
 
	return 0;
}
