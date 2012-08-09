/**************************************************************************
 **
 **   File:         MomentObj.h
 **
 **   Description:  Generic base class for simple type storage
 **
 **   Author:       B. Butler
 **   Created:      3-26-11
 **
 **************************************************************************/

#ifndef MomentObj_h
#define MomentObj_h

#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "TString.h"
#include "Key.h"
#include "TNamed.h"
#include "Error.h"

using std::map;
using std::vector;
using std::set;
using std::cout;
using std::endl;
using std::exit;

enum {OBJ, BOOL, UINT, INT, LONG, FLOAT, DOUBLE, STRING};

class MapObj {

	public:
		MapObj() { }
		virtual ~MapObj() { }
		
		virtual MapObj* Clone(const char* newname = "") const = 0;
	
		virtual unsigned int Type() const { Abort("MapObj: ERROR function Type() not implemented for this type"); return -1; }
		virtual bool WeakRef() const { Abort("MapObj: ERROR function WeakRef() not implemented for this type"); return false; }
		virtual void WeakRef(bool /*weakref*/) { Abort("MapObj: ERROR function WeakRef(bool) not implemented for this type"); }

		//Simple types
		virtual bool Bool() const { Abort("MapObj: ERROR function Bool() not implemented for this type"); return false; }		
		virtual unsigned int UInt() const { Abort("MapObj: ERROR function UInt() not implemented for this type"); return 0; }
		virtual int Int() const { Abort("MapObj: ERROR function Int() not implemented for this type"); return 0; }
		virtual Long64_t Long() const { Abort("MapObj: ERROR function Long() not implemented for this type"); return 0; }
		virtual float Float() const { Abort("MapObj: ERROR function Float() not implemented for this type"); return 0.; }
		virtual double Double() const { Abort("MapObj: ERROR function Double() not implemented for this type"); return 0.; }
		virtual TString String() const { Abort("MapObj: ERROR function String() not implemented for this type"); return ""; }
	
		//Object vectors
		virtual bool Add(TObject* /*obj*/) { Abort("MapObj: ERROR function Add() not implemented for this type"); return false; }
		virtual bool Remove(const unsigned int& /*index*/) { Abort("MapObj: ERROR function Remove() not implemented for this type"); return false; }

		virtual int Index(TObject const* const /*obj*/) const { Abort("MapObj: ERROR function Index() not implemented for this type"); return 0; }
		virtual unsigned int Objs() const { Abort("MapObj: ERROR function Objs() not implemented for this type"); return 0; }	
		virtual TObject* Obj(const unsigned int& /*index*/) const { Abort("MapObj: ERROR function Obj() not implemented for this type"); return 0; }
		virtual vector<TObject*>* ObjVec() { Abort("MapObj: ERROR function ObjVec() not implemented for this type"); return 0; }

		ClassDef(MapObj,1);

	protected:
		template <typename T>
		T from_string(const TString& s) const {
			TString u(s);
			T t;
			if (!(s.CompareTo("false",TString::kIgnoreCase)&&s.CompareTo("kfalse",TString::kIgnoreCase))) u = "0"; //Handle alphabetic booleans
			else if (!(s.CompareTo("true",TString::kIgnoreCase)&&s.CompareTo("ktrue",TString::kIgnoreCase))) u = "1";
			std::istringstream iss(u.Data());
			if((iss >> t).fail()) {
				TString msg("MapObj: ERROR conversion of ");
				msg.Append(s);
				msg.Append(" to numeric type failed");
				Abort(msg);
			}
			return t;
		}

		template <typename T>
		TString from_T(const T& t) const {
			std::ostringstream oss;
			if((oss << std::boolalpha << t).fail()) {
				TString msg("MapObj: ERROR conversion to string failed");
				Abort(msg);
			}
			return oss.str();
		}
};

class BoolObj : public MapObj {

	public:
		BoolObj() { }
		BoolObj(bool data) : m_data(data) { }
		virtual ~BoolObj() { }
		
		virtual MapObj* Clone(const char* /*newname = ""*/) const {
			BoolObj* obj = new BoolObj(*this);
			return obj;
		}
		
		unsigned int Type() const { return BOOL; }

		bool Bool() const { return m_data; }
		unsigned int UInt() const { return (unsigned int)m_data; }
		int Int() const { return (int)m_data; }
		Long64_t Long() const { return (Long64_t)m_data; }
		float Float() const { return (float)m_data; }
		double Double() const { return (double)m_data; }
		TString String() const { return from_T(m_data); }
		
