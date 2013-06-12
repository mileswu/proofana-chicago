#ifndef __PROOFANA__
#define __PROOFANA__

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all defined_ines;
#pragma link off all functions;
#pragma link C++ nestedclasses;
#pragma link C++ all typedefs;

//Physics objects
#pragma link C++ class Particle+;
#pragma link C++ class Point+;

//Object storage classes
#pragma link C++ class Key<0>-;
#pragma link C++ class Key<1>-;
#pragma link C++ class Key<2>-;
#pragma link C++ class KeyObj<0>+;
#pragma link C++ class KeyObj<1>+;
#pragma link C++ class KeyObj<2>+;
#pragma link C++ class MapObj+;
#pragma link C++ class BoolObj+;
#pragma link C++ class UIntObj+;
#pragma link C++ class IntObj+;
#pragma link C++ class LongObj+;
#pragma link C++ class FloatObj+;
#pragma link C++ class DoubleObj+;
#pragma link C++ class StringObj+;
#pragma link C++ class ObjVecObj-;
#pragma link C++ class MomentObj+;

//Core classes
#pragma link C++ class Config+;
#pragma link C++ class Event+;
#pragma link C++ class Timer+;
#pragma link C++ class SelectorBase+;
#pragma link C++ class AnalysisBase+;
#pragma link C++ class AnalysisChain+;
#pragma link C++ class TDirCache+;
#pragma link C++ class EventBuilderBase+;
#pragma link C++ class ProofAna+;

//Auxiliary functions
#pragma link C++ function GetMoreOutputs;
#pragma link C++ function GetDatasets;

//Suppress some warnings/fix Finalize error running PROOFLite
#pragma link C++ class map<Key<0>,MapObj*>+;
#pragma link C++ class pair<Key<2>,AnalysisBase*>+;
#pragma link C++ class pair<Key<2>,TProofOutputFile*>+;
#pragma link C++ class pair<Key<2>,TFile*>+;
#pragma link C++ class pair<Key<2>,vector<Key<2> > >+;


#pragma link C++ class vector<vector<int> >+; 

#endif

#endif

