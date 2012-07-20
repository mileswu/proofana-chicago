/**************************************************************************
 **
 **   File:         MomentObj.cxx
 **
 **   Description:  Generic base class for simple type storage
 **
 **   Author:       B. Butler
 **   Created:      3-26-11
 **
 **************************************************************************/

#define MomentObj_cxx

#include "MomentObj.h"
#include "Particle.h"

//Sorting structs

struct pt_cmp {

  bool reverse;

  bool operator ()(TObject* a, TObject* b) const {

	/*
    Particle* x = dynamic_cast<Particle*>(a);
    Particle* y = dynamic_cast<Particle*>(b);

    if(!(x&&y)) Abort("MomentObj: ERROR tried to sort vector of non-ParticleSortBase objects by Pt");
    */
    
    //sacrifice error-checking for speed
    Particle* x = static_cast<Particle*>(a);
    Particle* y = static_cast<Particle*>(b);    

	//strict weak ordering
	if(TMath::Abs(x->p.Pt()-y->p.Pt())<1e-5) return false;

    bool result = false;
    if (x->p.Pt() > y->p.Pt()) result=true;
    else result=false;

    if (reverse) return !result;
    return result;
  }
};

struct e_cmp {

  bool reverse;

  bool operator ()(TObject* a, TObject* b) const {

	/*
    Particle* x = dynamic_cast<Particle*>(a);
    Particle* y = dynamic_cast<Particle*>(b);

    if(!(x&&y)) Abort("MomentObj: ERROR tried to sort vector of non-ParticleSortBase objects by E");
    */
    
    //sacrifice error-checking for speed
    Particle* x = static_cast<Particle*>(a);
    Particle* y = static_cast<Particle*>(b);    
    
	//strict weak ordering
	if(TMath::Abs(x->p.E()-y->p.E())<1e-5) return false;

    bool result = false;
    if (x->p.E() > y->p.E()) result=true;
    else result=false;

    if (reverse) return !result;
    return result;
  }
};

class value_cmp {

	public:
		value_cmp(const MomKey& sortkey) : m_sortkey(sortkey) { }
		~value_cmp() { }

		bool operator ()(TObject* a, TObject* b) const {

			/*
			MomentObj* x = dynamic_cast<MomentObj*>(a);
			MomentObj* y = dynamic_cast<MomentObj*>(b);
			
			if(!(x&&y)) Abort("MomentObj: ERROR tried to sort vector of non-MomentObj objects by key value");
			*/
			
    		//sacrifice error-checking for speed
    		MomentObj* x = static_cast<MomentObj*>(a);
    		MomentObj* y = static_cast<MomentObj*>(b);   			
			
			if((x->Type(m_sortkey)==OBJ)||(y->Type(m_sortkey)==OBJ)) {
				TString msg("MomentObj: ERROR key ");
				msg.Append(m_sortkey.Data());
				msg.Append(" refers to an object, not a simple type");
				Abort(msg);
			}
		
			//Pick type with most precision, relies on proper ordering in enum
			int type = x->Type(m_sortkey)>y->Type(m_sortkey) ? x->Type(m_sortkey) : y->Type(m_sortkey);

			float flx, fly;
			double dbx, dby;

			switch(type) {
				case BOOL:
	        		if (x->Bool(m_sortkey) > y->Bool(m_sortkey)) return true;
					return false;
				case INT:
	        		if (x->Int(m_sortkey) > y->Int(m_sortkey)) return true;
					return false;
				case LONG:
        			if (x->Long(m_sortkey) > y->Long(m_sortkey)) return true;
					return false;
				case FLOAT:
					flx = x->Float(m_sortkey);
					fly = y->Float(m_sortkey);
					if (TMath::Abs(flx-fly)<1e-5) return false; //strict weak ordering
	        		if (flx > fly) return true;
					return false;
				case DOUBLE:
					dbx = x->Double(m_sortkey);
					dby = y->Double(m_sortkey);
					if (TMath::Abs(dbx-dby)<1e-10) return false; //strict weak ordering
	        		if (dbx > dby) return true;
					return false;
				case STRING:
	       			if (x->String(m_sortkey) > y->String(m_sortkey)) return true;
					return false;
				default:
					Abort("MomentObj: ERROR unknown sort type?");
					return false;
			}			
		}
		
	private:
	
		MomKey m_sortkey;
};

