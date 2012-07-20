/**************************************************************************
 **
 **   File:         Key.h
 **
 **   Description:  String-based key class to avoid string copies and comparisons
 **                 
 **
 **   Author:       B. Butler
 **
 **   Created:      12-29-11
 **   Modified:
 **
 **************************************************************************/

#ifndef Key_h
#define Key_h

#include <string>
#include "TString.h"
#include "TObject.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;


class InternalKey {

	public:
	
		InternalKey(const char* str) : m_str(str), m_len(strlen(str)), m_init(false) { } //Transient 
		InternalKey(const char* str, unsigned int len) : m_str(str), m_len(len), m_init(false) { } //Transient 
		InternalKey(const string& str) : m_str(str.c_str()), m_len(str.length()), m_init(false) { } //Transient
		InternalKey(const TString& str) : m_str(str.Data()), m_len(str.Length()), m_init(false) { } //Transient
		InternalKey(const char* str1, unsigned int len1, const char* str2, unsigned int len2) { Init(str1,len1,str2,len2); } //Persistent
		InternalKey(const InternalKey& key); //copy constructor, makes deep copy of str
		~InternalKey() { if(m_init) delete [] m_str; }
		
		void swap(InternalKey& key);
		static int IntCompare(const InternalKey& x, const InternalKey& y) {
			unsigned int lenx(x.Length());
			unsigned int leny(y.Length());		
			if(lenx!=leny) return (leny-lenx);
		
			return memcmp(x.Data(), y.Data(), lenx);	
		}

		unsigned int Length() const { return m_len; }
		const char* Data() const { return m_str; }
		bool Init() const { return m_init; }
		
	private:

		void Init(const char* str, unsigned int nchar) {

			char* data = new char[nchar+1];
   			data[nchar] = 0;  // terminating null
   			
   			memcpy(data, str, nchar);
      		m_str = data;
   			m_len = nchar;
   			m_init = true;

		}
		
		void Init(const char* str1, unsigned int nchar1, const char* str2, unsigned int nchar2) {

			char* data = new char[nchar1+nchar2+1];
   			data[nchar1+nchar2] = 0;  // terminating null
   			
   			memcpy(data, str1, nchar1);
   			memcpy(&data[nchar1], str2, nchar2);
      		m_str = data;
   			m_len = nchar1+nchar2;
   			m_init = true;

		}		

		const char* m_str;
		unsigned int m_len;		
		bool m_init;
		
};

struct InternalKey_cmp { //faster comparison function

	bool operator()(const InternalKey* x,const InternalKey* y) const {
		unsigned int lenx(x->Length());
		unsigned int leny(y->Length());		
		if(lenx!=leny) return lenx<leny;
		
		return (memcmp(x->Data(), y->Data(), lenx)>0);
	}

};

template<unsigned int N>
class Key {

	public:
	
		Key() : m_key(0) { }
		Key(const char* str) { InternalKey key(str); m_key = Init(key); }
		Key(const char* str, unsigned int len) { InternalKey key(str,len); m_key = Init(key); }
		Key(const string& str) { InternalKey key(str); m_key = Init(key); }
		Key(const TString& str) { InternalKey key(str); m_key = Init(key); }
		Key(InternalKey& key) : m_key(Init(key)) { }
		~Key() { }
		
		void swap(Key& key);

		const InternalKey* operator()() const { return m_key; }
		bool operator<(const Key key) const { return (m_key<key.m_key); }
		bool operator>(const Key key) const { return (m_key>key.m_key); }
		bool operator==(const Key key) const { return (m_key==key.m_key); }
		bool operator!=(const Key key) const { return (m_key!=key.m_key); }
		
		Key operator+(const Key key) const { return Key(Init(m_key,key.m_key)); }
		Key& operator+=(const Key key) { m_key = Init(m_key,key.m_key); return *this; }
		Key& Append(const Key key) { m_key = Init(m_key,key.m_key); return *this; }