		ClassDef(BoolObj,1);

	private:
		Bool_t m_data;
};

class UIntObj : public MapObj {

	public:
		UIntObj() { }
		UIntObj(unsigned int data) : m_data(data) { }
		virtual ~UIntObj() { }
		
		virtual MapObj* Clone(const char* /*newname = ""*/) const {
			UIntObj* obj = new UIntObj(*this);
			return obj;
		}
		
		unsigned int Type() const { return UINT; }

		bool Bool() const { return (bool)m_data; }
		unsigned int UInt() const { return m_data; }
		int Int() const { return (int)m_data; }
		Long64_t Long() const { return (Long64_t)m_data; }
		float Float() const { return (float)m_data; }
		double Double() const { return (double)m_data; }
		TString String() const { return from_T(m_data); }
		
		ClassDef(UIntObj,1);

	private:
		UInt_t m_data;
};

class IntObj : public MapObj {

	public:
		IntObj() { }
		IntObj(int data) : m_data(data) { }
		virtual ~IntObj() { }
		
		virtual MapObj* Clone(const char* /*newname = ""*/) const {
			IntObj* obj = new IntObj(*this);
			return obj;
		}
		
		unsigned int Type() const { return INT; }

		bool Bool() const { return (bool)m_data; }
		unsigned int UInt() const { return (unsigned int)m_data; }
		int Int() const { return m_data; }
		Long64_t Long() const { return (Long64_t)m_data; }
		float Float() const { return (float)m_data; }
		double Double() const { return (double)m_data; }
		TString String() const { return from_T(m_data); }
		
		ClassDef(IntObj,1);

	private:
		Int_t m_data;
};

class LongObj : public MapObj {

	public:
		LongObj() { }
		LongObj(Long64_t data) : m_data(data) { }
		virtual ~LongObj() { }
		
		virtual MapObj* Clone(const char* /*newname = ""*/) const {
			LongObj* obj = new LongObj(*this);
			return obj;
		}
		
		unsigned int Type() const { return LONG; }

		bool Bool() const { return (bool)m_data; }
		unsigned int UInt() const { return (unsigned int)m_data; }
		int Int() const { return (int)m_data; }
		Long64_t Long() const { return m_data; }
		float Float() const { return (float)m_data; }
		double Double() const { return (double)m_data; }
		TString String() const { return from_T(m_data); }
		
		ClassDef(LongObj,1);

	private:
		Long64_t m_data;
};

class FloatObj : public MapObj {

	public:
		FloatObj() { }
		FloatObj(float data) : m_data(data) { }
		virtual ~FloatObj() { }
		
		virtual MapObj* Clone(const char* /*newname = ""*/) const {
			FloatObj* obj = new FloatObj(*this);
			return obj;
		}
		
		unsigned int Type() const { return FLOAT; }

		bool Bool() const { return (bool)m_data; }
		unsigned int UInt() const { return (unsigned int)m_data; }
		int Int() const { return (int)m_data; }
		Long64_t Long() const { return (Long64_t)m_data; }
		float Float() const { return m_data; }
		double Double() const { return (double)m_data; }
		TString String() const { return from_T(m_data); }
		
		ClassDef(FloatObj,1);

	private:
		Float_t m_data;
};

class DoubleObj : public MapObj {

	public:
		DoubleObj() { }
		DoubleObj(double data) : m_data(data) { }
		virtual ~DoubleObj() { }
		
		virtual MapObj* Clone(const char* /*newname = ""*/) const {
			DoubleObj* obj = new DoubleObj(*this);
			return obj;
		}
		
		unsigned int Type() const { return DOUBLE; }

		bool Bool() const { return (bool)m_data; }
		unsigned int UInt() const { return (unsigned int)m_data; }
		int Int() const { return (int)m_data; }
		Long64_t Long() const { return (Long64_t)m_data; }
		float Float() const { return (float)m_data; }
		double Double() const { return m_data; }
		TString String() const { return from_T(m_data); }
		
		ClassDef(DoubleObj,1);

	private:
		Double_t m_data;
};

class StringObj : public MapObj {

	public:
		StringObj() { }
		StringObj(TString data) : m_data(data) { }
		virtual ~StringObj() { }
		
