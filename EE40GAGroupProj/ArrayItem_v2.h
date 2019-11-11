#ifndef __ARRAYITEM_V2_H_
#define __ARRAYITEM_V2_H_

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <string>

#include <iostream>
using namespace std;


////// this is needed to check compatibility between items (and to use a derived-class pointer that is passes as "base class")
template <class Base, class Derived> Derived* typecastItem(Base* basic_ptr, Derived* derivedItem_ptr)
{
	Derived* typecasted_ptr=dynamic_cast <Derived *>(basic_ptr);

	if(typecasted_ptr==NULL)				
		printf("\n Error typecasting item: type does not match the expected derived item\n");

	return typecasted_ptr;
}
//////


class basic_sort_criteria{
protected:
	bool ascending;	//	
public:
	basic_sort_criteria(){setAscending(true);}
	void setAscending(bool value){ascending=value;}
	bool getAscending(){return ascending;}
	virtual void setOptionFromKeyboard()
	{
		char sortascending;
		cout << "Sort option is ASCENDING? Type Y and press ENTER" << endl;
		cin >> sortascending;
		if( (sortascending=='y') || (sortascending == 'Y') )
			setAscending(true);
		else
			setAscending(false);
	}
	virtual void printOptionToScreen()
	{
		cout << "Sort option is ";
		if(getAscending())
			cout << "ASCENDING" << endl;
		else
			cout << "DISCENDING" << endl;		
	}
};

//
class array_manipulator;
//class item_array;

class basic_item{
protected:
	bool empty;
	string itemTypeName;	
	bool locked;
	
	// Used (later on) by the general_item_array:
	int includedBy;
	void incrementincludedBy() { includedBy++; }
	void decrementincludedBy() { includedBy=( (includedBy>0) ? (includedBy-1) : 0 ); }
	// end test
public:
	basic_item() { empty = true; locked = false;  includedBy = 1; }
	~basic_item() { ; }
	bool isEmpty(){return empty;}
	bool isLocked() { return locked; }
	virtual void setLocked(bool lock_input)
	{
		if(!isEmpty())
			locked = lock_input;
	}
	int isIncludedBy() {return includedBy;}
	void printItemTypeName() { cout << endl << "Item type: " << itemTypeName << endl; }
	//These must be implemented by any derived item	
	virtual void printItemOnScreen()=0;
	virtual void enterItemFromKeyboard()=0;
	//virtual void loadItemFromFile(FILE* fin)=0;
	virtual void generateRandomItem()=0;	
	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria=NULL)=0;
	virtual basic_item* allocateEmptyItem()=0;
	virtual void deallocateItem(basic_item* itemToDestroy) = 0;
	//virtual basic_item* makeCopyofItem()=0;
	virtual bool compatibilityCheck(basic_item* other_item) = 0;		

	//friend bool fromSourceToDestinationAarray(item_array& sourceArray, int startPosSour, int totElem, item_array& destArray, int startPosDest, bool removeFromSource);
	//friend void general_item_array::incrementIncludedBy(basic_item* item);
	//friend class array_manipulator;
	//friend class item_array;
	friend class general_item_array;
};

//
class integer_item: public basic_item{
protected:
	int item_value;	
public:
	integer_item(){itemTypeName = "integer_item";}
	~integer_item() { cout << "integer_item destructor called" << endl; } // can remove the printout after testing

	int getItemVal(){return item_value;}
	
	virtual void printItemOnScreen()
	{
		if(isEmpty())
			cout << "Item is empty." << endl;
		else
			cout << "Item value is " << item_value << " . " << endl;
	}

	virtual void enterItemFromKeyboard()
	{
		if(isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "Insert integer element then hit enter." << endl;
			cin >> item_value;
			cout << endl;

			// item filled
			empty = false;
		}		
	}

	virtual void generateRandomItem()
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			int item;
			int max_rand_val = 1000;

			item = rand();
			item = item % (max_rand_val + 1);

			// turn to negative 30% of the time
			if ((rand() % 10) >= 7)
				item = (-1)*item;

