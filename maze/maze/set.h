#ifndef SET_H
#define SET_H

#include <cassert>
#include "setIterator.h"
#include "setConstIterator.h"

template <class T>
class Set
{
public:

   // constructors/destructors
   Set(int in_capacity = 0);
   Set(const Set<T> & source);
   ~Set();
   Set<T> & operator = (const Set<T> & source);

   // basic metadata
   int size() const { return m_size; }
   int capacity() const { return m_capacity; }
   bool empty() const { return m_size <= 0; }

   // data management
   void clear() { m_size = 0; }
   void insert(const T & in_item);
   void erase(SetIterator<T> & in_location);

   // data access
   SetIterator<T> find(T & in_item) const;
   SetIterator<T> begin() const;
   SetIterator<T> end() const;
   SetConstIterator<T> cbegin() const { return SetConstIterator<T>(m_data); }
   SetConstIterator<T> cend() const { return SetConstIterator<T>(m_data + m_size); }

   // operations
   Set<T> operator && (const Set<T> & rhs) const;
   Set<T> operator || (const Set<T> & rhs) const;
   Set<T> operator - (const Set<T> & rhs) const;

private:
   // internal helper functions
   bool findIndex(const T & in_item, int & out_index) const;
   bool isValid() const;
   bool isDataSorted() const;
   void deleteData();
   void copy(const Set<T> & source);
   void copyData(T * in_data);
   void allocate(int in_capacity);
   void resize();
   void addToEnd(const T & in_item);

   // fields
   int m_capacity;
   int m_size;
   T * m_data;
};

/*************************************
* SET :: CONSTRUCTOR
* Creates a new instance of a Set with
* an optional capacity
*************************************/
template <class T>
Set<T> ::Set(int in_capacity)
   : m_capacity(in_capacity), m_size(0), m_data(NULL)
{
   assert(m_capacity >= 0);
   if (m_capacity < 0)
      m_capacity = 0;

   if (m_capacity)
      allocate(m_capacity);

   assert(isValid());
}

/*************************************
* SET :: COPY CONSTRUCTOR
* Creates a new instance of a Set
* by copying an existing set
*************************************/
template<class T>
Set<T>::Set(const Set<T>& source)
   : m_capacity(source.m_capacity), m_size(source.m_size), m_data(NULL)
{
   assert(source.isValid());

   copy(source);

   assert(isValid());
}

/*************************************
* SET :: DESTRUCTOR
* Destroys a set and deallocates its memory
*************************************/
template<class T>
inline Set<T>::~Set()
{
   deleteData();
}

/*************************************
* SET :: ASSIGNMENT
* Copies another set into this one
*************************************/
template<class T>
inline Set<T>& Set<T>::operator=(const Set<T>& source)
{
   if (this == &source) // copying onto ourself, do nothing
      return *this;

   assert(source.isValid());

   deleteData();

   copy(source);

   assert(isValid());

   return *this;
}

/*************************************
* SET :: INSERT
* Inserts an item into the set, if it
* doesn't exist already. Expands the
* capacity of the set if necessary to
* make room.
*************************************/
template <class T>
void Set<T> ::insert(const T & in_item)
{
   // first we see if we already have the item; if so, we can just quit
   int itemIndex = 0;
   if (findIndex(in_item, itemIndex))
      return;

   // check to see if we have capacity, and if not, resize
   if (m_size == m_capacity)
   {
      resize();
   }

   assert(m_size < m_capacity);

   // Now, earlier, we used findIndex to check if the item
   // already exists. If we're here, we know that it doesn't
   // and we also know where it should go, thanks to findIndex

   // First, make room for the new item
   assert(itemIndex >= 0);
   for (int i = m_size; i > itemIndex; i--)
      m_data[i] = m_data[i - 1];

   // And now we insert it
   m_data[itemIndex] = in_item;
   m_size++;

   assert(isValid());
   assert(isDataSorted());
}

/*************************************
* SET :: ERASE
* Removes an item - specified by an
* iterator - from the set.
*************************************/
template<class T>
inline void Set<T>::erase(SetIterator<T>& in_location)
{
   int i = 0;
   if (findIndex(*in_location, i))
   {
      for (; i < m_size - 1; i++)
         m_data[i] = m_data[i + 1];

      m_size--;
   }
   assert(isValid());
}

