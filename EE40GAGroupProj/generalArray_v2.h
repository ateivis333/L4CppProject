#ifndef __GENERALARRAY_V2_H_
#define __GENERALARRAY_V2_H_


#include "itemArray_v2.h"

// version with inheritance

// Additional to base class supports the following
// Array size expands dinamically; 
// Elements can be shared between multiple arrays without duplications; 
//  removing an elemnt form an array does not delete it if that element is still included by another array
class general_item_array : public item_array {
protected:	
	int min_extension;
	int pct_extension;	
	int getExtenedMaxSize()
	{
		int curr_size = getMaxSize();
		int extension_size = (curr_size*pct_extension) / 100;
		if (extension_size < min_extension)
			extension_size = min_extension;
		return (curr_size + extension_size);
	}
	bool expandArray()
	{ 
		bool result = false;
		if(!memIsAllocated())
			result = allocateArray(min_extension, false);
		else
		{
			int curr_size = getMaxSize();			
			int in_arraysize = getExtenedMaxSize();
			// calloc guarantees the pointer are set to NULL
			basic_item ** thearray_extended = (basic_item **)calloc(in_arraysize, sizeof(basic_item *));
			if (thearray_extended != NULL) 
			{
				// transfer the content			
				for (int i = 0; i < curr_size; i++)
					thearray_extended[i] = thearray[i];					
				// delete old mem
				// if calloc is used to allocate
				free(thearray);				
				// update 
				max_arraysize = in_arraysize;
				thearray = thearray_extended;				
				result = true;
			}
		}
		return result;
	}
	virtual bool checkIndexIsAllowed(int index, bool reading_acess = true)
	{
		if( (!reading_acess) && (index>=getMaxSize()) && (index<getExtenedMaxSize()) )
			return expandArray();
		else
			return item_array::checkIndexIsAllowed(index, reading_acess);
	}
	virtual bool insertElementPtr(int index, basic_item* item_ptr, bool element_to_be_deleted_externally)
	{
		bool result = item_array::insertElementPtr(index, item_ptr, element_to_be_deleted_externally);
		if (result && (item_ptr != NULL))
			item_ptr->incrementincludedBy();
		return result;
	}
	virtual basic_item* allocateSpecificItem()
	{
		basic_item* result = item_array::allocateSpecificItem();
		if (result != NULL)
			result->decrementincludedBy();// this will be incremented at the following insertion
		return result;
	}
	virtual basic_item *getNremoveElementPtr(int index)
	{
		
		basic_item *elem_ptr = item_array::getNremoveElementPtr(index);
		if (elem_ptr != NULL)
			elem_ptr->decrementincludedBy();
		return elem_ptr;
	}
	virtual void deallocateArrayContent()
	{
		if ((memIsAllocated()) && (getMaxSize() > 0))
		{
			int inclusions, totalWhenNontIncludedElsewehere = 0;
			basic_item* curr_elem_ptr;
			// delete items that are ONLY included in this array
			for (int index = 0; index < getMaxSize(); index++)
			{
				// this will reduce the "includedBy" count
				curr_elem_ptr = getNremoveElementPtr(index);
				if (curr_elem_ptr != NULL)
				{
					inclusions = curr_elem_ptr->isIncludedBy();
					if (inclusions <= totalWhenNontIncludedElsewehere)
						deallocateSpecificItem(curr_elem_ptr);
				}
			}
			tot_items = 0;
			resetCurrIndexDefault();
		}
	}


