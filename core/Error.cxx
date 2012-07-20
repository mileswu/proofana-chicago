/**************************************************************************
 **
 **   File:         Error.cxx
 **
 **   Description:  Error handling functions
 **                 
 **
 **   Author:       B. Butler
 **
 **   Created:      2-25-12
 **   Modified:
 **
 **************************************************************************/

#ifndef Error_cxx
#define Error_cxx

#include "Error.h"
#include "TProofServ.h"
#include "TError.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void Abort(const char* msg) {

	cout << msg << endl;

	if(gProofServ) {
		gProofServ->SetLastMsg(msg);
		gProofServ->HandleException(1);
	}
	else exit(1);
}

#endif
