#ifndef GROUP1HEADER
#define GROUP1HEADER

#include "ArrayItem_v2.h"
#include "generalArray_v2.h"

//for generating random string
static const char alphanum[] =
//"0123456789"
//"!@#$%^&*"
//"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";


static const string countrylist[] =//only including europoean for now
{ 
	"blank",
	"Austria",
	"Belgium",
	"Bulgaria",
	"Croatia",
	"Cyprus",
	"Czech Republic",
	"Denmark",
	"Estonia",
	"Finland",
	"France",
	"Germany",
	"Greece",
	"Hungary",
	"Ireland",
	"Italy",
	"Latvia",
	"Lithuania",
	"Luxembourg",
	"Malta",
	"Netherlands",
	"Poland",
	"Portugal",
	"Romania",
	"Slovak Republic",
	"Slovenia",
	"Spain",
	"Sweden",
	"United Kingdom" };

static const string programmelist[] =//only including engineering for now
{ 
	"blank",
	"Electrical & Electronic engineering",
	"Software engineering",
	"Chemical engineering",
	"Mechanical engineering",
	"Civil engineering",
};

static const string bloodtypelist[] =
{	
	"blank"
	"O+",
	"A+",
	"B+",
	"AB+",
	"O-",
	"A-",
	"B-",
	"AB-"
};

static const string firstnamelist[] =
{
	"blank",
	"David",
	"John",
	"Paul",
	"Mark",
	"James",
	"Andrew",
	"Scott",
	"Steven",
	"Robert",
	"Stephen"
};

static const string lastnamelist[] =
{
	"blank",
	"Smith",
	"Brown",
	"Wilson",
	"Stewart",
	"Robertson",
	"Thomson",
	"Campbell",
	"Anderson",
	"Scott",
	"Reid"
};