class value_rcmp {

	public:
		value_rcmp(const MomKey& sortkey) : m_sortkey(sortkey) { }
		~value_rcmp() { }

		bool operator ()(TObject* a, TObject* b) const {

			/*
			MomentObj* x = dynamic_cast<MomentObj*>(a);
			MomentObj* y = dynamic_cast<MomentObj*>(b);
			
			if(!(x&&y)) Abort("MomentObj: ERROR tried to sort vector of non-MomentObj objects by key value");
			*/
			
    		//sacrifice error-checking for speed
    		MomentObj* x = static_cast<MomentObj*>(a);
    		MomentObj* y = static_cast<MomentObj*>(b);   			
			
			if((x->Type(m_sortkey)==OBJ)||(y->Type(m_sortkey)==OBJ)) {
				TString msg("MomentObj: ERROR key ");
				msg.Append(m_sortkey.Data());
				msg.Append(" refers to an object, not a simple type");
				Abort(msg);
			}
		
			//Pick type with most precision, relies on proper ordering in enum
			int type = x->Type(m_sortkey)>y->Type(m_sortkey) ? x->Type(m_sortkey) : y->Type(m_sortkey);

			float flx, fly;
			double dbx, dby;

			switch(type) {
				case BOOL:
	        		if (x->Bool(m_sortkey) < y->Bool(m_sortkey)) return true;
					return false;
				case INT:
	        		if (x->Int(m_sortkey) < y->Int(m_sortkey)) return true;
					return false;
				case LONG:
        			if (x->Long(m_sortkey) < y->Long(m_sortkey)) return true;
					return false;
				case FLOAT:
					flx = x->Float(m_sortkey);
					fly = y->Float(m_sortkey);
					if (TMath::Abs(flx-fly)<1e-5) return false; //strict weak ordering
	        		if (flx < fly) return true;
					return false;
				case DOUBLE:
					dbx = x->Double(m_sortkey);
					dby = y->Double(m_sortkey);
					if (TMath::Abs(dbx-dby)<1e-10) return false; //strict weak ordering
	        		if (dbx < dby) return true;
					return false;
				case STRING:
        			if (x->String(m_sortkey) < y->String(m_sortkey)) return true;
					return false;
				default:
					Abort("MomentObj: ERROR unknown sort type?");
					return false;
			}
		}
		
	private:
	
		MomKey m_sortkey;
};


//ObjVecObj constituent reference counting
map<TObject*,unsigned int> ObjVecObj::m_objrefs;

//cloning
map<TObject*,TObject*> ObjVecObj::m_clonemap;
bool MomentObj::m_clonerunning = false;

void ObjVecObj::Streamer(TBuffer &R__b)
{
   // Stream an object of class ObjVecObj.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ObjVecObj::Class(),this);
      
      //reference counting
      if(!m_weakref) {
      	vector<TObject*>::iterator iter = m_data.begin();
      	for(; iter!=m_data.end(); ++iter) ++m_objrefs[*iter];
      }
   } else {
      R__b.WriteClassBuffer(ObjVecObj::Class(),this);
   }
}

//MomentObj methods

bool MomentObj::Set(const MomKey& key, bool value) {
	map<MomKey,MapObj*>::iterator iter;
	if(Find(key,iter)) {
		delete iter->second;
		iter->second = new BoolObj(value);
	}
	else m_keyMap.insert(iter, map<MomKey,MapObj*>::value_type(key, new BoolObj(value)));
	return true;
}

bool MomentObj::Set(const MomKey& key, unsigned int value) {
	map<MomKey,MapObj*>::iterator iter;
	if(Find(key,iter)) {
		delete iter->second;
		iter->second = new UIntObj(value);
	}
	else m_keyMap.insert(iter, map<MomKey,MapObj*>::value_type(key, new UIntObj(value)));
	return true;
}

bool MomentObj::Set(const MomKey& key, int value) {
	map<MomKey,MapObj*>::iterator iter;
	if(Find(key,iter)) {
		delete iter->second;
		iter->second = new IntObj(value);
	}
	else m_keyMap.insert(iter, map<MomKey,MapObj*>::value_type(key, new IntObj(value)));
	return true;
}