/*************************************
* SET :: FIND
* If it exists, locates an item in the
* set, returning it as an iterator
*************************************/
template<class T>
inline SetIterator<T> Set<T>::find(T & in_item) const
{
   int index = 0;

   if (findIndex(in_item, index))
      return SetIterator<T>(m_data + index);

   return end();
}

/*************************************
* SET :: BEGIN
* Returns an iterator pointing at the
* start of the set
*************************************/
template <class T>
SetIterator<T> Set<T> ::begin() const
{
   return SetIterator<T>(m_data);
}

/*************************************
* SET :: END
* Returns an iterator pointing at the
* end of the set
*************************************/
template<class T>
inline SetIterator<T> Set<T>::end() const
{
   return SetIterator<T>(m_data + m_size);
}

/*************************************
* SET :: INTERSECTION
* Returns a new set that contains all
* the elements that exist both in this
* set and another (passed as a parameter)
*************************************/
template<class T>
inline Set<T> Set<T>::operator&&(const Set<T>& rhs) const
{
   assert(isValid());
   assert(rhs.isValid());

   Set<T> result;

   // If either side is empty, we can exit now
   if (empty() || rhs.empty())
      return result;

   // Now we do our work. This is essentially a merge join
   int iSet1 = 0;
   int iSet2 = 0;

   while (iSet1 < m_size || iSet2 < rhs.m_size)
   {
      // we check to see if we've run out of either set
      if (iSet1 == m_size)
         return result;

      if (iSet2 == rhs.m_size)
         return result;

      if (m_data[iSet1] == rhs.m_data[iSet2]) // A match
      {
         result.addToEnd(m_data[iSet1]);
         iSet1++;
         iSet2++;
      }
      else if (m_data[iSet1] < rhs.m_data[iSet2])
         iSet1++;
      else
         iSet2++;
   }

   return result;
}

/*************************************
* SET :: UNION
* Creates a new set with all of the
* elements that appear both in this set
* and another (passed as a parameter)
*************************************/
template<class T>
inline Set<T> Set<T>::operator||(const Set<T>& rhs) const
{
   assert(isValid());
   assert(rhs.isValid());

   Set<T> result;

   // first, we deal with some simple cases
   if (empty() && rhs.empty())
      return result;
   else if (empty())
   {
      return rhs;
   }
   else if (rhs.empty())
   {
      return *this;
   }

   // now we do the more challenging case
   int iSet1 = 0;
   int iSet2 = 0;
   while (iSet1 < m_size || iSet2 < rhs.m_size)
   {
      if (iSet1 == m_size) // we've run out of elements in this side;
         result.addToEnd(rhs.m_data[iSet2++]);
      else if (iSet2 == rhs.m_size) // we've run out of elements on the rhs
         result.addToEnd(m_data[iSet1++]);
      else if (m_data[iSet1] == rhs.m_data[iSet2])
      {
         result.addToEnd(m_data[iSet1]);
         iSet1++;
         iSet2++;
      }
      else if (m_data[iSet1] < rhs.m_data[iSet2])
         result.addToEnd(m_data[iSet1++]);
      else
         result.addToEnd(rhs.m_data[iSet2++]);
   }

   return result;
}

/*************************************
* SET :: DIFFERENCE
* Finds the difference between this set
* and another, i.e. those items that
* exist in this set that do not exist
* in the other.
*************************************/
template<class T>
inline Set<T> Set<T>::operator-(const Set<T>& rhs) const
{
   assert(isValid());
   assert(rhs.isValid());

   if (rhs.empty())
      return *this;

   Set<T> result;

   int index = 0;

   for (int i = 0; i < m_size; i++)
   {
      if (!rhs.findIndex(m_data[i], index))
         result.addToEnd(m_data[i]);
   }

   assert(result.isValid());
   assert(result.isDataSorted());

   return result;
}

