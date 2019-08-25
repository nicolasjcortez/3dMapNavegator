
#ifndef VETOR3D_H
#define VETOR3D_H

#include <iostream>
using namespace std;

#include <iomanip>
using std::setw ;

#include <cmath>
using namespace std;

#include "ponto3d.h"

template <class T>
class Vetor3D : public Ponto3D<T> 
{		
    public:
        Vetor3D();
	Vetor3D( T x, T y, T z );
        Vetor3D( Vetor3D<T> &v );
        Vetor3D<T> operator+( Vetor3D<T> v );
        Vetor3D<T> operator+( T s );
        void operator+=( Vetor3D<T> v );
        void operator+=( T s );
        Vetor3D<T> operator-( Vetor3D<T> v );
        Vetor3D<T> operator-( T s );
        void operator-=( Vetor3D<T> v );
        void operator-=( T s );
        Vetor3D<T> operator*( Vetor3D<T> v );
        Vetor3D<T> operator*( T s );
        void operator*=( Vetor3D<T> v );
        void operator*=( T s );
        Vetor3D<T> operator/( Vetor3D<T> v );
        Vetor3D<T> operator/( T s );
        void operator/=( Vetor3D<T> v );
        void operator/=( T s );
        int operator==( Vetor3D<T> v );
        int operator!=( Vetor3D<T> v );
        Vetor3D<T> operator-( );
        friend ostream &operator<<( ostream &output,  Vetor3D<T> &v )
        {
             output<<"(" << v.Ponto3D<T>::getX() << "," << v.Ponto3D<T>::getY() << "," << v.Ponto3D<T>::getZ() << ")";
             return output;
        }
        friend istream &operator>>( istream &input,  Vetor3D<T> &v )
        {
            T x, y, z;
            input >> x;
            input >> y;
            input >> z;           
            v.Ponto3D<T>::setX( x );
            v.Ponto3D<T>::setY( y );
            v.Ponto3D<T>::setZ( z );
            return input;
        }
        T produtoEscalar( Vetor3D<T> v );
        void produtoVet( Vetor3D<T> v );
        double calculaComprimento();
        double comprimentoQuadrado();
        double normalizaVet();
        double angulo( Vetor3D<T> v );
        Vetor3D<T> operator%( Vetor3D<T> v );
        int operator<( Vetor3D<T> v );
        int operator>( Vetor3D<T> v );
    private:
        
};  



template <class T>
Vetor3D<T>::Vetor3D()
{
    Ponto3D<T>::setX( 0 );
	Ponto3D<T>::setY( 0 );
	Ponto3D<T>::setZ( 0 );
}



template <class T>
Vetor3D<T>::Vetor3D( T x, T y, T z) : Ponto3D<T>( x, y, z )
{
}



template <class T>
Vetor3D<T>::Vetor3D( Vetor3D<T> &v)
{
   	Ponto3D<T>::setX( v.Ponto3D<T>::getX() );
	Ponto3D<T>::setY( v.Ponto3D<T>::getY() ); 
	Ponto3D<T>::setZ( v.Ponto3D<T>::getZ() ); 
}



template <class T>
Vetor3D<T> Vetor3D<T>::operator+( Vetor3D<T> v )
{
	Vetor3D<T> aux;
	aux.Ponto3D<T>::setX( Ponto3D<T>::getX() + v.Ponto3D<T>::getX() );
	aux.Ponto3D<T>::setY( Ponto3D<T>::getY() + v.Ponto3D<T>::getY() );
	aux.Ponto3D<T>::setZ( Ponto3D<T>::getZ() + v.Ponto3D<T>::getZ() );		
	return aux;
}



template <class T>
Vetor3D<T> Vetor3D<T>::operator+( T s )
{
	Vetor3D<T> aux;
	aux.Ponto3D<T>::setX( Ponto3D<T>::getX() + s );
	aux.Ponto3D<T>::setY( Ponto3D<T>::getY() + s );
	aux.Ponto3D<T>::setZ( Ponto3D<T>::getZ() + s );		
	return aux;
}