		virtual MapObj* Clone(const char* /*newname = ""*/) const {
			StringObj* obj = new StringObj(*this);
			return obj;
		}
		
		unsigned int Type() const { return STRING; }

		bool Bool() const { return from_string<bool>(m_data); }
		unsigned int UInt() const { return from_string<unsigned int>(m_data); }
		int Int() const { return from_string<int>(m_data); }
		Long64_t Long() const { return from_string<Long64_t>(m_data); }
		float Float() const { return from_string<float>(m_data); }
		double Double() const { return from_string<double>(m_data); }
		TString String() const { return m_data; }
		
		ClassDef(StringObj,1);

	private:
		TString m_data;
};

class ObjVecObj : public MapObj {

	public:
		ObjVecObj(bool weakref = false) : m_weakref(weakref) { }
		ObjVecObj(const ObjVecObj& objvecobj) : MapObj(objvecobj), m_weakref(objvecobj.m_weakref), m_data(objvecobj.m_data) { //not a deep copy of objects
			if(!m_weakref) {
				vector<TObject*>::iterator iter = m_data.begin();
				for(; iter!=m_data.end(); ++iter) ++m_objrefs[*iter];
			}
		}
		virtual ~ObjVecObj() {
			if(!m_weakref) {
				//Delete objects depending on reference count
				vector<TObject*>::iterator iter = m_data.begin();
				map<TObject*,unsigned int>::iterator mapiter;
				for(; iter!=m_data.end(); ++iter) {
					mapiter = m_objrefs.find(*iter);
        			if(!(--(mapiter->second))) {
						m_objrefs.erase(mapiter);
						delete *iter;
					}
				}
			}
		}
		
		virtual MapObj* Clone(const char* /*newname = ""*/) const { //deep copy of objects, while preserving links

			ObjVecObj* obj = new ObjVecObj(m_weakref);
			
			vector<TObject*>::const_iterator iter = m_data.begin();
			map<TObject*,TObject*>::iterator mapiter; 
			TObject* newobj;
			for(; iter!=m_data.end(); ++iter) {
				mapiter = m_clonemap.lower_bound(*iter);
				if(mapiter != m_clonemap.end() && ((*iter)==mapiter->first)) newobj = mapiter->second;
				else {
					newobj = (*iter)->Clone();
					m_clonemap.insert(mapiter,map<TObject*,TObject*>::value_type(*iter,newobj));
				}
				
				if(!m_weakref) ++m_objrefs[newobj];
				obj->m_data.push_back(newobj);
			}
			return obj;
		}
		
		unsigned int Type() const { return OBJ; }
		bool WeakRef() const { return m_weakref; }
		void WeakRef(bool weakref) {
			if(m_weakref==weakref) return;
			
			if(weakref) { //remove a reference from the count
				vector<TObject*>::iterator iter = m_data.begin();
				map<TObject*,unsigned int>::iterator mapiter;
				for(; iter!=m_data.end(); ++iter) {
					mapiter = m_objrefs.find(*iter);
        			if(!(--(mapiter->second))) m_objrefs.erase(mapiter);
				}			
			}
			else { //add a reference to the count
				vector<TObject*>::iterator iter = m_data.begin();
				for(; iter!=m_data.end(); ++iter) ++m_objrefs[*iter];		
			}
			
			m_weakref = weakref;
		}

		bool Add(TObject* obj) { if(!m_weakref) {++m_objrefs[obj];} m_data.push_back(obj); return true; }
		bool Remove(const unsigned int& index) {
			if(index>=m_data.size()) Abort("ObjVecObj Remove(): ERROR index out-of-range");
			
			//reference counting
			if(!m_weakref) {
				map<TObject*,unsigned int>::iterator iter = m_objrefs.find(m_data.at(index));
				if(!(--(iter->second))) {
					delete iter->first;
					m_objrefs.erase(iter);
				}
			}
			m_data.erase(m_data.begin()+index);
			return true;
		}

		int Index(TObject const* const obj) const {
			for(unsigned int i = 0; i < m_data.size(); ++i) {
				if(obj==m_data.at(i)) return i;
			}
			return -1;
		}
		unsigned int Objs() const { return m_data.size(); }	
		TObject* Obj(const unsigned int& index) const { if(index>=m_data.size()) { Abort("ObjVecObj Obj(): ERROR index out-of-range"); } return m_data.at(index); }
		vector<TObject*>* ObjVec() { return &m_data; } //for sorting only, DO NOT ADD OR REMOVE ANYTHING, IT WILL CAUSE MEMORY LEAKS
		
