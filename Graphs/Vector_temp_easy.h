/** 
* \class Vector
* \author Manuel Noll
* 
*  Copyright(c) 20010 Manuel Noll - All rights reserved
*  \version 1.0
*  \date    2010
*/


#ifndef VECTOR_H
#define VECTOR_H

#include <vector>

namespace neuralgas {

template<typename T> class Vector : public std::vector<T>
{
  
 public: 
         inline Vector(){this->resize(1);}
         inline Vector(const int& dim){this->resize(dim);}
         inline Vector(const int& dim,const T& value){this->resize(dim,value);}
         //operator+= based on the operations = and +
         inline Vector<T>& operator+=(const Vector<T>&);
         //operator-= based on the operations = and -
         inline Vector<T>& operator-=(const Vector<T>&);
         //operator*= for scalars based on the operations = and *
         template<typename S> inline Vector<T>& operator*=(const S&);
         //operator/= for scalars based on the operations = and /
         template<typename S> inline Vector<T>& operator/=(const S&);
         // operator+ adding two vectors and returning a new result vector         
         inline Vector<T> operator+(const Vector<T>&);
         // const operator+ adding two const vectors and returning a new result vector         
         inline Vector<T> operator+(const Vector<T>&) const;
         // operator- subtracting two const vectors and returning a new result vector
         inline Vector<T> operator-(const Vector<T>&);
         // operator- subtracting two const vectors and returning a new result vector
         inline Vector<T> operator-(const Vector<T>&) const;
         // operator* multiplies a vector by an arbitrary scalar from the right 
         template<typename S> inline Vector<T> operator*(const S&);
         // operator* const multiplies a vector by an arbitrary scalar from the right 
         template<typename S> inline Vector<T> operator*(const S&) const;
         // operator/ divides a vector by an arbitrary scalar from the right        
         template<typename S> inline Vector<T> operator/(const S&);
         // operator/ const divides a vector by an arbitrary scalar from the right 
         template<typename S> inline Vector<T> operator/(const S&) const;
      
         /** \brief friend operator* permitting a multiplication from the left with an arbitrary type
         *
         * Operators where the left element is not a class element have to be global or friend operators.
         * This friend operator* permits a multiplication from the left with an arbitray type 
         * where the scalar is internaly casted to the type of the vector.
         * \param factor is the scalar by which the vector is multiplied
         * \param v is the vector that is multiplied by the scalar 
         */   
         template<typename S> friend inline Vector<T> operator*(const S& factor,const Vector<T>& v)
         {
          return v*factor;
         }
        
 private:
         // dummy variable for operator[] that is returned if given index is out of range
         T _dummy;         
};

/** operator+= based on the operations = and +
*
* \param to_add_assign is what has to be added
*/
template<typename T> inline Vector<T>& Vector<T>::operator+=(const Vector<T>& to_add_assign)
{
  (*this) = (*this) + to_add_assign;
  return *this;
}  

/** operator-= based on the operations = and -
*
* \param to_subtract_assign is what has to be subtracted 
*/
template<typename T> inline Vector<T>& Vector<T>::operator-=(const Vector<T>& to_subtract_assign)
{
  (*this) = (*this) - to_subtract_assign;
  return *this;
}  

/** operator*= for scalars based on the operations = and *
*
* \param to_mul_assign is the factor by which the vector has to be multiplied
*/
template<typename T> template<typename S> inline Vector<T>& Vector<T>::operator*=(const S& to_mul_assign)
{
  (*this) = (*this) * to_mul_assign;
  return *this;
}  

/** operator/= for scalars based on the operations = and /
*
* \param to_div_assign is the factor by which the vector has to be divided
*/
template<typename T> template<typename S> inline Vector<T>& Vector<T>::operator/=(const S& to_div_assign)
{
  (*this) = (*this) / to_div_assign;
  return *this;
}  

/** \brief const operator+ adding two const vectors and returning a new result vector
*
*   The const operator+ adds two const vectors of an arbitray type, creates a result vector
*   and returns that sum vector.
*   \param to_add is the vector that is added to lhs element
*/
template<typename T> inline Vector<T> Vector<T>::operator+(const Vector<T>& to_add) const
{
 int tsize = this->size();  
 Vector<T>* result   =new Vector<T>(tsize);

 if ( tsize == to_add.size() )
 {
        if (tsize > 1 )
          for (int i=0, j=0; i < (tsize / 2) ; i++,j+=2)
          {
             (*result)[j]   =  to_add[j] + (*this)[j];
             (*result)[j+1]   =  to_add[j+1] + (*this)[j+1];
          }
       if (tsize%2==1)
           (*result)[tsize-1] = (*this)[tsize-1]+to_add[tsize-1];       
 }  
 return (*result);
} 

/** \brief operator+ adding two vectors and returning a new result vector
*
*   The const operator+ adds two vectors of an arbitray type, creates a result vector
*   and returns that sum vector.
*   \param to_add is the vector that is added to lhs element
*/
template<typename T> inline Vector<T> Vector<T>::operator+(const Vector<T>& to_add)
{
 int tsize = this->size();  
 Vector<T>* result   =new Vector<T>(tsize);

 if ( tsize == to_add.size() )
 {
        if (tsize > 1 )
          for (int i=0, j=0; i < (tsize / 2) ; i++,j+=2)
          {
             (*result)[j]   =  to_add[j] + (*this)[j];
             (*result)[j+1]   =  to_add[j+1] + (*this)[j+1];
          }
       if (tsize%2==1)
           (*result)[tsize-1] = (*this)[tsize-1]+to_add[tsize-1];       
 }  
 return (*result);
}

/** \brief const operator- subtracting two const vectors and returning a new result vector
*
*   The const operator- subtracts two const vectors of an arbitray type, creates a result vector
*   and returns that difference vector.
*   \param to_subtract is the vector that is subtracted from the lhs element
*/
template<typename T> inline Vector<T> Vector<T>::operator-(const Vector<T>& to_subtract) const
{ 
 int tsize = this->size();  
 Vector<T>* result   =new Vector<T>(tsize);

 if ( tsize == to_subtract.size() )
 {
        if (tsize > 1 )
          for (int i=0, j=0; i < (tsize / 2) ; i++,j+=2)
          {
             (*result)[j]     =  (*this)[j] - to_subtract[j];
             (*result)[j+1]   =   (*this)[j+1] - to_subtract[j+1];
          }
       if (tsize%2==1)
           (*result)[tsize-1] = (*this)[tsize-1]-to_subtract[tsize-1];       
 }  
 return (*result);
}  

/** \brief operator- subtracting two vectors and returning a new result vector
*
*   The operator- subtracts two vectors of an arbitray type, creates a result vector
*   and returns that difference vector.
*   \param to_subtract is the vector that is subtracted from the lhs element
*/
template<typename T> inline Vector<T> Vector<T>::operator-(const Vector<T>& to_subtract)
{ 
 
 int tsize = this->size();  
 Vector<T>* result   =new Vector<T>(tsize);

 if ( tsize == to_subtract.size() )
 {
        if (tsize > 1 )
          for (int i=0, j=0; i < (tsize / 2) ; i++,j+=2)
          {
             (*result)[j]     =  (*this)[j] - to_subtract[j];
             (*result)[j+1]   =   (*this)[j+1] - to_subtract[j+1];
          }
       if (tsize%2==1)
           (*result)[tsize-1] = (*this)[tsize-1]-to_subtract[tsize-1];       
 }  
 return (*result);
}  

/** \brief operator* multiplies a vector by an arbitrary scalar from the right 
*
* The operator* multiplies a vector by an arbitrary scalar from the right 
* and is therefore the complement of the friend operator*.
* If the scalar is of a different than the vector then the type of the scalar is internaly
* casted to the type of the vector.
* The result is the lhs vector.
* \param factor is the scalar by which the vector is multiplied
*/
template<typename T> template<typename S> inline Vector<T> Vector<T>::operator*(const S& factor)
{   
 int tsize = this->size();  
 const T    scalar   =   (T)factor;
 Vector<T>* result   =new Vector<T>(tsize);

 if (tsize > 1 )
  for (int i=0, j=0; i < (tsize / 2) ; i++,j+=2)
  {
     (*result)[j]     =  (*this)[j] * scalar;
     (*result)[j+1]   =   (*this)[j+1] * scalar;
  }
 if (tsize%2==1)
   (*result)[tsize-1] = (*this)[tsize-1] * scalar;       
 return (*result);

}

/** \brief operator* const multiplies a vector by an arbitrary scalar from the right 
*
* The operator* multiplies a vector by an arbitrary scalar from the right 
* and is therefore the complement of the friend operator*.
* If the scalar is of a different than the vector then the type of the scalar is internaly
* casted to the type of the vector.
* The result is the lhs vector.
* \param factor is the scalar by which the vector is multiplied
*/
template<typename T> template<typename S> inline Vector<T> Vector<T>::operator*(const S& factor) const
{   
 int tsize = this->size();  
 const T    scalar   =   (T)factor;
 Vector<T>* result   =new Vector<T>(tsize);

 
 if (tsize > 1 )
  for (int i=0, j=0; i < (tsize / 2) ; i++,j+=2)
  {
     (*result)[j]     =  (*this)[j] * scalar;
     (*result)[j+1]   =   (*this)[j+1] * scalar;
  }
 if (tsize%2==1)
   (*result)[tsize-1] = (*this)[tsize-1] * scalar;       
   
 return (*result);

}

/** \brief operator/ divides a vector by an arbitrary scalar from the right 
*
* The operator/ divides a vector by an arbitrary scalar from the right 
* and is therefore the complement of the friend operator*.
* If the scalar is of a different than the vector then the type of the scalar is internaly
* casted to the type of the vector.
* The result is the lhs vector.
* \param factor is the scalar by which the vector is divided
*/
template<typename T> template<typename S> inline Vector<T> Vector<T>::operator/(const S& factor) 
{   

 int tsize = this->size();  
 const T    scalar   =   (T)factor;
 Vector<T>* result   =new Vector<T>(tsize);

 if (tsize > 1 )
  for (int i=0, j=0; i < (tsize / 2) ; i++,j+=2)
  {
     (*result)[j]     =  (*this)[j] / scalar;
     (*result)[j+1]   =   (*this)[j+1] / scalar;
  }
 if (tsize%2==1)
   (*result)[tsize-1] = (*this)[tsize-1] / scalar;       

 return (*result);

}


/** \brief operator/ const divides a vector by an arbitrary scalar from the right 
*
* The operator* divides a vector by an arbitrary scalar from the right 
* and is therefore the complement of the friend operator/.
* If the scalar is of a different than the vector then the type of the scalar is internaly
* casted to the type of the vector.
* The result is the lhs vector.
* \param factor is the scalar by which the vector is divided
*/
template<typename T> template<typename S> inline Vector<T> Vector<T>::operator/(const S& factor) const
{   
 int tsize = this->size();  
 const T    scalar   =   (T)factor;
 Vector<T>* result   =new Vector<T>(tsize);

 if (tsize > 1 )
  for (int i=0, j=0; i < (tsize / 2) ; i++,j+=2)
  {
     (*result)[j]     =  (*this)[j] / scalar;
     (*result)[j+1]   =   (*this)[j+1] / scalar;
  }
 if (tsize%2==1)
   (*result)[tsize-1] = (*this)[tsize-1] / scalar;       

 return (*result);

}

} // namespace neuralgas

#endif
