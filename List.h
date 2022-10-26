// File:	 List.h
//
// Purpose:  Linked List template class.  Used for processing terms 
//           from document collection.
//
// Written by:   Hassan Bashiri, April 2004 (Farvadin 1383) for 
//               use in a Latent Semantic Indexing (SDD) application.   

#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <cassert>
#include "ListNode.h"

class List {
public:
   List();      // constructor
   ~List();     // destructor
   void insertAtFront( const unsigned int  &, const char * );
   void insertAtBack( const unsigned int &, const char * );
   int insertNode(const unsigned int &, const char * );
   TermNode* findTerm( const unsigned int & );
   bool isEmpty() const;
   void printoFile(ofstream &) const;
private:
   TermNode *firstPtr;  // pointer to first node
   TermNode *lastPtr;   // pointer to last node

   // Utility function to allocate a new node
   TermNode *getNewNode( const unsigned int &, const char * );
};

// Default constructor
List::List() : firstPtr( 0 ), lastPtr( 0 ) { }

// Destructor
List::~List()
{
   if ( !isEmpty() ) {    // List is not empty
      cout << "Destroying nodes ...\n";

      TermNode *currentPtr = firstPtr, *tempPtr;

      while ( currentPtr != 0 ) {  // delete remaining nodes
         tempPtr = currentPtr;
		 currentPtr = currentPtr->nextPtr;
         delete tempPtr;
      }
   }

   cout << "All nodes destroyed\n\n";
}

// Insert a node at the front of the list
void List::insertAtFront( const unsigned int &value, const char *docIDtf )
{
   TermNode *newPtr = getNewNode( value, docIDtf );

   if ( isEmpty() )  // List is empty
      firstPtr = lastPtr = newPtr;
   else {          // List is not empty
      newPtr->nextPtr = firstPtr;
      firstPtr = newPtr;
   }
}

// Insert a node at the back of the list
void List::insertAtBack( const unsigned int &value, const char *docIDtf )
{
   TermNode *newPtr = getNewNode( value, docIDtf );

   if ( isEmpty() )  // List is empty
      firstPtr = lastPtr = newPtr;
   else {          // List is not empty
      lastPtr->nextPtr = newPtr;
      lastPtr = newPtr;
   }
}

int List::insertNode(const unsigned int &termID, const char *docIDtf )
{
	if ( isEmpty() ){
		TermNode *newPtr = getNewNode ( termID, docIDtf );
		firstPtr = lastPtr = newPtr;
		return 1;
	}
	if ( termID < firstPtr->termID ){
		insertAtFront(termID, docIDtf);
		return 1;
	}
	if (termID > lastPtr->termID){
		insertAtBack(termID, docIDtf);
		return 1;
	}
	TermNode *currentNode, *beforeNode;
	currentNode = beforeNode = firstPtr;
	TermNode *newPtr = getNewNode(termID, docIDtf);
	while ( currentNode ){
		if ( currentNode->termID == termID ){
			strcat ( currentNode->docList, docIDtf);
			return 1;
		}
		if ( currentNode->termID < termID ){
			beforeNode = currentNode;
			currentNode = currentNode->nextPtr;
		}
		else{
			newPtr->nextPtr = currentNode;
			beforeNode->nextPtr = newPtr;
			return 1;
		}
	}
	return 0; //unsuccessful insertion
}

TermNode* List::findTerm( const unsigned int &value )
{
   TermNode *currentPtr = firstPtr;
	
   while ( currentPtr != 0 ) {
	   if ( currentPtr->termID == value )
		   return currentPtr;
	   currentPtr = currentPtr->nextPtr; 
   }
	
   return NULL; //no match

}


// Is the List empty?
bool List::isEmpty() const 
   { return firstPtr == 0; }

// Return a pointer to a newly allocated node
TermNode *List::getNewNode( const unsigned int &value, const char *docIDtf )
{
   TermNode *ptr = new TermNode( value, docIDtf );
   assert( ptr != 0 );
   return ptr;
}

// Display the contents of the List
void List::printoFile(ofstream &output) const
{
   if ( isEmpty() ) {
      output << "The list is empty\n\n";
      return;
   }

   TermNode *currentPtr = firstPtr;

   cout << "Writing to file ... \n";
   while ( currentPtr != 0 ) {
	   output << currentPtr->termID << ' ' << currentPtr->docList << endl;
      currentPtr = currentPtr->nextPtr;
   }
   cout << "File is written successfull.\n";
}

#endif