#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */
class CPropList;

class Prop{
public:
        Prop(int invID,const string & name);
        void Print(void) const;
        int getID() const;
        string getName() const;
        void AddDept(CPropList*&);
        CPropList * getActualDept()const;
        
private:
        vector<CPropList*> prevDepartments;
        int m_invID;
        string m_name;
};

class CPropList
{
public:
  int InvID (void) const;
  string Name (void) const;
  int Remains (void) const;
  void Next (void);
  CPropList(const string & deptName,const string & city, const string & country, Prop * thing);
  void Print(void) const;
  string getCity() const;
  string getCountry() const;
  string getDeptName() const;
  string getFullName() const;
  void addThing(Prop*&);
  int getSize()const;
  bool erasePtr(unsigned int i);
  int linearSearch(int invID) const;
  int binarySearch(int invID) const;
  void sortStuff();
  void insertionSort();
  
private:
    vector<Prop*> stuffPtr;
    string m_deptName;
    string m_country;
    string m_city;
    string fullName;
    int counter;
    
};

class CInventory
{
public:
  CInventory (void){};
   ~CInventory (void);
  bool AddProp (int invID,
		const string & name,
		const string & deptName,
		const string & city, const string & country);
  bool DelProp (int invID);
  bool Transfer (int invID,
		 const string & deptName,
		 const string & city, const string & country);
  CPropList PropList (const string & deptName,
		      const string & city, const string & country) const;
  int PropCount (const string & deptName,
		 const string & city, const string & country) const;
  void Print() const;
  
private:
    vector<CPropList *> departments;
    vector<Prop *> stuff;
    void sortStuff();
    void insertionSort();
    void insertionSortDept();
    int binarySearch(int)const;
    int binarySearchDept(string, string, string) const;
};

bool compareId(Prop * a, Prop * b){ 
    return a->getID() < b->getID();
}

bool compareFullName(CPropList * a, CPropList * b){ 
    return a->getFullName() < b->getFullName();
}

//--------------------- CINVENTORY --------------------------------

void CInventory::insertionSort() {
    int j;
    Prop * tmp;
    for (unsigned i = 1; i < stuff.size(); i++) {
        j = i;
        while (j > 0 && stuff[j - 1]->getID() > stuff[j]->getID()) {
                  tmp = stuff[j];
                  stuff[j] = stuff[j - 1];
                  stuff[j - 1] = tmp;
                  j--;
            }
      }
}
void CInventory::insertionSortDept() {
    int j;
    CPropList * tmp;
    for (unsigned i = 1; i < departments.size(); i++) {
        j = i;
        while (j > 0 && departments[j - 1]->getFullName() > departments[j]->getFullName()) {
                  tmp = departments[j];
                  departments[j] = departments[j - 1];
                  departments[j - 1] = tmp;
                  j--;
            }
      }
}

int CInventory::binarySearch(int val) const{
    int mid,left = 0, right = stuff.size() - 1;
    while(left <= right)
	{
		mid = (left +right )/2;
		if( val > stuff[mid]->getID())
			left = mid + 1;
		else if( val < stuff[mid]->getID())
			right = mid - 1;
		else
			return mid;
	}
    return -1;
}


int CInventory::binarySearchDept(string deptName, string city, string country) const{
    string fullName = deptName + "-" + city + "-" + country;
    int mid,left = 0, right = departments.size() - 1;
    while(left <= right)
	{
		mid = (left +right )/2;
		if( fullName > departments[mid]->getFullName())
			left = mid + 1;
		else if( fullName < departments[mid]->getFullName())
			right = mid - 1;
		else
			return mid;
	}
    return -1;
}



void CInventory::Print() const{
    for(unsigned int i = 0; i < departments.size();i++)
        departments[i]->Print();
}


CInventory::~CInventory (void){
    for(unsigned int i = 0; i < departments.size();i++)
        delete (departments[i]);
    
    for(unsigned int i = 0; i < stuff.size();i++)
        delete (stuff[i]);
}