	void merge(int l, int m, int r, basic_sort_criteria* sort_criteria)
	{
		int i, j, k;
		int n1 = m - l + 1;
		int n2 = r - m;

		/* create temp arrays */
		item_array L, R;
		/* Copy data to temp arrays L[] and R[] */
		for (i = 0; i < n1; i++) 
		{
			basic_item* item = this->getNremoveElementPtr(l + i);;
			if (item != NULL)
			{
				L.insertElementPtr(i, item, false);
			}
		}
		for (j = 0; j < n2; j++)
		{
			basic_item* item = this->getNremoveElementPtr(m + 1 + j);
			if (item != NULL)
			{
				L.insertElementPtr(j, item, false);
			}
		}

		/* Merge the temp arrays back into arr[l..r]*/
		i = 0; // Initial index of first subarray 
		j = 0; // Initial index of second subarray 
		k = l; // Initial index of merged subarray 

		while (i < n1 && j < n2)
		{
			if ((L.getElementPtr(i) != NULL) && (R.getElementPtr(i) != NULL))
			{
				if (L.getElementPtr(i))
				{
					this->insertElementPtr(k, L.getElementPtr(i), false);
					i++;
				}
				else
				{
					this->insertElementPtr(k, R.getElementPtr(j), false);
					j++;
				}
				k++;
			}
			
		}

		/* Copy the remaining elements of L[], if there
		   are any */
		while (i < n1)
		{
			this->insertElementPtr(k, L.getElementPtr(i), false);
			i++;
			k++;
		}

		/* Copy the remaining elements of R[], if there
		   are any */
		while (j < n2)
		{
			this->insertElementPtr(k, R.getElementPtr(j), false);
			j++;
			k++;
		}
	}

	void sort_merge(int l, int r, basic_sort_criteria* sort_criteria)
	{
		if (l < r)
		{
			int m = l + (r - 1) / 2;
			sort_merge(l, m, sort_criteria);
			sort_merge(m + 1, r, sort_criteria);
			merge(l, m, r, sort_criteria);
		}
	}



public:	
	general_item_array() { min_extension = 5; pct_extension = 10; }
	~general_item_array() { deallocateArray(); }
	// copy constructor
	void mergesort(basic_sort_criteria* sort_criteria = NULL)
	{
		// To be completed by students;

		// sugestions (see mergesort for integers):
		// Create two "half-size arrays" for the two half/lists.
		// Move the items in the input array to the two "half-size arrays" using the getNremoveElementPtr(...) and appendElementPtr(...) 
		//
		// Sort the two "half-size arrays" recursively;
		//
		// For the two "half-size arrays", write a (privante) subroutine merge_sorted_lists to fill the intial array (now empty but allocated)
		// with the two "half-size arrays" that are now sorted; Use  A->IsLargerThan(B); .
		//
		// close recursion for a list of one element

		// note: sort_criteria_ptr is an optional paramteter (default is null): see bublerot for examples

		sort_merge(1, getMaxSize()-1, sort_criteria);
	}
	bool allocateArrayAndItems(int in_arraysize, bool allocate_each_element) { return allocateArray(in_arraysize, allocate_each_element); }
	// friend functions
	//friend bool fromSourceToDestinationAarray(item_array& sourceArray, int startPosSour, int totElem, item_array& destArray, int startPosDest, bool removeFromSource);
	friend class array_manipulator;
};


class array_manipulator {
private:
	;
	//static incrementincludedBy()
public:
	// transfer (part of) content from source to destination; may leave the originals in place
	static bool fromSourceToDestinationAarray(general_item_array& sourceArray, int startPosSour, int totElem, general_item_array& destArray, int startPosDest, bool removeFromSource)
	{
		bool result = false;
		int souceIndex, destIndex;
		// compatiblity check between prototypes
		basic_item * refSource = sourceArray.getItemPrototype();
		basic_item * refDestination = sourceArray.getItemPrototype();
		if ((refSource != NULL) && (refDestination != NULL) && (startPosSour >= 0) && (totElem > 0) && (startPosDest >= 0))
			if (refSource->compatibilityCheck(refDestination))
				for (int elem = 0; elem < totElem; elem++)
				{
					souceIndex = startPosSour + elem;
					destIndex = startPosDest + elem;
					if (sourceArray.checkIndexIsAllowed(souceIndex) && destArray.checkIndexIsAllowed(destIndex, false))
					{
						basic_item* movingitem;
						if (removeFromSource)
							movingitem = sourceArray.getNremoveElementPtr(souceIndex);
						else
							movingitem = sourceArray.getElementPtr(souceIndex);
						// if the element is "removed" from the source, then it will be deleted by the destination 
						result = destArray.insertElementPtr(destIndex, movingitem, !removeFromSource);
						if (!result && !removeFromSource)
						{
							if (!removeFromSource)
								sourceArray.insertElementPtr(souceIndex, movingitem, !removeFromSource);
							break;
						}
					}
					else
					{
						result = false;
						break;
					}
				}
		return result;
	}
};



#endif