template <class T>
void Vetor3D<T>::operator+=( Vetor3D<T> v )
{
	Ponto3D<T>::setX( Ponto3D<T>::getX() + v.Ponto3D<T>::getX() );
	Ponto3D<T>::setY( Ponto3D<T>::getY() + v.Ponto3D<T>::getY() );
	Ponto3D<T>::setZ( Ponto3D<T>::getZ() + v.Ponto3D<T>::getZ() );		
}

template <class T>
void Vetor3D<T>::operator+=( T s )
{
	Ponto3D<T>::setX( Ponto3D<T>::getX() + s );
	Ponto3D<T>::setY( Ponto3D<T>::getY() + s );
	Ponto3D<T>::setZ( Ponto3D<T>::getZ() + s );		
}


template <class T>
Vetor3D<T> Vetor3D<T>::operator-( Vetor3D<T> v )
{
	Vetor3D<T> aux;
	aux.Ponto3D<T>::setX( Ponto3D<T>::getX() - v.Ponto3D<T>::getX() );
	aux.Ponto3D<T>::setY( Ponto3D<T>::getY() - v.Ponto3D<T>::getY() );
	aux.Ponto3D<T>::setZ( Ponto3D<T>::getZ() - v.Ponto3D<T>::getZ() );		
	return aux;
}


template <class T>
Vetor3D<T> Vetor3D<T>::operator-( T s )
{
	Vetor3D<T> aux;
	aux.Ponto3D<T>::setX( Ponto3D<T>::getX() - s );
	aux.Ponto3D<T>::setY( Ponto3D<T>::getY() - s );
	aux.Ponto3D<T>::setZ( Ponto3D<T>::getZ() - s );		
	return aux;
}


template <class T>
void Vetor3D<T>::operator-=( T s )
{
	Ponto3D<T>::setX( Ponto3D<T>::getX() - s );
	Ponto3D<T>::setY( Ponto3D<T>::getY() - s );
	Ponto3D<T>::setZ( Ponto3D<T>::getZ() - s );		
}


template <class T>
void Vetor3D<T>::operator-=( Vetor3D<T> v )
{
	Ponto3D<T>::setX( Ponto3D<T>::getX() - v.Ponto3D<T>::getX() );
	Ponto3D<T>::setY( Ponto3D<T>::getY() - v.Ponto3D<T>::getY() );
	Ponto3D<T>::setZ( Ponto3D<T>::getZ() - v.Ponto3D<T>::getZ() );		
}

template <class T>
Vetor3D<T> Vetor3D<T>::operator*( Vetor3D<T> v )
{
	Vetor3D<T> aux;
	aux.Ponto3D<T>::setX( Ponto3D<T>::getX() * v.Ponto3D<T>::getX() );
	aux.Ponto3D<T>::setY( Ponto3D<T>::getY() * v.Ponto3D<T>::getY() );
	aux.Ponto3D<T>::setZ( Ponto3D<T>::getZ() * v.Ponto3D<T>::getZ() );	
	return aux;
}



template <class T>
Vetor3D<T> Vetor3D<T>::operator*( T s )
{
	Vetor3D<T> aux;
	aux.Ponto3D<T>::setX( Ponto3D<T>::getX() * s );
	aux.Ponto3D<T>::setY( Ponto3D<T>::getY() * s );
	aux.Ponto3D<T>::setZ( Ponto3D<T>::getZ() * s );	
	return aux;
}


template <class T>
void Vetor3D<T>::operator*=( Vetor3D<T> v )
{
	Ponto3D<T>::setX( Ponto3D<T>::getX() * v.Ponto3D<T>::getX() );
	Ponto3D<T>::setY( Ponto3D<T>::getY() * v.Ponto3D<T>::getY() );
	Ponto3D<T>::setZ( Ponto3D<T>::getZ() * v.Ponto3D<T>::getZ() );		
}


