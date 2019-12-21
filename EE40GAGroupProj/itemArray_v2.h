#ifndef __ITEMARRAY_V2_H_
#define __ITEMARRAY_V2_H_

#include "arrayItem_v2.h"

class item_array{
protected:	
	// this is the prottipical elemnt
	basic_item *itemPrototype;	
	// this is an array of pointers.
	// each pints to a generic element
	basic_item **thearray;		
	// number of array entries allocated
	int max_arraysize;
	bool memallocated;
	// number of array entries used
	int tot_items;	
	// index of current item (may be useful)
	int current_index;
	// functions that are needed by each derived class to do sorting (this is useful for bublbesort
	bool swapElements(int elem1_index, int elem2_index)
	{
		// check that the memory is allocated and that the element indexes are within array boundary
		if(memIsAllocated() && checkIndexIsAllowed(elem1_index) && checkIndexIsAllowed(elem2_index) )
		{
			basic_item* temp_swap;
			temp_swap=thearray[elem1_index];						
			thearray[elem1_index]=thearray[elem2_index];
			thearray[elem2_index]=temp_swap;
			return true;
		}
		return false;
	}
	// allocate item on heap for the specific type (calling the porper constructor)
	virtual basic_item* allocateSpecificItem()
	{
		basic_item* result = NULL;
		if (itemPrototype != NULL)
			result = itemPrototype->allocateEmptyItem();		
		return result;
	}
	// remove item from heap for the specific type  (calling the porper distructor)
	void deallocateSpecificItem(basic_item* item)
	{
		if(item!=NULL && itemPrototype != NULL)
			itemPrototype->deallocateItem(item);
	}
	virtual void deallocateArrayContent()
	{
		if( (memIsAllocated()) && (getMaxSize()>0) )
		{
			basic_item* curr_elem_ptr;
			// delete all items that have been allocated
			for(int index=0; index<getMaxSize();index++)
			{
				curr_elem_ptr=getNremoveElementPtr(index);
				// call a function for the specific item
				if (curr_elem_ptr != NULL)
					deallocateSpecificItem(curr_elem_ptr);
			}
			tot_items=0;
			resetCurrIndexDefault();
		}
	}
	void deallocateArray()
	{
		deallocateArrayContent();
		if( (memIsAllocated()) && (getMaxSize()>0) )
		{
			// now deallocate the memory for the array itself
			free(thearray);
			thearray=NULL;			
			memallocated=false;	
			max_arraysize = 0;
		}
	}
	// reset the current index to the default value (-1)
	void resetCurrIndexDefault(){current_index=-1;}
	// protected but requried to be accessible to friend functions:
	// "access" the item prototype (can be used by anotehr array)
	basic_item* getItemPrototype()
	{
		if (itemPrototype == NULL)
			cout << endl << "Warning: itemPrototype is undefined" << endl;
		return itemPrototype;
	}
	bool resetCurrIndexToZero()
	{
		if (checkIndexIsAllowed(0))
		{
			current_index = 0;
			return true;
		}
		return false;
	}
	// to remove (pop) elemnts (useful with mergesort)	
	basic_item * getCurrElementPtr()
	{
		basic_item* result = getElementPtr(current_index);
		return result;
	}
	
	// removes the element from the array:
	virtual basic_item *getNremoveElementPtr(int index)
	{
		// copy the pointer (NULL if index is not allowed)
		basic_item *elem_ptr = getElementPtr(index);
		if (elem_ptr != NULL)
		{
			// clear the pointer in the array, but the moemory it points to is still valid (and the pointer is returned)
			thearray[index] = NULL;
			tot_items--;
			if (tot_items < 0)
				tot_items = 0;
		}
		return elem_ptr;
	}
	// insert an element to the array (if requred position is available)

