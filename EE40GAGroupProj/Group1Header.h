#ifndef GROUP1HEADER
#define GROUP1HEADER

#include "ArrayItem_v2.h"
#include "itemArray_v2.h"

//for generating random string
static const char alphanum[] =
//"0123456789"
//"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

class integer_itemWithLimits :public integer_item {
protected:
	const static int default_min = -50, default_max = 50;
	int min_val = default_min, max_val = default_max;
	bool debug_mode;
	bool printboundaries;

	bool withinBounds(int input_value)
	{
		return ((input_value >= min_val) & (input_value <= max_val));
	}
	void inpoutBoundaries(int in_min, int in_max)
	{
		min_val = in_min;
		max_val = in_max;
	}

public:
	//constructor
	integer_itemWithLimits(bool debug_flag = false, bool printboundariesflag = false)
	{
		itemTypeName = "Integer_itemWithLimits";
		debug_mode = debug_flag;
		printboundaries = printboundariesflag;
	}
	//destructor
	~integer_itemWithLimits(){ 
		if (debug_mode)
		{
			cout << "integer_item destructor called" << endl;
		}
	}

	virtual void generateRandomItem()
	{
		generateRandomItemWithinLimits(min_val, max_val);
	}

	void inputBoundariesFromKeyboard()
	{
		if (isLocked())
			cout << "Error in inputBoundariesFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "Insert integer lower boundary then hit enter." << endl;
			cin >> min_val;
			cout << endl;
			cout << "Insert integer upper boundary then hit enter." << endl;
			cin >> max_val;
			cout << endl;

			// item filled
			empty = false;
		}
	}

	virtual void enterItemFromKeyboard()
	{
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "Insert integer element then hit enter." << endl;
			int temp_value;
			cin >> temp_value;
			if (withinBounds(temp_value))
			{
				item_value = temp_value;
				cout << endl;
				// item filled
				empty = false;
			}
			else
			{
				cout << "Error in enterItemFromKeyboard: Input value is out of bounds" << endl;
			}
			
		}
	}

	virtual void printItemOnScreen()
	{
		if (isEmpty())
		{
			cout << "Item is empty." << endl;
		}
		else
		{
			cout << "Item value is " << item_value;
			if (printboundaries) 
			{
				cout << ", Boundaries are " << min_val << " : " << max_val;
			}
			cout << " . " << endl;
		}
	}

	
};



    
class integer_AvgCGS_mark :public integer_itemWithLimits {
protected:
	const static int default_min = 0, default_max = 22;
public:
	//constructor
	integer_AvgCGS_mark()
	{
		itemTypeName = "integer_AvgCGS_mark";
		inpoutBoundaries(default_min, default_max);
	}
	//destructor
	~integer_AvgCGS_mark() { ; }
	
	virtual void enterItemFromKeyboard()
	{
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "Insert Average CGS mark for current level, then hit enter." << endl;
			int temp_value;
			cin >> temp_value;
			if (withinBounds(temp_value))
			{
				item_value = temp_value;
				cout << endl;
				// item filled
				empty = false;
			}
			else
			{
				cout << "Error in enterItemFromKeyboard: Input value is out of bounds (0-22)" << endl;
			}

		}
	}

	virtual void printItemOnScreen()
	{
		if (isEmpty())
		{
			cout << "Item is empty." << endl;
		}
		else
		{
			cout << "Average CGS Mark: " << item_value;
			if (printboundaries)
			{
				cout << ", Boundaries are: " << min_val << " : " << max_val;
			}
			cout << " . " << endl;
		}
	}
};


class string_item :public basic_item {
protected:
	string stringValue;
	const static int defaultRandomStringSize = 5;
	int randomStringSize = defaultRandomStringSize;
	char genRandom()
	{
		return alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	void generateRandom(int stringSize = defaultRandomStringSize)
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			stringValue = "";
			for (unsigned int i = 0; i < stringSize; ++i)
			{
				stringValue += genRandom();
			}
			// item filled
			empty = false;
		}
	}

public:
	string_item() { itemTypeName = "string_item"; }
	~string_item() { ; }
	virtual void printItemOnScreen()
	{
		if (isEmpty())
		{
			cout << "Item is empty." << endl;
		}
		else
		{
			cout << "Item value is " << stringValue << " . " << endl;
		}
	}
	
	string getItemVal() { return stringValue; }

	virtual void enterItemFromKeyboard()
	{
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "Insert string element then hit enter." << endl;
			cin >> stringValue;
			cout << endl;
			// item filled
			empty = false;
		}
	}

	virtual void generateRandomItem()
	{
		generateRandom(randomStringSize);
	}

	void inputRandomStringSize()
	{
		if (isLocked())
			cout << "Error in inputRandomStringSize: Item is locked" << endl;
		else
		{
			cout << "Insert integer element then hit enter." << endl;
			cin >> randomStringSize;
			cout << endl;
		}
	}

	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new string_item;
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
			string_item* typecasted_other_item = typecastItem(itemToDestroy, this);
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
			string_item* typecasted_other_item = typecastItem(other_item, this);
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

	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;

		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		string_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// now verify if the other item is larger than the curren
		if ((stringValue.compare(typecasted_other_item->getItemVal())) > 0) {result = true; }

		// chek if there are sorting options to apply 
		if (sort_criteria != NULL)
		{
			// if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

	

};
#endif
 