bool CInventory::AddProp (int invID,const string & name, const string & deptName,const string & city, const string & country){
    if(binarySearch(invID) != -1){
        cout << "duplicitni vkladani" << endl;
        return false; 
    }
    
    Prop * thing = new Prop(invID, name);
    vector<Prop*>::iterator where = std::lower_bound(stuff.begin(), stuff.end(), thing, compareId);
    //cout << "vkladam na pozici: " << where << endl;
    stuff.insert(where,thing);
    
    int index = binarySearchDept(deptName,city,country);
    if(index !=  -1){
        departments[index]->addThing(thing);
        CPropList * department = (departments.at(index));
        thing->AddDept(department);
    }else{
        CPropList * department = new CPropList(deptName, city, country, thing);
        vector<CPropList*>::iterator it = std::lower_bound(departments.begin(), departments.end(), department, compareFullName);
        departments.insert(it,department);
        thing->AddDept(department);
    }
    
    /*cout <<"Departments:" << endl << "-----------------------------" << endl;
    
    for(unsigned int i = 0; i < departments.size();i++){
        cout << departments[i]->getFullName() << endl;
    }
    cout << "-----------------------------" << endl;*/
    return true;
}

int CInventory::PropCount (const string & deptName,
		 const string & city, const string & country) const{
    int index = binarySearchDept(deptName,city,country);
    if(index == -1){
        return 0;
    }
    return departments[index]->getSize();
}


CPropList CInventory::PropList (const string & deptName, const string & city, const string & country) const{
    int index = binarySearchDept(deptName,city,country);
    if(index != -1)
        return *(departments[index]);
    else{
        return CPropList("","","",NULL);
    }
    
}

bool CInventory::Transfer (int invID, const string & deptName, const string & city, const string & country){
    Prop * item;
    CPropList * newDepartment;
    
    //hledame Prop
    int j = binarySearch(invID);
    if(j>=0)
        item = stuff[j];
    else{
        cout << "Tato vec jeste neni v databazi, nelze prevest." << endl;
        return false;
    }
    
    if(item->getActualDept()->getCity() == city && item->getActualDept()->getCountry() == country 
                                                && item->getActualDept()->getDeptName() == deptName){
        cout << "Snaha o prevedeni na aktualni oddeleni" << endl;
        return false;
    }
    int ind = item->getActualDept()->binarySearch(invID);
    item->getActualDept()->erasePtr(ind);
    int index = binarySearchDept(deptName,city,country);
    if(index !=  -1){//ano
        departments[index]->addThing(item); //pridat vec do oddeleni
        newDepartment = (departments.at(index));
    }else{//ne, zakladame nove
        newDepartment = new CPropList(deptName, city, country, item);
        vector<CPropList*>::iterator it = std::lower_bound(departments.begin(), departments.end(), newDepartment, compareFullName);
        departments.insert(it,newDepartment);
    }
    //kazdopadne pridame Prop vazbu na nove oddeleni
    item->AddDept(newDepartment);
    return true;
}


bool CInventory::DelProp (int invID){
    //hledame Prop
    Prop * item;
    int j = binarySearch(invID);
    if(j>=0)
        item = stuff[j];
    else{
        cout << "Tato vec jeste neni v databazi, nelze odstranit." << endl;
        return false;
    }
    
    //mazeme odkaz z PropListu
    int ind = item->getActualDept()->binarySearch(invID);
    item->getActualDept()->erasePtr(ind);
    
    //mazeme odkaz ze stuff
    stuff.erase(stuff.begin() + j);
    //mazeme pamet, na kterou ukazuje uz pouze item
    delete item;
    return true;
}

//----------------------- CPROPLIST --------------------------------
void CPropList::Print() const{
    cout << "-----" <<m_deptName << " (" << m_city << ", " << m_country << ")-----" << endl;
    for(unsigned int i = 0; i < stuffPtr.size();i++){
        cout << i << ": ";
        stuffPtr[i]->Print();
        cout << endl;
    }
    cout << endl;
}