			item_value = item;
			// item filled
			empty = false;
		}
	}
	
	void generateRandomItemWithinLimits(int min_val, int max_val)
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			int item;

			if (min_val > max_val)
			{
				int temp = min_val;
				min_val = max_val;
				max_val = temp;
			}

			int max_rand_val = max_val - min_val;

			item = rand();
			item = item % (max_rand_val);
			item = item + min_val;
			item_value = item;
			// item filled
			empty = false;
		}
	}

	//virtual void loadItemFromFile(FILE* fin);
	
	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria=NULL)
	{
		bool result=false;
		
		// if the other item is "empty" (non allocated) don't do any comparison
		if(other_item==NULL)
			return false;

		
		// first typecast the other item to confimr it is the same as this;
		integer_item* typecasted_other_item = typecastItem(other_item, this);
		
		// check that it worked
		if(typecasted_other_item==NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// now verify if the other item is larger than the curren
		if( getItemVal() > (typecasted_other_item->getItemVal()) )
			result=true;


		// chek if there are sorting options to apply 
		if(sort_criteria!=NULL)
		{
			// if sorting is in descending order the result is reversed 
			if( !( sort_criteria->getAscending() ) )
				result=!result;
		}

		return result;
	}

	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new integer_item;
		if (result == NULL)
		{
			cout << endl << "Out of memeory allocating ";
			cout << itemTypeName << endl;
		}	
		return result;
	}
	virtual void deallocateItem(basic_item* itemToDestroy)
	{
		if (itemToDestroy != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			integer_item* typecasted_other_item = typecastItem(itemToDestroy, this);
			delete typecasted_other_item;
		}
	}
		
	virtual bool compatibilityCheck(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item != NULL)
		{
			// typecast the other item to confirm it is the same as this;
			integer_item* typecasted_other_item = typecastItem(other_item, this);
			if (typecasted_other_item != NULL)
				result = true;
			else
			{
				cout << endl << "Check failed for Item type: ";
				cout << itemTypeName << endl;
			}
		}
		return result;
	}
	
};



class intmat_sort_criteria : public basic_sort_criteria{
public:
	enum matsortoptions{inf_val=0, sort_maxval, sort_determ, sup_val};
private:
	matsortoptions thesortoption;
public:	
	intmat_sort_criteria() {setOption(sort_maxval);}
	void setOption(matsortoptions value)
	{ 
		if( (value>inf_val) && (value < sup_val) )
			thesortoption = value;
		else
			thesortoption = sort_maxval;
	}
	matsortoptions getOption() { return thesortoption; }
	virtual void setOptionFromKeyboard()
	{
		char sortoption;
		cout << "Sort option: Type M for MAXVAL; Type D for DETERMINANT; then press ENTER" << endl;
		cin >> sortoption;
		switch (sortoption) {
		case 'M':
		case 'm':
			setOption(sort_maxval);
			break;
		case 'D':
		case 'd':
			setOption(sort_determ);
			break;
		}
		basic_sort_criteria::setOptionFromKeyboard();
	}
	virtual void printOptionToScreen()
	{
		cout << "intmat Sorting by ";
		switch (getOption()) {
		case sort_maxval:
			cout << "MAXVAL " << endl;
			break;
		case sort_determ:
			cout << "DETERMINANT " << endl;
			break;
		}
		basic_sort_criteria::printOptionToScreen();
	}
};

class intmat_item: public basic_item{
protected:
	static const int matsize=2;
	int matrix[matsize][matsize];

	int getMatVal(int i, int j)
	{
		if( (i>=0 && i<matsize) && (j>=0 && j<matsize) && (!isEmpty()) )
		{
			return matrix[i][j];
		}
		else
			return 0;
	}

	void setMatVal(int i, int j, int val)
	{
		if( (i>=0 && i<matsize) && (j>=0 && j<matsize) && (isEmpty()) )
		{
			matrix[i][j] = val;
		}
	}

public:
	intmat_item(){itemTypeName = "intmat_item";}
	~intmat_item() { cout << "intmat_item destructor called" << endl; } // can remove the printout after testing
	
	//find and report the largest value in the matrix
	int getMaxVal()
	{
		int max=getMatVal(0, 0);
		for(int i=0; i<matsize; i++)
			for(int j=0; j<matsize; j++)
				if( max < getMatVal(i,j) )
					max=getMatVal(i,j);
		return max;
	}
	
