#ifndef GROUP1HEADER
#define GROUP1HEADER

#include "ArrayItem_v2.h"
#include "itemArray_v2.h"

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





#endif
 