		void Merge(const ObjVecObj* objvec) {
			if(objvec->m_weakref!=m_weakref) Abort("ObjVecObj Merge(): ERROR Attempted merging of strong and weak-referenced ObjVecObj");
			
			vector<TObject*>::const_iterator iter = objvec->m_data.begin();
			for(; iter!=objvec->m_data.end(); ++iter) {
				if(!m_weakref) ++m_objrefs[*iter];
				m_data.push_back(*iter);
			}
		}
		
		static void ClearCloneCache() { m_clonemap.clear(); }

		ClassDef(ObjVecObj,1);

	private:
		bool m_weakref;
		vector<TObject*> m_data;
		
		//reference counting
		static map<TObject*,unsigned int> m_objrefs; //! don't persistify
		
		//preserve links while cloning
		static map<TObject*,TObject*> m_clonemap;
};


class MomentObj : public TNamed {
	public:
		MomentObj() { }
		virtual ~MomentObj() {Reset();}

		MomentObj(const MomentObj& obj) : TNamed(obj), m_keyMap(obj.m_keyMap) { //deep copies
			bool topclone = !m_clonerunning;
			if(topclone) m_clonerunning = true;

			map<MomKey, MapObj*>::iterator iter = m_keyMap.begin();
			for(; iter!=m_keyMap.end(); iter++) iter->second = iter->second->Clone();
			
			if(topclone) {
				m_clonerunning = false;
				ObjVecObj::ClearCloneCache();
			}
		}
		
		virtual TObject* Clone(const char* newname = "") const { //deep copies
			MomentObj* obj = new MomentObj(*this);
			obj->SetName(newname);

			return static_cast<TObject*>(obj);
		}
		
		MomentObj& operator=(const MomentObj& obj) { //deep copies
			Reset();
			m_keyMap = obj.m_keyMap;
			
			bool topclone = !m_clonerunning;
			if(topclone) m_clonerunning = true;
			
			map<MomKey, MapObj*>::iterator iter = m_keyMap.begin();
			for(; iter!=m_keyMap.end(); iter++) iter->second = iter->second->Clone();
			
			if(topclone) {
				m_clonerunning = false;
				ObjVecObj::ClearCloneCache();
			}
			
			return *this;
		}
		
		//copy from obj to this, but do not clone sub-objects in ObjVecObj, just copy the pointers
		void ShallowClone(const MomentObj& obj) {
			Reset();
			m_keyMap = obj.m_keyMap;
			
			map<MomKey, MapObj*>::iterator iter = m_keyMap.begin();
			for(; iter!=m_keyMap.end(); iter++) {
				if(iter->second->Type()==OBJ) {
					iter->second = new ObjVecObj(*(ObjVecObj*)iter->second);
				}
				else iter->second = iter->second->Clone(); //simple types (moments) copied
			}			
		}

		//move all from obj to this
		void Move(MomentObj& obj) {
			Reset();
			m_keyMap = obj.m_keyMap;
			obj.m_keyMap.clear();
		}
		
		void Reset() {
			map<MomKey, MapObj*>::iterator iter = m_keyMap.begin();
			for(; iter!=m_keyMap.end(); iter++) delete iter->second;
			m_keyMap.clear();
		}
		virtual void Show() const;
		bool Exists(const MomKey& key) const {return m_keyMap.find(key)!=m_keyMap.end(); }
		int Type(const MomKey& key) const { return Find(key)->second->Type(); }
		bool WeakRef(const MomKey& key) const { return Find(key)->second->WeakRef(); }
		void WeakRef(const MomKey& key, bool weakref) { Find(key)->second->WeakRef(weakref); }
		
		const map<MomKey,MapObj*>& Keys() const {return m_keyMap;}

		bool Set(const MomKey& key, const bool value);
		bool Set(const MomKey& key, const int value);
		bool Set(const MomKey& key, const unsigned int value);
		bool Set(const MomKey& key, const Long64_t value);
		bool Set(const MomKey& key, const float value);
		bool Set(const MomKey& key, const double value);
		bool Set(const MomKey& key, const TString& value);
		bool Set(const MomKey& key, const char* value) {return Set(key,TString(value));}
		bool Set(const MomKey& key, const std::string& value) {return Set(key,TString(value.c_str()));}
		bool Set(const MomKey& key, TObject *value);

