// File:	 ListNode.h
//
// Purpose:  Linked List Node template class difinition.
//
//
// Auther:   Hassan Bashiri, April 2004 (Farvadin 1383)

#ifndef LISTNODE_H
#define LISTNODE_H
#include <string.h>
class List;  // forward declaration

class TermNode {
    friend class List;  // make List a friend
public:
	TermNode () : docList(0) {};
    TermNode (const unsigned int &, const char *);   // constructor
    ~TermNode () {};
    unsigned int getTerm() const;      // return termID in the node
    char* getdocsList() const;    // retrurn documents list and tf of a termID
private:
    unsigned int termID;                // data
    char *docList;
    TermNode *nextPtr;  // next node in the list
   
};

// Constructor
TermNode::TermNode (const unsigned int &Term, const char *docIDtf)
{
	termID = Term;
	docList = new char[strlen(docIDtf)+1];
	strcpy(docList, docIDtf);
	nextPtr = NULL;
}
   

// Return a copy of the data in the node
unsigned int TermNode::getTerm() const { return termID; }

char* TermNode::getdocsList() const { return docList; }

#endif