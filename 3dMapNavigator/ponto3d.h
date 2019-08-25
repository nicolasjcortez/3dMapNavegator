// ponto.h
#ifndef PONTO3D_H
#define PONTO3D_H
#include <iostream>
using namespace std;

template <class T>
class Ponto3D
{
	public:
		Ponto3D();
		Ponto3D( T x, T y, T z );
		Ponto3D( Ponto3D<T> &p );
		void print();
        virtual T getX();
        virtual T getY();
        virtual T getZ();
        virtual void setX( T x );
        virtual void setY( T y );
        virtual void setZ( T z );
		Ponto3D<T> operator+( Ponto3D<T> p );
        Ponto3D<T> operator-( Ponto3D<T> p );
        Ponto3D<T> operator*( Ponto3D<T> p );
        Ponto3D<T> operator/( Ponto3D<T> p );
        T _x, _y, _z;
        
	private:
		
};



template <class T>
Ponto3D<T>::Ponto3D()
{
	_x = 0;
	_y = 0;
}



template <class T>
Ponto3D<T>::Ponto3D( T x, T y, T z )
{
	_x = x;
	_y = y;
    _z = z;
}

template <class T>
Ponto3D<T>::Ponto3D( Ponto3D<T> &p )
{
   	_x = p._x;
	_y = p._y; 
	_z = p._z;
}
template <class T>
void Ponto3D<T>::print()
{
	cout << "(" << _x << "," << _y << "," << _z << ")" << endl;
}



template <class T>
T Ponto3D<T>::getX()
{
  return _x;
}


template <class T>
T Ponto3D<T>::getY()
{
  return _y;
}


template <class T>
T Ponto3D<T>::getZ()
{
  return _z;
}



template <class T>
void Ponto3D<T>::setX( T x )
{
    _x = x;
}



template <class T>
void Ponto3D<T>::setY( T y )
{
    _y = y;    
}



template <class T>
void Ponto3D<T>::setZ( T z )
{
    _z = z;    
}




template <class T>
Ponto3D<T> Ponto3D<T>::operator+( Ponto3D<T> p ){
	Ponto3D<T> aux;
	aux._x = _x + p._x;
	aux._y = _y + p._y;
	aux._z = _z + p._z;
	
	return aux;
}



template <class T>
Ponto3D<T> Ponto3D<T>::operator-( Ponto3D<T> p ){
	Ponto3D<T> aux;
	aux._x = _x - p._x;
	aux._y = _y - p._y;
	aux._z = _z - p._z;
	
	return aux;
}



template <class T>
Ponto3D<T> Ponto3D<T>::operator*( Ponto3D<T> p ){
	Ponto3D<T> aux;
	aux._x = _x * p._x;
	aux._y = _y * p._y;
	aux._z = _z * p._z;
	
	return aux;
}



template <class T>
Ponto3D<T> Ponto3D<T>::operator/( Ponto3D<T> p ){
	Ponto3D<T> aux;
	aux._x = _x / p._x;
	aux._y = _y / p._y;
	aux._z = _z / p._z;
	
	return aux;
}

#endif