	bool appendElementPtr(basic_item* item_ptr, bool element_to_be_deleted_externally)
	{
		bool success = insertElementPtr((current_index + 1), item_ptr, element_to_be_deleted_externally);		
		return success;
	}
	basic_item * getNremoveCurrElementPtr(bool advance_index = true)
	{
		basic_item* result = getNremoveElementPtr(current_index);
		if (advance_index)
			current_index++;
		else
			current_index--;
		tot_items--;
		return result;
	}
	//
	bool allocateArray(int in_arraysize, bool allocate_each_element)
	{
		if ((!memIsAllocated()) && (in_arraysize > 0))
		{
			//thearray=new basic_item*[in_arraysize];

			// calloc guarantees the pointer are set to NULL
			thearray = (basic_item **)calloc(in_arraysize, sizeof(basic_item *));
			
			if (thearray != NULL)
			{
				max_arraysize = in_arraysize;
				memallocated = true;

				// does each element need to be allocated?
				if (allocate_each_element)
				{
					// now associate each pointer to an  "empty" items (each needs to be allocated)
					for (int index = 0; index < getMaxSize(); index++)
					{
						basic_item* emptyitem = allocateSpecificItem();
						if (emptyitem == NULL)
						{
							cout << "Error in allocateArray(): out of memory for item " << index << endl;
							return false;
						}
						// append element (allocated here: it will be deleted by the destructor)
						appendElementPtr(emptyitem, false);
					}
				}
				resetCurrIndexToZero();
				return true;
			}
			else
				return false;
		}
		return false;
	}
	//
	virtual void setArrayItemLocked(int position, bool lockedstatus)
	{
		basic_item* elem_ptr = getElementPtr(position);
		if (elem_ptr != NULL)
		{
			elem_ptr->setLocked(lockedstatus);
		}		
	}
	int getCurrIndex() { return current_index; }
	bool printItemOnScreen(int position)
	{
		basic_item* elem_ptr = getElementPtr(position);
		if (elem_ptr != NULL)
		{
			elem_ptr->printItemOnScreen();
			return true;
		}
		return false;
	}
	virtual bool checkIndexIsAllowed(int index, bool reading_acess = true)
	{
		if (memIsAllocated() && index >= 0 && index < getMaxSize())
			return true;
		else
			return false;
	}
public: 
	item_array()
	{
		max_arraysize=0; 
		memallocated=false; 
		tot_items=0; 
		thearray=NULL; 
		itemPrototype = NULL; 
		resetCurrIndexDefault();
	}
	// the base-class destructor does nothing: the derived object destructor should call deallocateArrayAndContent()
	~item_array(){deallocateArray();}
	bool memIsAllocated() {return memallocated;}
	int getMaxSize() {return max_arraysize;}
	int getTotItems() {return tot_items;}	
	// attach the item prototype (only once).
	bool attachItemPrototype(basic_item* item_prototypeptr)
	{
		bool success = false;
		if (itemPrototype == NULL)
		{
			if(item_prototypeptr != NULL)
			{
				// skip this extra sefety check: not ready yet.
				// test the item can be typecasted to base type 
				//basic_item* reference_base_item=NULL;
				//basic_item* typecasted_item_ptr = NULL;
				//typecasted_item_ptr = typecastItem(item_prototypeptr, reference_base_item);
				// check that it worked
				//if (typecasted_item_ptr == NULL)
				//	cout << "itemPrototype must be derived from basic_item ." << endl;
				//else
				{itemPrototype= item_prototypeptr; success=true;}
			}
			else
				cout << endl << "itemPrototype item can't be NULL" << endl;
		}
		else
			cout << endl << "itemPrototype already set" << endl;

		return success;
	}
	//
	bool allocateArrayAndItems(int in_arraysize) {return allocateArray(in_arraysize, true);}
	//	
	// to print content to screen with the right format	
	void printArrayOnScreen()
	{
		if( memIsAllocated() && (getTotItems()>0) )
		{
			cout << "*** Array Content ***" << endl;
			// parse all positions
			for(int position=0; position<getMaxSize(); position++)
			{
				
				cout << "Element at position "<< position << " :" << endl;
				// print the item, if any
				if( printItemOnScreen(position) )
					cout << endl;
				else
					cout << "No Element in this position."<< endl;
			}
			cout << "**********" << endl;
		}
		else
			cout << "No Element allocated: skip printing." << endl;
	}
	// some input / output
	void enterArrayFromKeyboard()
	{
		if( memIsAllocated() && (getTotItems()>0) )
		{
			basic_item *curr_item;
			// parse all positions
			for(int position=0; position<getMaxSize(); position++)
			{
				curr_item=getElementPtr(position);
				if(curr_item==NULL)
					cout << "Element at position "<< position << "is not allocated" << endl;
				else
				{
					if (curr_item->isEmpty())
					{   // fill empty item
						cout << "Enter value for element at position " << position << endl;
						curr_item->enterItemFromKeyboard();
					}
					else
					{
						if (!curr_item->isLocked())
						{	// overwrite existing value
							cout << "Overwrtite value for element at position " << position << endl;
							curr_item->enterItemFromKeyboard();
						}
						else
							cout << "Error in enterArrayFromKeyboard(): element at position " << position << " is locked." << endl;
					}	
				}
			}
		}		
	}
	void fillRandomValueArray()
	{
		if( memIsAllocated() && (getTotItems()>0) )
		{
			basic_item *curr_item;
			
			// the following sets the random number generator differently
			// for evey execution, depending on the date/time.	
			srand((unsigned int)time(NULL));
			//


			// parse all positions
			for(int position=0; position<getMaxSize(); position++)
			{
				curr_item=getElementPtr(position);
				if(curr_item==NULL)
					cout << "Error in fillRandomValueArray(): Element at position "<< position << "is not allocated" << endl;
				else
					if(curr_item->isLocked())
						cout << "Error in fillRandomValueArray(): Element at position " << position << "is locked" << endl;
					else
						curr_item->generateRandomItem();
			}
		}		
	}
	void printArrayInfoOnScreen()
	{
		cout << endl << "Array info: " << endl;
		if (itemPrototype != NULL)
			itemPrototype->printItemTypeName();
		else
			cout << "Item type not defined" << endl;
		cout << endl << "Array max size " << getMaxSize() << endl;
		cout << endl << "Array tot items " << getTotItems() << endl;
		cout << endl << endl;
	}
	//
	void setArrayLocked(bool lockedstatus)
	{
		if (memIsAllocated() && (getTotItems() > 0))
		{
			// parse all positions
			for (int position = 0; position < getMaxSize(); position++)
				setArrayItemLocked(position, lockedstatus);			
		}
		else
			cout << "No Element allocated to lock/unlock." << endl;
	}
	//
	void bubblesort(basic_sort_criteria* sort_criteria_ptr=NULL)
	{
		// Parse the array with a double for loop.
		// Select two successive items using the getElementPtr: items A and B
		// Use  A->IsLargerThan(B); if so, use swapElements(...).

		// note: sort_criteria_ptr is an optional paramteter (default is null)
		// when present, this can be used to determine the sorting type:
		// example 1: "ascending" or "discending" for simple items that hold numbers or strings
		// example 2: "sort by name" or "sort by student ID" for complex items that hold both names and numbers 

		
		// To be completed by students:
		// The version below produces the correct result but performs some unnecessary comparisons.
		// Modify it so that it become smore efficient
		
		for(int loop_index=0; loop_index<getMaxSize()-1; loop_index++)
		{
			for(int curr_index=0; curr_index<getMaxSize()-(1+ loop_index); curr_index++)
			{
				bool comparison_result=true;
				basic_item* curr_item=getElementPtr(curr_index);
				basic_item* next_item=getElementPtr(curr_index+1);

				// in case there are "empty (non allocated) items"
				if(curr_item!=NULL)
					comparison_result=curr_item->IsLargerThan(next_item, sort_criteria_ptr);
				
				if(comparison_result)
					swapElements(curr_index, curr_index+1);
			}
		}

	}	


	virtual bool insertElementPtr(int index, basic_item* item_ptr, bool element_to_be_deleted_externally)
	{
		// check that the memory is allocated and that the element index is within array boundary
		if (memIsAllocated() && checkIndexIsAllowed(index))
		{
			// add the item if array element is not occupied already
			if ((thearray[index] == NULL))
			{
				if ((item_ptr != NULL))
				{
					thearray[index] = item_ptr;
					tot_items++;
					current_index = index;
					return true;
				}
				else
				{
					cout << endl << " Attempting to insert a NULL element." << endl;
					return false;
				}
			}
			else
			{
				cout << endl << " Attempting to overwrite element at position " << index << " : not allowed." << endl;
				return false;
			}
		}
		else
			return false;
	}
	// to insert/remove (push/pop) elemnts (useful with mergesort)	

	// access individual elements (via the index)
	basic_item* getElementPtr(int index)
	{
		// check that the memory is allocated and that the element indexes are within array boundary
		if (checkIndexIsAllowed(index))
			return thearray[index];
		else
			return NULL;
	}
};



#endif