		bool Get(const MomKey& key, bool& value) const;
		bool Get(const MomKey& key, unsigned int& value) const;
		bool Get(const MomKey& key, int& value) const;
		bool Get(const MomKey& key, Long64_t& value) const;
		bool Get(const MomKey& key, float& value) const;
		bool Get(const MomKey& key, double& value) const;
		bool Get(const MomKey& key, TString& value) const;
		bool Get(const MomKey& key, int index, TObject*& obj) const;

		bool Bool(const MomKey& key) const { return Find(key)->second->Bool(); }
		int UInt(const MomKey& key) const { return Find(key)->second->UInt(); }
		int Int(const MomKey& key) const { return Find(key)->second->Int(); }
		Long64_t Long(const MomKey& key) const { return Find(key)->second->Long(); }
		float Float(const MomKey& key) const { return Find(key)->second->Float(); }
		double Double(const MomKey& key) const { return Find(key)->second->Double(); }
		TString String(const MomKey& key) const { return Find(key)->second->String(); }

		bool AddVec(const MomKey& key, bool weakref = false);
		bool Add(const MomKey& key, TObject* obj) { return Find(key)->second->Add(obj); }

		//Removing objects from the event (overlap removal, etc.), make sure you loop from the highest index!		
		bool Remove(const MomKey& key, int index) { return Find(key)->second->Remove(index); }
		bool Remove(const MomKey& key) { 
			map<MomKey,MapObj*>::iterator iter = Find(key);
			delete iter->second;
			m_keyMap.erase(iter);
			return true;
		}
		
		//Sort function by key value
		void Sort(const MomKey& key, const MomKey& sortkey, bool reverse = false);
		void SortPt(const MomKey& key, bool reverse = false); //Only for TLorentzVector containers
		void SortE(const MomKey& key, bool reverse = false); //Only for TLorentzVector containers
		void SortPtAll(bool reverse = false); //Only sorts containers with objects inheriting from TLorentzVector
		
		TObject* Obj(const MomKey& key, int index = 0) const { return Find(key)->second->Obj(index); }
		int Index(const MomKey& key, TObject const* const obj) const { return Find(key)->second->Index(obj); }
		int Objs(const MomKey& key) const { return Find(key)->second->Objs(); }
		vector<TObject*>* ObjVec(const MomKey& key) { return Find(key)->second->ObjVec(); }
	
		//Aliasing vectors of objects (i.e., to create a subset of jets or electrons)
		void AliasVec(const MomKey& src, const MomKey& dest);
		
		//Convenient casting methods
		template <class T>
		T& Obj(const MomKey& key, int index = 0) { return *dynamic_cast<T*>(Obj(key, index)); }
		template <class T>
		vector<T*> ObjVec(const MomKey& key) {
			vector<T*>retval;
			for(int i=0; i<Objs(key); i++) {
				retval.push_back(&Obj<T>(key, i));
			}
			return retval;
		}

		ClassDef(MomentObj,1);
	
	private:

		map<MomKey,MapObj*>::iterator Find(const MomKey& key) {
			map<MomKey,MapObj*>::iterator iter;
			if(!Find(key,iter)) {
				TString msg("MomentObj: ERROR missing key ");
				msg.Append(key.Data());
				msg.Append(" requested");
				Abort(msg);
			}
			return iter;
		}	
		map<MomKey,MapObj*>::const_iterator Find(const MomKey& key) const {
			map<MomKey,MapObj*>::const_iterator iter;
			if(!Find(key,iter)) {
				TString msg("MomentObj: ERROR missing key ");
				msg.Append(key.Data());
				msg.Append(" requested");
				Abort(msg);
			}
			return iter;
		}
		
		bool Find(const MomKey& key, map<MomKey,MapObj*>::iterator& iter) {
			iter = m_keyMap.lower_bound(key);
			if(iter != m_keyMap.end() && (key()==iter->first())) return true;
			else return false;
		}
		bool Find(const MomKey& key, map<MomKey,MapObj*>::const_iterator& iter) const {
			iter = m_keyMap.lower_bound(key);
			if(iter != m_keyMap.end() && (key()==iter->first())) return true;
			else return false;
		}

		map<MomKey, MapObj*> m_keyMap;
		static bool m_clonerunning;
};

#endif  /* MomentObj_h */