template <class T>
void Vetor3D<T>::operator*=( T s )
{
	Ponto3D<T>::setX( Ponto3D<T>::getX() * s );
	Ponto3D<T>::setY( Ponto3D<T>::getY() * s );
	Ponto3D<T>::setZ( Ponto3D<T>::getZ() * s );		
}

template <class T>
Vetor3D<T> Vetor3D<T>::operator/( Vetor3D<T> v )
{
	Vetor3D<T> aux;
	aux.Ponto3D<T>::setX( Ponto3D<T>::getX() / v.Ponto3D<T>::getX() );
	aux.Ponto3D<T>::setY( Ponto3D<T>::getY() / v.Ponto3D<T>::getY() );
	aux.Ponto3D<T>::setZ( Ponto3D<T>::getZ() / v.Ponto3D<T>::getZ() );	
	return aux;
}

template <class T>
Vetor3D<T> Vetor3D<T>::operator/( T s )
{
	Vetor3D<T> aux;
	aux.Ponto3D<T>::setX( Ponto3D<T>::getX() / s );
	aux.Ponto3D<T>::setY( Ponto3D<T>::getY() / s );
	aux.Ponto3D<T>::setZ( Ponto3D<T>::getZ() / s );	
	return aux;
}

template <class T>
void Vetor3D<T>::operator/=( Vetor3D<T> v )
{
	Ponto3D<T>::setX( Ponto3D<T>::getX() / v.Ponto3D<T>::getX() );
	Ponto3D<T>::setY( Ponto3D<T>::getY() / v.Ponto3D<T>::getY() );
	Ponto3D<T>::setZ( Ponto3D<T>::getZ() / v.Ponto3D<T>::getZ() );		
}

template <class T>
void Vetor3D<T>::operator/=( T s )
{
	Ponto3D<T>::setX( Ponto3D<T>::getX() / s );
	Ponto3D<T>::setY( Ponto3D<T>::getY() / s );
	Ponto3D<T>::setZ( Ponto3D<T>::getZ() / s );		
}

template <class T>
int Vetor3D<T>::operator==( Vetor3D<T> v )
{
	if( (Ponto3D<T>::getX() == v.Ponto3D<T>::getX()) && (Ponto3D<T>::getY() == v.Ponto3D<T>::getY()) &&  (Ponto3D<T>::getZ() == v.Ponto3D<T>::getZ()) )
    {
        return 1;
    }
	return 0;
}

template <class T>
int Vetor3D<T>::operator!=( Vetor3D<T> v )
{
	if( (Ponto3D<T>::getX() != v.Ponto3D<T>::getX()) || (Ponto3D<T>::getY() != v.Ponto3D<T>::getY()) ||  (Ponto3D<T>::getZ() != v.Ponto3D<T>::getZ()) )
    {
        return 1;
    }
	return 0;
}

template <class T>
Vetor3D<T> Vetor3D<T>::operator-(  )
{
	Vetor3D<T> aux;
	aux.Ponto3D<T>::setX( -Ponto3D<T>::getX() );
	aux.Ponto3D<T>::setY( -Ponto3D<T>::getY() );
	aux.Ponto3D<T>::setZ( -Ponto3D<T>::getZ() );	
	return aux;
}

//template <class T>
//ostream &Vetor3D<T>::operator<<( ostream &output, Vetor3D<T> &v)
//{
//    output<<"(" << v.Ponto3D<T>::getX() << "," << v.Ponto3D<T>::getY() << "," << v.Ponto3D<T>::getZ() << ")" << endl;
//    return output;
//}

//template <class T>
//istream &Vetor3D<T>::operator>>( ostream &input, Vetor3D<T> &v)
//{
//    T x, y, z;
//    input >> x;
//    input  >> y;
//    input >>  z;
//    v.Ponto3D<T>::setX( x );
//    v.Ponto3D<T>::setX( y );
//    v.Ponto3D<T>::setX( z );
//    return input;
//}

