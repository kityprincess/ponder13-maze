#ifndef SETITERATOR_H
#define SETITERATOR_H

#include <cassert>
/********************************************************************
* Class: SETITERATOR
********************************************************************/
template <class T>
class SetIterator
{
public:

   // default constructor
   SetIterator() : p(NULL) {}

   // initialize to direct p to some item
   SetIterator(T * p) : p(p) {}

   // copy constructor
   SetIterator(const SetIterator & rhs) { *this = rhs; }

   // assignment operator
   SetIterator & operator = (const SetIterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // not equals operator
   bool operator == (const SetIterator & rhs) const
   {
      return rhs.p == this->p;
   }

   // not equals operator
   bool operator != (const SetIterator & rhs) const
   {
      return rhs.p != this->p;
   }

   // dereference operator
   T & operator * ()
   {
      assert(p != NULL);
      if (NULL == p)
         throw "ERROR: attempt to access an empty iterator.";

      return *p;
   }

   // prefix increment
   SetIterator <T> & operator ++ ()
   {
      p++;
      return *this;
   }

   // postfix increment
   SetIterator <T> operator++(int postfix)
   {
      SetIterator tmp(*this);
      p++;
      return tmp;
   }

   // prefix decrement
   SetIterator <T> & operator -- ()
   {
      p--;
      return *this;
   }

   // postfix decrement
   SetIterator <T> operator--(int postfix)
   {
      SetIterator tmp(*this);
      p--;
      return tmp;
   }

private:
    T * p;
};

#endif // !SETITERATOR_H
