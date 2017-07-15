#ifndef SETCONSTITERATOR_H
#define SETCONSTITERATOR_H

/*********************************************************************
* Class: SETCONSTITERATOR
*********************************************************************/
template <class T>
class SetConstIterator
{
public:
   // default constructor
   SetConstIterator() : p(NULL) {}

   // initialize to direct p to some item
   SetConstIterator(T * p) : p(p) {}

   // copy constructor
   SetConstIterator(const SetConstIterator & rhs) { *this = rhs; }

   // assignment operator
   SetConstIterator & operator = (const SetConstIterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // not equals operator
   bool operator != (const SetConstIterator & rhs) const
   {
      return rhs.p != this->p;
   }

   // dereference operator
   T & operator * ()
   {
      return *p;
   }

   // prefix increment
   SetConstIterator <T> & operator ++ ()
   {
      p++;
      return *this;
   }

   // postfix increment
   SetConstIterator <T> operator++(int postfix)
   {
      SetConstIterator tmp(*this);
      p++;
      return tmp;
   }

   // prefix decrement
   SetConstIterator <T> & operator -- ()
   {
      p--;
      return *this;
   }

   // postfix decrement
   SetConstIterator <T> operator--(int postfix)
   {
      SetConstIterator tmp(*this);
      p--;
      return tmp;
   }

private:
   T * p;
};

#endif // !SETCONSTITERATOR_H