/*************************************
* SET :: FINDINDEX
* If it exists in the set, returns the
* position of a given item (as an output
* reference) an returns true if the item
* was found.
*************************************/
template<class T>
bool Set<T> ::findIndex(const T & in_item, int & out_index) const
{
   if (empty())
      return false;

   assert(m_data != NULL);
   assert(m_size > 0);

   int begin = 0;
   int end = m_size - 1;

   while (begin <= end)
   {
      out_index = (begin + end) / 2;

      if (in_item == m_data[out_index])
         return true;

      if (in_item < m_data[out_index])
         end = out_index - 1;
      else
         begin = out_index + 1;
   }

   // There's ambiguity that arises; we can hit this point
   // and not have a match, but it's unclear whether our index should
   // be at the current location or right after it, so we check
   if (in_item > m_data[out_index])
      out_index++;

   return false;
}

/*************************************
* SET :: ISVALID
* Checks that key metadata in the set
* is consistent.
*************************************/
template<class T>
inline bool Set<T>::isValid() const
{
   bool isValid = true;

   isValid &= (m_capacity >= 0 && m_size >= 0);
   isValid &= (m_size <= m_capacity);
   isValid &= (m_capacity == 0 || m_data != NULL);
   isValid &= (m_capacity != 0 || m_data == NULL);

   return isValid;
}

/*************************************
* SET :: ISDATASORTED
* Confirms that all elements of the
* set are in the proper sort order
*************************************/
template<class T>
inline bool Set<T>::isDataSorted() const
{
   if (!m_capacity)
      return true;

   for (int i = 0; i < m_size; i++)
   {
      if (0 == i)
         continue;

      if (m_data[i] <= m_data[i - 1])
         return false;
   }

   return true;
}

/*************************************
* SET :: DELETEDATA
* Deallocate the data buffer for the
* set. Note: this does not change the
* set's capacity; that must be updated
* separately.
*************************************/
template<class T>
inline void Set<T>::deleteData()
{
   if (m_data != NULL)
   {
      delete[] m_data;
      m_data = NULL;
   }
}

/*************************************
* SET :: COPY
* Copies another set onto this one.
* Note: this does not deallocate the
* previous data buffer; that must be
* done separately.
*************************************/
template<class T>
inline void Set<T>::copy(const Set<T>& source)
{
   assert(source.isValid());

   m_capacity = source.m_capacity;
   m_size = source.m_size;

   if (m_capacity)
   {
      allocate(m_capacity);

      copyData(source.m_data);
   }

   assert(isValid());
}

/*************************************
* SET :: COPYDATA
* Copies the data from an array into
* this set's data buffer. Note: this
* does not attempt to allocate the
* buffer or ensure it has sufficient
* capacity; that must be done separately
*************************************/
template<class T>
inline void Set<T>::copyData(T * in_data)
{
   if (m_capacity)
   {
      assert(m_data);
      for (int i = 0; i < m_size; i++)
         m_data[i] = in_data[i];
   }
}

/*************************************
* SET :: ALLOCATE
* Creates a new data buffer with a given
* capacity. Note: does not attempt to
* deallocate an existing buffer; that
* must be done separately.
*************************************/
template<class T>
inline void Set<T>::allocate(int in_capacity)
{
   if (in_capacity)
   {
      try
      {
         m_data = new T[in_capacity];
      }
      catch (std::bad_alloc)
      {
         throw "ERROR: Unable to allocate a new buffer for Set.";
      }
   }
}

/*************************************
* SET :: RESIZE
* Doubles the capacity of the data buffer
* and copies over all content from the
* old buffer into the new.
*************************************/
template<class T>
inline void Set<T>::resize()
{
   T * oldData = m_data;

   int capacity = m_capacity == 0 ? 1 : m_capacity * 2;

   allocate(capacity);

   if (NULL != oldData)
   {
      copyData(oldData);
      delete[] oldData;
   }

   m_capacity = capacity;

   assert(isValid());
}

/*************************************
* SET :: ADDTOEND
* Adds an item onto the end of the
* set. Does not check to see that it
* is unique across the entire set.
* Resizes the set if needed.
*************************************/
template<class T>
inline void Set<T>::addToEnd(const T & in_item)
{
   // check to see if we already have the item at the end
   if (!empty() && m_data[m_size - 1] == in_item)
      return;

   // check to see if we need more capacity
   if (m_size == m_capacity)
   {
      resize();
   }

   m_data[m_size] = in_item;
   m_size++;

   assert(isValid());
   assert(isDataSorted());
}

#endif // !SET_H