template <class T>
T  Vetor3D<T>::produtoEscalar( Vetor3D<T> v )
{
    T p;    
    p = v.Ponto3D<T>::getX() * Ponto3D<T>::getX() + v.Ponto3D<T>::getY() * Ponto3D<T>::getY() + v.Ponto3D<T>::getZ() * Ponto3D<T>::getZ();
    return p;
}

template <class T>
void  Vetor3D<T>::produtoVet( Vetor3D<T> v )
{
    T x, y, z;
    x = Ponto3D<T>::getY() * v.Ponto3D<T>::getZ() - Ponto3D<T>::getZ() * v.Ponto3D<T>::getY();
    y = Ponto3D<T>::getZ() * v.Ponto3D<T>::getX() - Ponto3D<T>::getX() * v.Ponto3D<T>::getZ();
    z = Ponto3D<T>::getX() * v.Ponto3D<T>::getY() - Ponto3D<T>::getY() * v.Ponto3D<T>::getX();
    Ponto3D<T>::setX( x );
	Ponto3D<T>::setY( y );
	Ponto3D<T>::setZ( z );	
}



template <class T>
double Vetor3D<T>::calculaComprimento()
{
    double comp;
    comp=sqrt( Ponto3D<T>::getX() * Ponto3D<T>::getX() + Ponto3D<T>::getY() * Ponto3D<T>::getY() + Ponto3D<T>::getZ() * Ponto3D<T>::getZ());
    return comp;
}



template <class T>
double Vetor3D<T>::comprimentoQuadrado()
{
    double res;
    res=calculaComprimento()*calculaComprimento();
    return res;
}

template <class T>
double Vetor3D<T>::normalizaVet()
{
    double norma;
    norma=calculaComprimento();
    Ponto3D<T>::setX( Ponto3D<T>::getX() / norma );
	Ponto3D<T>::setY( Ponto3D<T>::getY() / norma );
	Ponto3D<T>::setZ( Ponto3D<T>::getZ() / norma );	
    return norma;
}

template <class T>
double  Vetor3D<T>::angulo( Vetor3D<T> v )
{
    double ang;
    ang=acos( produtoEscalar(v) / ( calculaComprimento() * v.calculaComprimento() ) ) * 180 / 3.14159265;
	return ang;
}


template <class T>
Vetor3D<T> Vetor3D<T>::operator%( Vetor3D<T> v )
{
    T x, y, z;
    Vetor3D<T> aux;
    x = Ponto3D<T>::getY() * v.Ponto3D<T>::getZ() - Ponto3D<T>::getZ() * v.Ponto3D<T>::getY();
    y = Ponto3D<T>::getZ() * v.Ponto3D<T>::getX() - Ponto3D<T>::getX() * v.Ponto3D<T>::getZ();
    z = Ponto3D<T>::getX() * v.Ponto3D<T>::getY() - Ponto3D<T>::getY() * v.Ponto3D<T>::getX();
    aux.Ponto3D<T>::setX( x );
	aux.Ponto3D<T>::setY( y );
	aux.Ponto3D<T>::setZ( z );
    return aux;	
}



template <class T>
int Vetor3D<T>::operator<( Vetor3D<T> v )
{
	if( (Ponto3D<T>::getX() < v.Ponto3D<T>::getX()) && (Ponto3D<T>::getY() < v.Ponto3D<T>::getY()) &&  (Ponto3D<T>::getZ() < v.Ponto3D<T>::getZ()) )
    {
        return 1;
    }
	return 0;
}



template <class T>
int Vetor3D<T>::operator>( Vetor3D<T> v )
{
	if( (Ponto3D<T>::getX() > v.Ponto3D<T>::getX()) && (Ponto3D<T>::getY() > v.Ponto3D<T>::getY()) &&  (Ponto3D<T>::getZ() > v.Ponto3D<T>::getZ()) )
    {
        return 1;
    }
	return 0;
}



#endif

