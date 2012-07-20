/**************************************************************************
 **
 **   File:         SelectorBase.h
 **
 **   Description:  Selector interface class
 **                 
 **
 **   Author:       B. Butler
 **
 **   Created:      12-10-11
 **   Modified:
 **
 **************************************************************************/

#ifndef SelectorBase_h
#define SelectorBase_h

#include "Key.h"

class TFile;
class Config;

class SelectorBase {
public :

	SelectorBase() { }
	virtual 			~SelectorBase() { }
    virtual TFile*		GetFile(AnaKey /*name */) {/* to be overridden by derived class */ return 0;}
    
	
	ClassDef(SelectorBase,0);
	
private :

};

#endif