bool MomentObj::Set(const MomKey& key, Long64_t value) {
	map<MomKey,MapObj*>::iterator iter;
	if(Find(key,iter)) {
		delete iter->second;
		iter->second = new LongObj(value);
	}
	else m_keyMap.insert(iter, map<MomKey,MapObj*>::value_type(key, new LongObj(value)));
	return true;
}

bool MomentObj::Set(const MomKey& key, float value) {
	map<MomKey,MapObj*>::iterator iter;
	if(Find(key,iter)) {
		delete iter->second;
		iter->second = new FloatObj(value);
	}
	else m_keyMap.insert(iter, map<MomKey,MapObj*>::value_type(key, new FloatObj(value)));
	return true;
}

bool MomentObj::Set(const MomKey& key, double value) {
	map<MomKey,MapObj*>::iterator iter;
	if(Find(key,iter)) {
		delete iter->second;
		iter->second = new DoubleObj(value);
	}
	else m_keyMap.insert(iter, map<MomKey,MapObj*>::value_type(key, new DoubleObj(value)));
	return true;
}

bool MomentObj::Set(const MomKey& key, const TString& value) {
	map<MomKey,MapObj*>::iterator iter;
	if(Find(key,iter)) {
		delete iter->second;
		iter->second = new StringObj(value);
	}
	else m_keyMap.insert(iter, map<MomKey,MapObj*>::value_type(key, new StringObj(value)));
	return true;
}

bool MomentObj::AddVec(const MomKey& key, bool weakref) {
	map<MomKey,MapObj*>::iterator iter;
	if(Find(key,iter)) {
		delete iter->second;
		iter->second = new ObjVecObj(weakref);
	}
	else m_keyMap.insert(iter, map<MomKey,MapObj*>::value_type(key, new ObjVecObj(weakref)));
	return true;
}

void MomentObj::AliasVec(const MomKey& src, const MomKey& dest)
{
	map<MomKey,MapObj*>::const_iterator itersrc;
	map<MomKey,MapObj*>::iterator iterdest;
	if(!Find(src,itersrc)) Find(src); //generate error message
	if(itersrc->second->Type()!=OBJ) Abort("MomentObj: ERROR cannot alias non-object vector key");
	
	if(Find(dest,iterdest)) (static_cast<ObjVecObj*>(iterdest->second))->Merge(static_cast<const ObjVecObj*>(itersrc->second));
	else {
		ObjVecObj* objvec = new ObjVecObj(*static_cast<ObjVecObj*>(itersrc->second));
		m_keyMap.insert(iterdest, map<MomKey,MapObj*>::value_type(dest,objvec));
	}
}

bool MomentObj::Get(const MomKey& key, bool& value) const {
	map<MomKey,MapObj*>::const_iterator iter;
	if(Find(key,iter)) {
		value = iter->second->Bool();
		return true;
	}
	return false;
}

bool MomentObj::Get(const MomKey& key, unsigned int& value) const {
	map<MomKey,MapObj*>::const_iterator iter;
	if(Find(key,iter)) {
		value = iter->second->UInt();
		return true;
	}
	return false;
}

bool MomentObj::Get(const MomKey& key, int& value) const {
	map<MomKey,MapObj*>::const_iterator iter;
	if(Find(key,iter)) {
		value = iter->second->Int();
		return true;
	}
	return false;
}

bool MomentObj::Get(const MomKey& key, Long64_t& value) const {
	map<MomKey,MapObj*>::const_iterator iter;
	if(Find(key,iter)) {
		value = iter->second->Long();
		return true;
	}
	return false;
}

bool MomentObj::Get(const MomKey& key, float& value) const {
	map<MomKey,MapObj*>::const_iterator iter;
	if(Find(key,iter)) {
		value = iter->second->Float();
		return true;
	}
	return false;
}

bool MomentObj::Get(const MomKey& key, double& value) const {
	map<MomKey,MapObj*>::const_iterator iter;
	if(Find(key,iter)) {
		value = iter->second->Double();
		return true;
	}
	return false;
}

bool MomentObj::Get(const MomKey& key, TString& value) const {
	map<MomKey,MapObj*>::const_iterator iter;
	if(Find(key,iter)) {
		value = iter->second->String();
		return true;
	}
	return false;
}

bool MomentObj::Get(const MomKey& key, int index, TObject*& obj) const {
	map<MomKey,MapObj*>::const_iterator iter;
	if(Find(key,iter)) {
		if(iter->second->Type()!=OBJ) return false;
		obj = iter->second->Obj(index);
		return true;
	}
	return false;
}

