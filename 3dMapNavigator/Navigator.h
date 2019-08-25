/* 
 * File:   Navigator.h
 * Author: mgattass
 *
 * Created on September 13, 2014, 5:30 PM
 */

#ifndef NAVIGATOR_H
#define	NAVIGATOR_H

class Navigator
{
public:
    /**
     * Construtor default.
     * @param px, py, pz - posicao inicial da camera.
     */
    Navigator( double px, double py, double pz );

    /**
     * 
     * @param keys[in] - teclas que estao pressionadas atualmente.
     * @param px, py, pz[out] - posicao da camera
     * @param tx, ty, tz[out] - posicao para onde a camera esta apontada.
     */
    void computeCamera( const bool keys[], double &px, double &py, double &pz,
                        double &tx, double &ty, double &tz );

    /**
     * Define a variacao da rotacao em x e em y.
     * @param dx - variacao da rotacao em x.
     * @param dy - variacao da rotacao em y.
     */
    void setDeltaRotation( double dx, double dy );

    /**
     * Define a velocidade de movimento.
     * @param velocity - velocidade de movimento.
     */
    void setVelocity( double velocity );

    /**
     * Define a variacao da velocidade de movimento.
     * @param dv - variacao da velocidade de movimento.
     */
    void setDeltaVelocity( double dv );

    /**
     * Default
     */
    virtual ~Navigator( );
private:
    /**
     * Posicao da camera.
     */
    double _positionX, _positionY, _positionZ;

    /**
     * Ponto alvo da camera.
     */
    double _targetX, _targetY, _targetZ;

    /**
     * Direcao que a camera anda.
     */
    double _directionX, _directionY, _directionZ;

    /**
     * Velocidade que a camera anda.
     */
    double _velocity;

    /**
     * Rotacao em x e y.
     */
    double _rotateX, _rotateY;

    /**
     * Velocidade de rotacao.
     */
    double _velocityRotation;

};

#endif	/* NAVIGATOR_H */