int CPropList::binarySearch(int val) const{
    int mid,left = 0, right = stuffPtr.size() - 1;
    while(left <= right)
	{
		mid = (left +right )/2;
		if( val > stuffPtr[mid]->getID())
			left = mid + 1;
		else if( val < stuffPtr[mid]->getID())
			right = mid - 1;
		else
			return mid;
	}
    return -1;
}

void CPropList::sortStuff(){
    
    std::sort(stuffPtr.begin(), stuffPtr.end(), compareId );
}

void CPropList::insertionSort() {
    int j;
    Prop * tmp;
    for (unsigned i = 1; i < stuffPtr.size(); i++) {
        j = i;
        while (j > 0 && stuffPtr[j - 1]->getID() > stuffPtr[j]->getID()) {
                  tmp = stuffPtr[j];
                  stuffPtr[j] = stuffPtr[j - 1];
                  stuffPtr[j - 1] = tmp;
                  j--;
            }
      }
}

CPropList::CPropList(const string & deptName,const string & city, const string & country, Prop * thing){
    m_city = city;
    m_country = country;
    m_deptName = deptName;
    fullName = m_deptName + "-" + m_city + "-" + m_country;
    //cout << "fullname noveho oddeleni: " << fullName << endl;
    if(thing != NULL)
        stuffPtr.push_back(thing);
    counter = 0;
    
}


int CPropList::linearSearch(int invID) const{
    for (unsigned int i = 0; i< stuffPtr.size(); i++){
        if(stuffPtr[i]->getID() == invID){
            return i;
        }
    }
    return -1;
}

void CPropList::addThing(Prop*& sth){
    vector<Prop*>::iterator it = std::lower_bound(stuffPtr.begin(), stuffPtr.end(), sth, compareId);
    stuffPtr.insert(it,sth);
}

string CPropList::getCity() const{
    return m_city;
}

string CPropList::getCountry() const{
    return m_country;
}

string CPropList::getDeptName() const{
    return m_deptName;
}

string CPropList::getFullName() const{
    return fullName;
}

int CPropList::getSize() const{
    return stuffPtr.size();
}


int CPropList::InvID (void) const{
    return stuffPtr[counter]->getID();
    
}

string CPropList::Name (void) const{
    return stuffPtr[counter]->getName();
    
}

int CPropList::Remains (void) const{
      return stuffPtr.size() - counter;
}

void CPropList::Next (void){
    counter++;
}

bool CPropList::erasePtr(unsigned int i){
    if(i < 0 || i>= stuffPtr.size() )
        return false;
    
    stuffPtr.erase(stuffPtr.begin() + i);
    return true;
}
//-------------------------------    PROP    ---------------------------------------------
Prop::Prop(int invID,const string & name){
    m_invID = invID;
    m_name = name;
}

void Prop::Print(void) const{
    cout << "-> " << m_invID << ": " << m_name;
}

int Prop::getID() const{
    return m_invID;
}

string Prop::getName() const{
    return m_name;
}

void Prop::AddDept(CPropList*& dept){
    prevDepartments.push_back(dept);
}
        
CPropList * Prop::getActualDept()const{
    return prevDepartments.back();
}