void MomentObj::Sort(const MomKey& key, const MomKey& sortkey, bool reverse) {
	vector<TObject*>* coll = Find(key)->second->ObjVec();

	if(coll->size()) {	
		if(dynamic_cast<MomentObj*>(*coll->begin())) {
			if(reverse) {
				value_rcmp cmp(sortkey);
				sort(coll->begin(),coll->end(),cmp);			
			}
			else {
				value_cmp cmp(sortkey);
				sort(coll->begin(),coll->end(),cmp);
			}
		}
		else {
			Abort(TString::Format("MomentObj: ERROR tried to sort vector of non-MomentObj objects (%s) by %s",key.Data(),sortkey.Data()));	
		}
	}
}

void MomentObj::SortPt(const MomKey& key, bool reverse) {
	vector<TObject*>* coll = Find(key)->second->ObjVec();

	if(coll->size()) {		
		if(dynamic_cast<Particle*>(*coll->begin())) {	
			static pt_cmp cmp;
			cmp.reverse = reverse;		
			sort(coll->begin(),coll->end(),cmp);
		}
		else {
			Abort(TString::Format("MomentObj: ERROR tried to sort vector of non-Particle objects (%s) by Pt",key.Data()));	
		}
	}
}

void MomentObj::SortE(const MomKey& key, bool reverse) {
	vector<TObject*>* coll = Find(key)->second->ObjVec();

	if(coll->size()) {	
		if(dynamic_cast<Particle*>(*coll->begin())) {	
			static e_cmp cmp;
			cmp.reverse = reverse;
			sort(coll->begin(),coll->end(),cmp);
		}
		else {
			Abort(TString::Format("MomentObj: ERROR tried to sort vector of non-Particle objects (%s) by E",key.Data()));
		}
	} 
}


void MomentObj::SortPtAll(bool reverse) {

	static pt_cmp cmp;
	cmp.reverse = reverse;
	
	//Sort all object vectors whose members inherit from TLorentzVector by Pt
	map<MomKey, MapObj*>::iterator coll = m_keyMap.begin();
	for(;coll!=m_keyMap.end();coll++) {
		if(coll->second->Type()!=OBJ) continue;
		if(coll->second->ObjVec()->size()) {
			if(dynamic_cast<Particle*>(*coll->second->ObjVec()->begin())) {
				sort(coll->second->ObjVec()->begin(),coll->second->ObjVec()->end(),cmp);
			}		
		}
	}
}

void MomentObj::Show() const {

	
	//Reorder map with a scheme more intelligible to humans (but less efficient for searches)
	map<TString, MapObj*> reordered_map;
	map<MomKey, MapObj*>::const_iterator q = m_keyMap.begin();
	for(; q!=m_keyMap.end(); ++q) reordered_map[q->first.Data()] = q->second;

	cout << "*******************************************************************" << endl;
	cout << "* Key                | Type         | Value/Size                  *" << endl;	
	cout << "*-----------------------------------------------------------------*" << endl;
	map<TString, MapObj*>::const_iterator p;
	for(p = reordered_map.begin(); p!=reordered_map.end(); ++p) {
		TString first(p->first);
		TString second;
		TString third;
		if(p->second->Type()!=OBJ) third = p->second->String();
		switch(p->second->Type()) {
			case BOOL:
				second = "bool";
				break;
			case UINT:
				second = "unsigned int";
				break;
			case INT:
				second = "int";
				break;
			case LONG:
				second = "long";
				break;
			case FLOAT:
				second = "float";
				break;
			case DOUBLE:
				second = "double";
				break;
			case STRING:
				second = "string";
				break;
			case OBJ:
				second = "objects";
				third = TString::Format("%i",p->second->Objs());
				break;
			default:
				TString msg("MomentObj: unrecognized type ");
				msg.Append(TString::Format("%i",p->second->Type()));
				msg.Append(" for key ");
				msg.Append(p->first);
				Abort(msg);
				break;
		}
		while(first.Length()<18) { first.Append(" ");}
		while(second.Length()<12) { second.Append(" ");}	
		while(third.Length()<27) { third.Append(" ");}	
		cout << "* " << first << " | " << second << " | " << third << " *"<< endl;
	}
	cout << "*******************************************************************" << endl;
}

