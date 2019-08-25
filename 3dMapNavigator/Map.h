/* 
 * File:   Map.h
 * Author: Nicolas Cortez
 *
 * Created on September 30, 2014, 1:54 PM
 */

#include "vetor3d.h"
#ifndef MAP_H
#define	MAP_H
#include <iostream>
#include <vector>
using namespace std;

class Map {
public:
    
    
/**
 *	Constroe o mapa a partir de um arquivo .txt
 *
 *	@param fileName nome do arquivo a ser lido
 */ 
    Map(const char *fileName );  
    
    
    
/**
 *	Obtem a dimensao em x do mapa de alturas
 *
 *	@return dimX
 */ 
    unsigned int getDimX();
    
    
    
 /**
 *	Obtem a dimensao em y do mapa de alturas
 *
 *	@return dimY
 */ 
    unsigned int getDimY();
    
    
    
 /**
 *	Obtem o espacamento em x entre os vertices do mapa de alturas
 *
 *	@return espX
 */ 
    float getEspX();
    
    
    
/**
 *	Obtem o espacamento em y entre os vertices do mapa de alturas
 *
 *	@return espY
 */
    float getEspY();
    
    
    
/**
 *	Obtem o buffer de vertices do mapa de alturas
 *
 *	@return coordinates
 */
    float *getCoordinates();
    
    
    
/**
 *	Obtem o buffer de normals do mapa de alturas
 *
 *	@return normals
 */
    float *getNormals();
    
    
    
/**
 *	Obtem o buffer dos paramentros da textura1D do mapa de alturas
 *
 *	@return textura1d
 */
    float *getTexture1D();
    
    
    
/**
 *	Obtem o buffer dos paramentros da textura2D do mapa de alturas
 *
 *	@return textura2D
 */
    float *getTexture2D();
    
    
    
/**
 *	Obtem o buffer dos indices dos vertices da triangulacao do mapa de alturas
 *
 *	@return triangles
 */  
    unsigned int *getTriangles();
    
    
/**
 *	Obtem o numero de vertices
 *
 *	@return numero de vertices
 */
    unsigned int getNVertices();
    
    
    
/**
 *	Obtem o numerode triangulos
 *
 *	@return numero de triangulos 
 */  
    unsigned int getNTriangles();
    float getHMax();
    float getHMin();
    vector <float> getErrors();
    vector <float> getRadius();
    int getLevel();
    vector<unsigned int> getTrianglesRefined( );
    
    
    //pre processamento do mapa gerando o vetor de raios e de erros
    unsigned int computeIndex(unsigned int j, unsigned int i);
    void vertexPreCompute( int i, int j, int di, int dj, int n );
    void meshPreCompute( unsigned int level );
    
    //funcao do lod 
    int active(unsigned int i);
    void triangleStripAppend( unsigned int indexVertex,int p );
    void submeshRefine(unsigned int i, unsigned int j, unsigned int k, int level);
    void meshRefine(float tolerancia,double &pX, double &pY, double &pZ, float heightCanvas  );
    
    //lod com view-Culling
    int viewCullingTest( unsigned int indexVertex  );
    void submeshRefineCulling(unsigned int i, unsigned int j, unsigned int k, int level);
    void meshRefineCulling(float tolerancia,double &pX, double &pY, double &pZ, float heightCanvas, double* frustumPlanes[]  );
   
    ~Map();
private:
    
    unsigned int _dimX;
    unsigned int _dimY;
    float _espX;
    float _espY;
    float _hMax;
    float _hMin;
    int _level;
    float *_coordinates;
    float *_normals;
    unsigned int *_triangles;
    float *_texture1D;
    float *_texture2D;
    unsigned int _nVertices;
    unsigned int _nTriangles;
    vector<float> _errors;
    vector<float> _radius;   
    vector <unsigned int> _trianglesRefined;
    float _k; 
    int _parity;

    Vetor3D <double> _observator;
    
    //viewCulling
    double** _frustumPlanes;
};


#endif	/* MAP_H */