#ifndef __PROGTEST__
int
main (void)
{
    /*
  CInventory b1;
  
  b1.AddProp (100, "Notebook", "Accounting", "Prague", "CZ");
  //b1.AddProp (200, "Chair", "Accounting", "Brno", "CZ");
  
  b1.AddProp (101, "Notebook", "Human Resources", "Prague", "CZ");
  b1.AddProp (102, "Notebook", "Accounting", "Brno", "CZ");
  b1.AddProp (103, "Notebook", "Accounting", "Prague", "USA");
  b1.AddProp (104, "Desktop PC", "Accounting", "Prague", "CZ");
  
  
  b1.Print();
  
  

  assert (b1.PropCount ("Accounting", "Prague", "CZ") == 2);
  for (CPropList l = b1.PropList ("Accounting", "Prague", "CZ"); l.Remains (); l.Next ())
    cout << l.InvID () << ' ' << l.Name () << endl;
  
  
  // the following 2 items, in any order:

  // 100 Notebook
  // 104 Desktop PC

  assert (b1.PropCount ("Human Resources", "Prague", "CZ") == 1);
  for (CPropList l = b1.PropList ("Human Resources", "Prague", "CZ");
       l.Remains (); l.Next ())
    cout << l.InvID () << ' ' << l.Name () << endl;
  // the following property

  // 101 Notebook

  assert (b1.PropCount ("Accounting", "Brno", "CZ") == 1);
  for (CPropList l = b1.PropList ("Accounting", "Brno", "CZ"); l.Remains ();
       l.Next ())
    cout << l.InvID () << ' ' << l.Name () << endl;
  // the following property

  // 102 Notebook

  assert (b1.PropCount ("Accounting", "Prague", "USA") == 1);
  for (CPropList l = b1.PropList ("Accounting", "Prague", "USA");
       l.Remains (); l.Next ())
    cout << l.InvID () << ' ' << l.Name () << endl;
  // the following property

  // 103 Notebook

  assert (b1.Transfer (104, "Accounting", "Prague", "USA") == true);
  b1.Print();

  
    assert (b1.PropCount ("Accounting", "Prague", "CZ") == 1);
  for (CPropList l = b1.PropList ("Accounting", "Prague", "CZ"); l.Remains ();
       l.Next ())
    cout << l.InvID () << ' ' << l.Name () << endl;
  // the following property

  // 100 Notebook

  assert (b1.PropCount ("Accounting", "Prague", "USA") == 2);
  for (CPropList l = b1.PropList ("Accounting", "Prague", "USA");
       l.Remains (); l.Next ())
    cout << l.InvID () << ' ' << l.Name () << endl;
  // the following 2 items, in any order:

  // 103 Notebook
  // 104 Desktop PC

  assert (b1.DelProp (104) == true);
  assert (b1.PropCount ("Accounting", "Prague", "USA") == 1);
  for (CPropList l = b1.PropList ("Accounting", "Prague", "USA");
       l.Remains (); l.Next ())
    cout << l.InvID () << ' ' << l.Name () << endl;
  // the following property

  // 103 Notebook

  assert (b1.AddProp (104, "Chair", "Public relations", "Paris", "France") ==
	  true);
  assert (b1.PropCount ("Public relations", "Paris", "France") == 1);
  for (CPropList l = b1.PropList ("Public relations", "Paris", "France");
       l.Remains (); l.Next ())
    cout << l.InvID () << ' ' << l.Name () << endl;
  // the following property

  // 104 Chair

  CInventory b2;
  assert (b2.AddProp (100, "Notebook", "Accounting", "Prague", "CZ") == true);
  assert (b2.AddProp (101, "Notebook", "Human Resources", "Prague", "CZ") ==
	  true);
  b2.Print();
  assert (b2.AddProp (100, "Table", "CEO Office", "Prague", "CZ") == false);
  b2.Print();
  assert (b2.DelProp (102) == false);
  assert (b2.Transfer (102, "CEO Office", "Prague", "CZ") == false);
  assert (b2.Transfer (100, "Accounting", "Prague", "CZ") == false);
  assert (b2.DelProp (100) == true);
  assert (b2.DelProp (100) == false);
  assert (b2.PropCount ("Facility Services", "Prague", "CZ") == 0);
  for (CPropList l = b2.PropList ("CEO Office", "Prague", "USA");
       l.Remains (); l.Next ())
    cout << l.InvID ()<< ": " << l.Name () << endl;
  // empty output
  
  
*/
  return 0;
}
#endif /* __PROGTEST__ */

// Hate my formatting? Get rid of it!  man 1 indent
//Thanks!