	//compute and report matrix determinant
	int getDet()
	{	
		if (matsize == 2)
		{
			int maindiag = getMatVal(0, 0) * getMatVal(1, 1);
			int secdiag = getMatVal(0, 1) * getMatVal(1, 0);
			return (maindiag * ((-1)*secdiag));
		}
		cout << endl << " Determinant not ready for general case " << endl;
		return 0;
	}

	virtual void printItemOnScreen()
	{
		if(isEmpty())
			cout << "Item is empty." << endl;
		else
		{
			for(int i=0; i<matsize; i++)
				for(int j=0; j<matsize; j++)
					cout << "Item pos "<< i << " , " << j << " value is " << getMatVal(i,j) << " . " << endl;
			cout << endl;
		}
	}

	virtual void enterItemFromKeyboard()
	{
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			int val;
			for (int i = 0; i < matsize; i++)
				for (int j = 0; j < matsize; j++)
				{
					cout << "Enter integer value for";
					cout << "Item pos " << i << " , " << j << " :" << endl;
					cin >> val;
					setMatVal(i, j, val);
				}
			cout << "Matrix filled" << endl;
		}
	}

	virtual void generateRandomItem()
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			int item;
			int max_rand_val = 100;

			for (int i = 0; i < matsize; i++)
				for (int j = 0; j < matsize; j++)
				{

					item = rand();
					item = item % (max_rand_val + 1);

					// turn to negative 30% of the time
					if ((rand() % 10) >= 7)
						item = (-1)*item;

					setMatVal(i, j, item);
				}
			// item filled
			empty = false;
		}
	}
	
	//virtual void loadItemFromFile(FILE* fin);
	
	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria=NULL)
	{
		bool result=false;
		intmat_sort_criteria matSortOption;
		
		// if the other item is "empty" (non allocated) don't do any comparison
		if(other_item==NULL)
			return false;

		
		// first typecast the other item to confimr it is the same as this;
		intmat_item* typecasted_other_item = typecastItem(other_item, this);
		
		// check that it worked
		if(typecasted_other_item==NULL)
		{
			cout << "Other item is not of type intmat2x2_item." << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// check if the sort_option is specific for the int_mat
		if (sort_criteria != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			intmat_sort_criteria* typecasted_sortoption = typecastItem(sort_criteria, &matSortOption);
			if (typecasted_sortoption != NULL)
				matSortOption.setOption(typecasted_sortoption->getOption());
		}

		// now verify if the other item is larger than the curren
		switch (matSortOption.getOption()) {
		case(intmat_sort_criteria::sort_maxval):
			if (getMaxVal() > (typecasted_other_item->getMaxVal()))
				result = true;
			break;
		case(intmat_sort_criteria::sort_determ):
			if( getDet() > (typecasted_other_item->getDet()) )
				result = true;
			break;
		}
		
		// chek if ascending/decenting sorting applies 
		if(sort_criteria!=NULL)
		{
			// if sorting is in descending order the result is reversed 
			if( !( sort_criteria->getAscending() ) )
				result=!result;
		}

		return result;
	}

	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new intmat_item;
		if (result == NULL)
		{
			cout << endl << "Out of memory allocating ";
			cout << itemTypeName << endl;
		}
		return result;
	}
	virtual void deallocateItem(basic_item* itemToDestroy)
	{
		if (itemToDestroy != NULL)
		{
			// first typecast the other item to confirm it is the same as this;
			intmat_item* typecasted_other_item = typecastItem(itemToDestroy, this);
			delete typecasted_other_item;
		}
	}
	virtual bool compatibilityCheck(basic_item* other_item)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item != NULL)
		{
			// typecast the other item to confirm it is the same as this;
			intmat_item* typecasted_other_item = typecastItem(other_item, this);
			if (typecasted_other_item != NULL)
				result = true;
			else
			{
				cout << endl << "Check failed for Item type: ";
				cout << itemTypeName << endl;
			}
		}
		return result;
	}

};


//#include "generalArray_v2.h"

#endif