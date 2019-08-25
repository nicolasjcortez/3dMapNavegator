/* 
 * File:   Navigator.cpp
 * Author: mgattass
 * 
 * Created on September 13, 2014, 5:30 PM
 */

#include <cmath>

#include "Navigator.h"
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif



Navigator::Navigator( double px, double py, double pz )
{
    _positionX = px;
    _positionZ = py;
    _positionY = pz;
    _targetX = _targetY = _targetZ = 0.0;
    _directionX = _directionY = _directionZ = 1.0;
    _velocity = 1;
    _velocityRotation = 0.1;
    _rotateX = _rotateY = 0.0;
}



void Navigator::computeCamera( const bool keys[], double& px, double& py, double& pz, double& tx, double& ty, double& tz )
{
    double localVelocity;

    //Atualiza a rotacao.
    if (keys['4'])
    {
        _rotateX -= _velocityRotation;
    }

    if (keys['6'])
    {
        _rotateX += _velocityRotation;
    }

    if (keys['8'])
    {
        _rotateY -= _velocityRotation;
    }

    if (keys['2'])
    {
        _rotateY += _velocityRotation;
    }

    //Define a velocidade com sinal.
    if (keys['w'])
    {
        localVelocity = _velocity;
    }
    else if (keys['s'])
    {
        localVelocity = -_velocity;
    }
    else
    {
        localVelocity = 0.0;
    }

    //Limita os angulos de rotacao.
    if (_rotateY >= 90.0)
    {
        _rotateY = 89.0;
    }

    if (_rotateY <=-90.0)
    {
        _rotateY = -89.0;
    }

    //Calcula a direcao que se deve andar.
    _directionX = sin( M_PI * _rotateX / 180.0 ) * cos( M_PI * _rotateY / 180.0 );
    _directionZ = sin( M_PI * _rotateY / 180.0 );
    _directionY = cos( M_PI * _rotateX / 180.0 ) * cos( M_PI * _rotateY / 180.0 );

    //Calcula o ponto para onde a camera aponta.
    _targetX = _directionX * 3 + _positionX;
    _targetY = _directionY * 3 + _positionY;
    _targetZ = _directionZ * 3 + _positionZ;

    //Calcula a posicao da camera.
    _positionX += _directionX * localVelocity;
    _positionY += _directionY * localVelocity;
    _positionZ += _directionZ * localVelocity;

    //Salva as configuracoes da camera nas variaveis da funcao.
    px = _positionX;
    py = _positionY;
    pz = _positionZ;
    tx = _targetX;
    ty = _targetY;
    tz = _targetZ;
}



void Navigator::setDeltaRotation( double dx, double dy )
{
    _rotateX += dx;
    _rotateY += dy;
}



void Navigator::setVelocity( double velocity )
{
    _velocity = velocity;
}



void Navigator::setDeltaVelocity( double dv )
{
    _velocity += dv;
}



Navigator::~Navigator( ) { }