class integer_itemWithLimits :virtual public integer_item {
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
	void inputBoundaries(int in_min, int in_max)
	{
		min_val = in_min;
		max_val = in_max;
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


	//Need to add these for when array is called
	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new integer_itemWithLimits;
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
			integer_itemWithLimits* typecasted_other_item = typecastItem(itemToDestroy, this);
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
			integer_itemWithLimits* typecasted_other_item = typecastItem(other_item, this);
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

class integer_AvgCGS_mark : virtual public integer_itemWithLimits {// adding virtual to solve double inheritance problem
protected:
	const static int default_min = 0, default_max = 22;
public:
	//constructor
	integer_AvgCGS_mark()
	{
		itemTypeName = "integer_AvgCGS_mark";
		inputBoundaries(default_min, default_max);
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


	//Need to add these for when array is called
	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new integer_AvgCGS_mark;
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
			integer_AvgCGS_mark* typecasted_other_item = typecastItem(itemToDestroy, this);
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
			integer_AvgCGS_mark* typecasted_other_item = typecastItem(other_item, this);
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

class string_item : virtual public basic_item {
protected:
	string stringValue;
	const static int defaultRandomStringSize = 5;
	int randomStringSize = defaultRandomStringSize;
	char genRandom()
	{
		return alphanum[rand() % (sizeof(alphanum) - 1)];
	}

public:
	string_item() { itemTypeName = "string_item"; }
	~string_item() { ; }

	void generateRandom(int stringSize = defaultRandomStringSize)
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			stringValue = "";
			for (int i = 0; i < stringSize; ++i)
			{
				stringValue += genRandom();
			}
			// item filled
			empty = false;
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
			cout << "Item value is " << stringValue << " . " << endl;
		}
	}
	
	void string_assign_value(string source)
	{
		if (isLocked())
			cout << "Error in string_assign_value: Item is locked" << endl;
		{
			stringValue = "";
			for (int i = 0; i < source.length(); ++i)
			{
				stringValue += source[i];
			}
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



	//Need to add these for when array is called
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
		basic_sort_criteria stringsortoption;
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

		// check if the sort_option is specific for the string
		if (sort_criteria != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			basic_sort_criteria* typecasted_sortoption = typecastItem(sort_criteria, &stringsortoption);
			if (typecasted_sortoption != NULL)
				stringsortoption.setAscending(typecasted_sortoption->getAscending());
		}



		// now verify if the other item is larger than the curren
		if ((stringValue.compare(typecasted_other_item->getItemVal())) < 0) { result = true; }

		// chek if there are sorting options to apply !!!not currently used!!!
		if (sort_criteria != NULL)
		{
			//if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}

};

class task2_sort_criteria : public basic_sort_criteria {
public:
	enum compositesortoptions { nosort = 0, firstname, lastname, level, grade, opt_stop };
private:
	compositesortoptions sort_option;
public:
	virtual void setOption(compositesortoptions inp_option)
	{
		if ((inp_option > nosort) && (inp_option < opt_stop))
			sort_option = inp_option;
		else
			sort_option = nosort;
	}
	task2_sort_criteria() { setOption(nosort); }
	compositesortoptions getOption() { return sort_option; }
	virtual void setOptionFromKeyboard()
	{
		char sortoption;
		cout << "Sort option: F-First name; L-Last name; Y-Level(Year); G-Grade; then press ENTER" << endl;
		cin >> sortoption;
		switch (sortoption) {
		case 'F':
		case 'f':
			setOption(firstname);
			break;
		case 'L':
		case 'l':
			setOption(lastname);
			break;
		case 'Y':
		case 'y':
			setOption(level);
			break;
		case 'G':
		case 'g':
			setOption(grade);
			break;
		}
		basic_sort_criteria::setOptionFromKeyboard();
		
	}
	virtual void printOptionToScreen()
	{
		cout << "String Sorting by ";
		switch (getOption()) {
		case firstname:
			cout << "Firstname ";
			break;
		case lastname:
			cout << "Lastname ";
			break;
		case level:
			cout << "Level ";
			break;
		case grade:
			cout << "Grade ";
			break;
		}
		if (basic_sort_criteria::getAscending())
		{
			cout << "ascending." << endl;
		}
		else
		{
			cout << "descending." << endl;
		}
	}
};

class task_2_item: virtual public string_item{
protected:
	static const int MAX_LEVELS = 5, default_length_firstname = 4, default_length_lastname = 6;
	integer_itemWithLimits current_level;
	integer_AvgCGS_mark avg_mark;
	string_item firstname, lastname;//not using middle names

public:
	task_2_item()
	{
		basic_item::itemTypeName = "task_2_item";
	}
	virtual void generateRandomItem()
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			firstname.generateRandom(default_length_firstname);
			lastname.generateRandom(default_length_lastname);
			current_level.generateRandomItemWithinLimits(1, MAX_LEVELS);
			avg_mark.generateRandomItem();
			empty = false;
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
			cout << "Name:" << firstname.getItemVal() << " " << lastname.getItemVal();
			cout << ", Current level: " << current_level.getItemVal() << " Average mark: " << avg_mark.getItemVal() <<  endl;
		}
	}

	virtual void enterItemFromKeyboard()
	{
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "First name:" << endl;
			firstname.enterItemFromKeyboard();
			cout << "Last name:" << endl;
			lastname.enterItemFromKeyboard();
			cout << "Current level:" << endl;
			current_level.enterItemFromKeyboard();
			cout << "Average grade:" << endl;
			avg_mark.enterItemFromKeyboard();
			empty = false;
		}
	}

	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;
		task2_sort_criteria compositesortoption;
		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		task_2_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// check if the sort_option is specific for the string
		if (sort_criteria != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			task2_sort_criteria* typecasted_sortoption = typecastItem(sort_criteria, &compositesortoption);
			if (typecasted_sortoption != NULL)
				compositesortoption.setOption(typecasted_sortoption->getOption());
		}


		// now verify if the other item is larger than the curren
		switch (compositesortoption.getOption()) {
		case(task2_sort_criteria::firstname):
			if ((firstname.getItemVal().string::compare(typecasted_other_item->firstname.getItemVal())) < 0)//default to ascending order
				result = true;
			break;
		case(task2_sort_criteria::lastname):
			if ((lastname.getItemVal().string::compare(typecasted_other_item->lastname.getItemVal())) < 0)
				result = true;
			break;
		case(task2_sort_criteria::level):
			if (current_level.getItemVal() > typecasted_other_item->current_level.getItemVal())
				result = true;
			break;
		case(task2_sort_criteria::grade):
			if (avg_mark.getItemVal() > typecasted_other_item->avg_mark.getItemVal())
				result = true;
			break;
		}


		// chek if there are more sorting options to apply
		if (sort_criteria != NULL)
		{
			//if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}


	//Need to add these for when array is called
	virtual basic_item* allocateEmptyItem()
	{
		basic_item* result = new task_2_item;
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
			task_2_item* typecasted_other_item = typecastItem(itemToDestroy, this);
			delete typecasted_other_item;
		}
	}


};

class task3_sort_criteria : public basic_sort_criteria {
public:
	enum compositesortoptions { nosort = 0, firstname, lastname,
		nationality, idnumber, programme, level, grade, height, weight,
		dob, bloodtype, opt_stop};
private:
	compositesortoptions sort_option;
public:
	virtual void setOption(compositesortoptions inp_option)
	{
		if ((inp_option > nosort) && (inp_option < opt_stop))
			sort_option = inp_option;
		else
			sort_option = nosort;
	}
	task3_sort_criteria() { setOption(nosort); }
	compositesortoptions getOption() { return sort_option; }
	virtual void setOptionFromKeyboard()
	{
		char sortoption;
		cout << "Sort option:" << endl;
		cout << "F-First name; L-Last name; N-Nationality;" << endl;
		cout << "I- ID#; P-Degree Programme; Y-Level(Year); G-Grade;" << endl;
		cout << "H-Height; W-Weight; B-Blood type; D-Date of Birth;" << endl;
		cout <<	"Press ENTER when done" << endl;
		cin >> sortoption;
		switch (sortoption) {
		case 'F':
		case 'f':
			setOption(firstname);
			break;
		case 'L':
		case 'l':
			setOption(lastname);
			break;
		case 'N':
		case 'n':
			setOption(nationality);
			break;
		case 'I':
		case 'i':
			setOption(idnumber);
			break;
		case 'P':
		case 'p':
			setOption(programme);
			break;
		case 'Y':
		case 'y':
			setOption(level);
			break;
		case 'G':
		case 'g':
			setOption(grade);
			break;
		case 'H':
		case 'h':
			setOption(height);
			break;
		case 'W':
		case 'w':
			setOption(weight);
			break;
		case 'B':
		case 'b':
			setOption(bloodtype);
			break;
		case 'D':
		case 'd':
			setOption(dob);
			break;
		}
		basic_sort_criteria::setOptionFromKeyboard();

	}
	virtual void printOptionToScreen()
	{
		cout << "String Sorting by ";
		switch (getOption()) {
		case firstname:
			cout << "Firstname ";
			break;
		case lastname:
			cout << "Lastname ";
			break;
		case level:
			cout << "Level ";
			break;
		case grade:
			cout << "Grade ";
			break;
		}
		if (basic_sort_criteria::getAscending())
		{
			cout << "ascending." << endl;
		}
		else
		{
			cout << "descending." << endl;
		}
	}
};

class studentrecord_item : public task_2_item{
protected:
	string_item nationality, programme, blood_type;
	integer_itemWithLimits weight, height, studentid, dateofbirth[3];
	static const int default_min_weight = 70,
		default_max_weight = 130,
		default_min_height = 140,
		default_max_height = 220,
		default_min_year = 1980,
		default_max_year = 2002;

	int randomIntWithBounds(int low_val, int high_val)
	{
		return rand() % high_val + low_val;
	}

public:
	studentrecord_item()
	{
		basic_item::itemTypeName = "studentrecord_item";
		nationality.string_assign_value(countrylist[0]);//assigning first value
		programme.string_assign_value(programmelist[0]);//assigning first value
		blood_type.string_assign_value(bloodtypelist[0]);//assigning first value
		firstname.string_assign_value(firstnamelist[0]);//assigning first value
		lastname.string_assign_value(lastnamelist[0]);//assigning first value
		dateofbirth[0].inputBoundaries(1, 31);
		dateofbirth[1].inputBoundaries(1, 12);
		dateofbirth[2].inputBoundaries(default_min_year, default_max_year);
		weight.inputBoundaries(default_min_weight, default_max_weight);
		height.inputBoundaries(default_min_height, default_max_height);
		studentid.inputBoundaries(1000, 9999);//using 4 digit id

		//technically id could start with 0, but wanted to have always same number of digits visible in console
	}

	virtual void generateRandomItem()
	{
		if (isLocked())
			cout << "Error in generateRandomItem: Item is locked" << endl;
		else
		{
			weight.generateRandomItem();
			height.generateRandomItem();
			studentid.generateRandomItem();
			blood_type.string_assign_value(bloodtypelist[randomIntWithBounds(1, sizeof(bloodtypelist) / sizeof(*bloodtypelist) - 1)]);
			programme.string_assign_value(programmelist[randomIntWithBounds(1, sizeof(programmelist) / sizeof(*programmelist) - 1)]);
			nationality.string_assign_value(countrylist[randomIntWithBounds(1, sizeof(countrylist) / sizeof(*countrylist) - 1)]);
			firstname.string_assign_value(firstnamelist[randomIntWithBounds(1, sizeof(firstnamelist) / sizeof(*firstnamelist) - 1)]);
			lastname.string_assign_value(lastnamelist[randomIntWithBounds(1, sizeof(lastnamelist) / sizeof(*lastnamelist) - 1)]);
			dateofbirth[2].generateRandomItem();
			dateofbirth[1].generateRandomItem();
			dateofbirth[0].generateRandomItem();
			current_level.generateRandomItemWithinLimits(1, MAX_LEVELS);
			avg_mark.generateRandomItem();

			empty = false;
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
			cout << "Name: " << firstname.getItemVal() << " " << lastname.getItemVal() << "; Nationality: " << nationality.getItemVal() << endl;
			cout << "Programme: " << programme.getItemVal() << "; ID#: " << studentid.getItemVal() << "; Level: " << current_level.getItemVal()
				<< "; Average grade: " << avg_mark.getItemVal() << endl;
			cout << "Date of birth: " << dateofbirth[0].getItemVal() << "-" << dateofbirth[1].getItemVal() << "-" << dateofbirth[2].getItemVal()
				<< "; Height: " << height.getItemVal() << "; Weight: " << weight.getItemVal() << "; Blood Type:" << blood_type.getItemVal() << endl;
		}
	}

	virtual void enterItemFromKeyboard()
	{
		if (isLocked())
			cout << "Error in enterItemFromKeyboard: Item is locked" << endl;
		else
		{
			cout << "First name:" << endl;
			firstname.enterItemFromKeyboard();
			cout << "Last name:" << endl;
			lastname.enterItemFromKeyboard();
			cout << "Nationality:" << endl;
			nationality.enterItemFromKeyboard();
			cout << "Programme:" << endl;
			programme.enterItemFromKeyboard();
			cout << "Student ID:" << endl;
			studentid.enterItemFromKeyboard();
			cout << "Current level:" << endl;
			current_level.enterItemFromKeyboard();
			cout << "Day of birth:" << endl;
			dateofbirth[0].enterItemFromKeyboard();
			cout << "Month of birth:" << endl;
			dateofbirth[1].enterItemFromKeyboard();
			cout << "Year of birth:" << endl;
			dateofbirth[2].enterItemFromKeyboard();
			cout << "Height:" << endl;
			height.enterItemFromKeyboard();
			cout << "Weight:" << endl;
			weight.enterItemFromKeyboard();
			cout << "Blood Type:" << endl;
			blood_type.enterItemFromKeyboard();

			empty = false;
		}
	}

	virtual bool IsLargerThan(basic_item* other_item, basic_sort_criteria* sort_criteria = NULL)
	{
		bool result = false;
		task3_sort_criteria compositesortoption;
		// if the other item is "empty" (non allocated) don't do any comparison
		if (other_item == NULL)
			return false;


		// first typecast the other item to confimr it is the same as this;
		studentrecord_item* typecasted_other_item = typecastItem(other_item, this);

		// check that it worked
		if (typecasted_other_item == NULL)
		{
			cout << endl << "Item type is: ";
			cout << itemTypeName << endl;
			return false;
			// items of the wrong type (or null pointers) will be pushed to the end of the list
		}

		// check if the sort_option is specific for the string
		if (sort_criteria != NULL)
		{
			// first typecast the other item to confimr it is the same as this;
			task3_sort_criteria* typecasted_sortoption = typecastItem(sort_criteria, &compositesortoption);
			if (typecasted_sortoption != NULL)
				compositesortoption.setOption(typecasted_sortoption->getOption());
		}

		// now verify if the other item is larger than the curren
		switch (compositesortoption.getOption()) {
		case(task3_sort_criteria::firstname):
			if (firstname.getItemVal().string::compare(typecasted_other_item->firstname.getItemVal()) < 0)//default to ascending order
				result = true;
			break;
		case(task3_sort_criteria::lastname):
			if (lastname.getItemVal().string::compare(typecasted_other_item->lastname.getItemVal()) < 0)//default to ascending order
				result = true;
			break;
		case(task3_sort_criteria::nationality):
			if (nationality.getItemVal().string::compare(typecasted_other_item->nationality.getItemVal()) < 0)//default to ascending order
				result = true;
			break;
		case(task3_sort_criteria::programme):
			if (programme.getItemVal().string::compare(typecasted_other_item->programme.getItemVal()) < 0)//default to ascending order
				result = true;
			break;
		case(task3_sort_criteria::idnumber):
			if (studentrecord_item::studentid.getItemVal() > typecasted_other_item->studentid.getItemVal())
				result = true;
			break;
		case(task3_sort_criteria::level):
			if (studentrecord_item::current_level.getItemVal() > typecasted_other_item->current_level.getItemVal())
				result = true;
			break;
		case(task3_sort_criteria::grade):
			if (studentrecord_item::avg_mark.getItemVal() > typecasted_other_item->avg_mark.getItemVal())
				result = true;
			break;
		case(task3_sort_criteria::height):
			if (studentrecord_item::height.getItemVal() > typecasted_other_item->height.getItemVal())
				result = true;
			break;
		case(task3_sort_criteria::weight):
			if (studentrecord_item::weight.getItemVal() > typecasted_other_item->weight.getItemVal())
				result = true;
			break;
		case(task3_sort_criteria::bloodtype):
			if (blood_type.getItemVal().string::compare(typecasted_other_item->blood_type.getItemVal()) < 0)//default to ascending order
				result = true;
			break;
		}
		
		// chek if there are more sorting options to apply
		if (sort_criteria != NULL)
		{
			//if sorting is in descending order the result is reversed 
			if (!(sort_criteria->getAscending()))
				result = !result;
		}

		return result;
	}



};
#endif
 