		Key operator+(const TString& str) const { InternalKey intkey(str); return Key(Init(m_key,Init(intkey))); }
		Key& operator+=(const TString& str) { InternalKey intkey(str); m_key = Init(m_key,Init(intkey)); return *this; }
		Key& Append(const TString& str) { InternalKey intkey(str); m_key = Init(m_key,Init(intkey)); return *this; }

		Key operator+(const char* str) const { InternalKey intkey(str); return Key(Init(m_key,Init(intkey))); }
		Key& operator+=(const char* str) { InternalKey intkey(str); m_key = Init(m_key,Init(intkey)); return *this; }
		Key& Append(const char* str) { InternalKey intkey(str); m_key = Init(m_key,Init(intkey)); return *this; }

		//Type conversion
		operator const char*() const { return m_key->Data(); }

		unsigned int Length() const { return m_key->Length(); }
		const char* Data() const { return m_key->Data(); }	
		
		ClassDefNV(Key,1)
		
	private:
	
		Key(InternalKey* key) : m_key(key) { }
	
		InternalKey* m_key; //!	

		static InternalKey* Init(InternalKey& key) {
		
			static InternalKey_cmp cmp;

			vector<InternalKey*>::iterator iter = lower_bound(m_vec.begin(),m_vec.end(),&key,cmp);
			if(iter != m_vec.end() && !(InternalKey::IntCompare(key, *(*iter)))) {
				return *iter;
			}
			else {
				InternalKey* newkey = new InternalKey(key); //copy, makes persistent if not
				m_vec.insert(iter,newkey);
				return newkey;
			}
		}

		static InternalKey* Init(InternalKey* key1, InternalKey* key2) {

			map<InternalKey*,map<InternalKey*, InternalKey*> >::iterator iter1 = m_append.lower_bound(key1);
			if(iter1 != m_append.end() && !m_append.key_comp()(key1,iter1->first)) {
				map<InternalKey*, InternalKey*>::iterator iter2 = iter1->second.lower_bound(key2);
				if(iter2 != iter1->second.end() && !iter1->second.key_comp()(key2,iter2->first)) {
					return iter2->second;
				}
				else {
					InternalKey tempkey(key1->Data(),key1->Length(),key2->Data(),key2->Length());
					InternalKey* newkey = Init(tempkey);
					iter1->second.insert(iter2,map<InternalKey*,InternalKey*>::value_type(key2,newkey));
					return newkey;
				}
			}
			else {
				InternalKey tempkey(key1->Data(),key1->Length(),key2->Data(),key2->Length());
				InternalKey* newkey = Init(tempkey);
				(m_append.insert(iter1,map<InternalKey*,map<InternalKey*, InternalKey*> >::value_type(key1,map<InternalKey*,InternalKey*>())))->second.insert(map<InternalKey*,InternalKey*>::value_type(key2,newkey));
				return newkey;
			}
		}

		static vector<InternalKey*> m_vec;
		static map<InternalKey*,map<InternalKey*, InternalKey*> > m_append;

};

template <unsigned int N>
ostream& operator<< (ostream &str, const Key<N>& key) {
	str << key.Data();
	return str;		
}

template<unsigned int N>
class KeyObj : public TObject {

	public:
		KeyObj() { }
		KeyObj(const Key<N>& key) : m_key(key) { } 
		virtual ~KeyObj() { }
		
		void SetKey(const Key<N> key) { m_key = key; }
		Key<N> GetKey() const { return m_key; }
	
		ClassDef(KeyObj,1)
		
	private:
	
		Key<N> m_key;

};

typedef Key<0> MomKey;
typedef Key<1> BranchKey;
typedef Key<2> AnaKey;

typedef KeyObj<0> MomKeyObj;
typedef KeyObj<1> BranchKeyObj;
typedef KeyObj<2> AnaKeyObj;

#endif
