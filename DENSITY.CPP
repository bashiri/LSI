
// Author: Hassan Bashiri, April 2004, (Farvardin 1382)

#include <iostream.h> 
#include <fstream.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define ROW 77896  // there is 77895 terms
#define COL 177090 // there is 177089 documents

int main(void)
{
	ifstream infile("termDoc.mtx",ios::in);
	if (!infile){
		cerr << "Input file could not be opened"  << endl;
		exit(1);
	}

	long int rowMtx[ROW] = {0};
	long int colMtx[COL] = {0};

	int nonzero = 0;
	int row, col;
	double weight;
	while ( !infile.eof() ){
		infile >> row >> col >> weight;
		if ( row == 0 || col == 0 ){
			cout << "error. " << endl;
			break;
		}
		nonzero++;
		rowMtx[row]++;
		colMtx[col]++;
	}

	int sum = 0;
	float MuR = 0;
	float MuC = 0;
	for (int i = 1; i < ROW; i++)
		sum = sum + rowMtx[i];
	MuR = (float)(sum) / ( ROW - 1 );

	sum = 0;
	for(i = 1; i < COL; i++)
		sum = sum + colMtx[i];
	MuC = (float)(sum) / ( COL - 1 );

	ofstream out("output.txt", ios::out);

	out << "MuR: " << MuR << endl;
	out << "MuC: " << MuC << endl;

	float density = (float)( MuR * MuC ) / nonzero;
	out << "Density: " << density << endl;

	out << " Processing is finished." << endl;
	infile.close();
	out << endl << " Non-Zero elements: " << nonzero << endl;
	return 0;
}