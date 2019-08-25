/* 
 * File:   Map.cpp
 * Author: treinamento
 * 
 * Created on September 30, 2014, 1:54 PM
 */
#include "Map.h"
#include "vetor3d.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>


#include <vector>
using namespace std;



Map::Map( const char *fileName )
{
    //Abre arquivo que contem as informacoes.
    std::ifstream in( fileName );

    //Verifica se houve erro na abertura.
    if (in.fail( ))
    {
        //Envia mensagem de erro.
        std::cout << "Erro ao abrir arquivo " << fileName << std::endl;

        //Aborta o programa.
        exit( 0 );
    }

    //Variavel para ler o texto e descarta-lo
    std::string trash;


    //Le a dimensao e o espacamento 
    in >> _dimX >> _dimY >> _espX >> _espY;
    _nVertices = _dimX*_dimY;
    _nTriangles = 2 * ( _dimX - 1 )*( _dimY - 1 );
    _hMax = -10E20;
    _hMin = 10E20;

    //calcula o level de refinamento
    _level = 0;
    int d = _dimX;
    while (d != 1)
    {
        d = d / 2;
        _level++;
    }



    //contador de indice do vertice
    unsigned int indexVertex = 0;
    _coordinates = new float [3 * _nVertices];
    for (unsigned int i = 0; i < _dimX; i++)
    {
        for (unsigned int j = 0; j < _dimY; j++)
        {
            float z;
            in >> z;
            _coordinates[3 * indexVertex + 0] = i*_espX;
            _coordinates[3 * indexVertex + 1] = j*_espY;
            _coordinates[3 * indexVertex + 2] = z;
            if (z > _hMax)
                _hMax = z;
            if (z < _hMin)
                _hMin = z;
            indexVertex++;
        }
    }



    //variavel para posicao no vetor de triangulos   
    unsigned int indexTriangle = 0;
    _triangles = new unsigned int [3 * _nTriangles ];
    for (unsigned int i = 0; i < _dimX - 1; i++)
    {
        for (unsigned int j = 0; j < _dimY - 1; j++)
        {
            //1 triangulo
            _triangles[3 * indexTriangle + 0] = computeIndex( j, i );
            _triangles[3 * indexTriangle + 1] = computeIndex( j, i + 1 );
            _triangles[3 * indexTriangle + 2] = computeIndex( j + 1, i );

            //outro triangulo 
            _triangles[3 * indexTriangle + 3] = computeIndex( j + 1, i );
            _triangles[3 * indexTriangle + 4] = computeIndex( j, i + 1 );
            _triangles[3 * indexTriangle + 5] = computeIndex( j + 1, i + 1 );
            indexTriangle += 2;
        }
    }

    _normals = new float [3 * _nVertices];
    for (unsigned int i = 0; i < _nTriangles; i++)
    {
        unsigned int indexVertex1 = _triangles[3 * i + 0];
        unsigned int indexVertex2 = _triangles[3 * i + 1];
        unsigned int indexVertex3 = _triangles[3 * i + 2];

        Vetor3D <float> aux1, aux2, aux3;
        aux1.setX( _coordinates[3 * indexVertex1 + 0] - _coordinates[3 * indexVertex2 + 0] );
        aux1.setY( _coordinates[3 * indexVertex1 + 1] - _coordinates[3 * indexVertex2 + 1] );
        aux1.setZ( _coordinates[3 * indexVertex1 + 2] - _coordinates[3 * indexVertex2 + 2] );

        aux2.setX( _coordinates[3 * indexVertex1 + 0] - _coordinates[3 * indexVertex3 + 0] );
        aux2.setY( _coordinates[3 * indexVertex1 + 1] - _coordinates[3 * indexVertex3 + 1] );
        aux2.setZ( _coordinates[3 * indexVertex1 + 2] - _coordinates[3 * indexVertex3 + 2] );
        aux3 = aux1 % aux2;

        _normals[3 * indexVertex1 + 0] += aux3.getX( );
        _normals[3 * indexVertex1 + 1] += aux3.getY( );
        _normals[3 * indexVertex1 + 2] += aux3.getZ( );

        _normals[3 * indexVertex2 + 0] += aux3.getX( );
        _normals[3 * indexVertex2 + 1] += aux3.getY( );
        _normals[3 * indexVertex2 + 2] += aux3.getZ( );

        _normals[3 * indexVertex3 + 0] += aux3.getX( );
        _normals[3 * indexVertex3 + 1] += aux3.getY( );
        _normals[3 * indexVertex3 + 2] += aux3.getZ( );
    }
    for (unsigned int i = 0; i < _nVertices; i++)
    {
        Vetor3D <float> aux;
        aux.setX( _normals[3 * i] );
        aux.setY( _normals[3 * i + 1] );
        aux.setZ( _normals[3 * i + 2] );
        aux.normalizaVet( );
        _normals[3 * i + 0] = aux.getX( );
        _normals[3 * i + 1] = aux.getY( );
        _normals[3 * i + 2] = aux.getZ( );
    }



    //calcula da textura por altura
    _texture1D = new float [_nVertices];
    for (int i = 0; i < _nVertices; i++)
    {
        //parametro em relacao a altura do vertice
        float t;
        t = ( _coordinates[i * 3 + 2] - _hMin ) / ( _hMax - _hMin );

        _texture1D[i] = t;

    }


    unsigned int indexTex = 0;
    _texture2D = new float [2 * _nVertices];
    for (int i = 0; i < _dimX; i++)
    {
        for (int j = 0; j < _dimY; j++)
        {
            //parametros em relacao a textura        
            float s = ( float ) i / ( _dimX - 1 );
            float t = ( float ) j / ( _dimY - 1 );
            _texture2D[2 * indexTex + 0] = s;
            _texture2D[2 * indexTex + 1] = t;
            indexTex++;
        }
    }



    //faz o pre processamento
    meshPreCompute( _level );


}


//refinamento



int Map::active( unsigned int i )
{
    Vetor3D <double> coordinate, aux;
    double lenSqrt;
    coordinate.setX( _coordinates[3 * i + 0] );
    coordinate.setY( _coordinates[3 * i + 1] );
    coordinate.setZ( _coordinates[3 * i + 2] );
    aux = coordinate - _observator;

    //  lenSqrt = |pi - e|^2 1050625
    lenSqrt = aux.produtoEscalar( aux );
    double right = ( ( 1.0 / _k ) * _errors[i] + _radius[i] )*( ( 1.0 / _k ) * _errors[i] + _radius[i] );
    if (right > lenSqrt)
    {
        return 1;
    }
    return 0;
}



void Map::triangleStripAppend( unsigned int indexVertex, int p )
{
    //parity(V)
    //indice ultimo vertice n tringle strip
    int n = _trianglesRefined.size( ) - 1;

    if (indexVertex != _trianglesRefined[n - 1] && indexVertex != _trianglesRefined[n])
    {
        if (p != _parity)
        {
            _parity = p;
        }
        else
        {
            _trianglesRefined.push_back( _trianglesRefined[n - 1] );
        }
        _trianglesRefined.push_back( indexVertex );
    }
}



void Map::submeshRefine( unsigned int i, unsigned int j, unsigned int k, int level )
{
    if (level > 0 && active( i ))
    {
        unsigned int med = ( j + k ) / 2;
        submeshRefine( med, j, i, level - 1 );
        triangleStripAppend( i, level % 2 );
        submeshRefine( med, i, k, level - 1 );
    }
}



void Map::meshRefine( float tolerancia, double &pX, double &pY, double &pZ, float heightCanvas )
{


    unsigned int indexSW = computeIndex( 0, 0 );
    unsigned int indexSE = computeIndex( 0, _dimX - 1 );
    unsigned int indexNW = computeIndex( _dimY - 1, 0 );
    unsigned int indexNE = computeIndex( _dimY - 1, _dimX - 1 );
    unsigned int indexC = computeIndex( ( _dimY - 1 ) / 2, ( _dimX - 1 ) / 2 );
    float fi = ( ( 3.1415 ) ) / 3;
    float lambda = heightCanvas / fi;
    _k = tolerancia / lambda;
    _parity = 0;
    //posicao do observador
    _observator.setX( pX );
    _observator.setY( pY );
    _observator.setZ( pZ );
    _trianglesRefined.reserve( _dimX * _dimY );
    _trianglesRefined.clear( );
    _trianglesRefined.push_back( indexSW );
    _trianglesRefined.push_back( indexSW );
    submeshRefine( indexC, indexSW, indexSE, 2 * _level );
    triangleStripAppend( indexSE, 1 );
    submeshRefine( indexC, indexSE, indexNE, 2 * _level );
    triangleStripAppend( indexNE, 1 );
    submeshRefine( indexC, indexNE, indexNW, 2 * _level );
    triangleStripAppend( indexNW, 1 );
    submeshRefine( indexC, indexNW, indexSW, 2 * _level );
    _trianglesRefined.push_back( indexSW );

}



void Map::submeshRefineCulling( unsigned int i, unsigned int j, unsigned int k, int level )
{
    if (level > 0 && active( i ))
    {
        if (viewCullingTest( i ) == 1)//totalmentedentro
        {
            unsigned int med = ( j + k ) / 2;
            submeshRefine( med, j, i, level - 1 );
            triangleStripAppend( i, level % 2 );
            submeshRefine( med, i, k, level - 1 );
        }
        else if (viewCullingTest( i ) == 2)
        {
            unsigned int med = ( j + k ) / 2;
            submeshRefineCulling( med, j, i, level - 1 );
            triangleStripAppend( i, level % 2 );
            submeshRefineCulling( med, i, k, level - 1 );
        }
    }
}



void Map::meshRefineCulling( float tolerancia, double &pX, double &pY, double &pZ, float heightCanvas, double* frustumPlanes[] )
{

    _frustumPlanes = frustumPlanes;
    unsigned int indexSW = computeIndex( 0, 0 );
    unsigned int indexSE = computeIndex( 0, _dimX - 1 );
    unsigned int indexNW = computeIndex( _dimY - 1, 0 );
    unsigned int indexNE = computeIndex( _dimY - 1, _dimX - 1 );
    unsigned int indexC = computeIndex( ( _dimY - 1 ) / 2, ( _dimX - 1 ) / 2 );
    float fi = ( ( 3.1415 ) ) / 3;

    float lambda = heightCanvas / fi;
    _k = tolerancia / lambda;
    _parity = 0;
    //posicao do observador
    _observator.setX( pX );
    _observator.setY( pY );
    _observator.setZ( pZ );

    _trianglesRefined.reserve( _dimX * _dimY );
    _trianglesRefined.clear( );
    _trianglesRefined.push_back( indexSW );
    _trianglesRefined.push_back( indexSW );
    submeshRefineCulling( indexC, indexSW, indexSE, 2 * _level );
    triangleStripAppend( indexSE, 1 );
    submeshRefineCulling( indexC, indexSE, indexNE, 2 * _level );
    triangleStripAppend( indexNE, 1 );
    submeshRefineCulling( indexC, indexNE, indexNW, 2 * _level );
    triangleStripAppend( indexNW, 1 );
    submeshRefineCulling( indexC, indexNW, indexSW, 2 * _level );
    _trianglesRefined.push_back( indexSW );

}



int Map::viewCullingTest( unsigned int indexVertex )
{
    bool outsideFrutum = false;
    double distance, dMin = 10E20;
    for (int i = 0; i < 6; i++)
    {
        distance = _coordinates[indexVertex * 3] * _frustumPlanes[i][0] + _coordinates[indexVertex * 3 + 1] * _frustumPlanes[i][1]
            + _coordinates[ indexVertex * 3 + 2] * _frustumPlanes[i][2] + _frustumPlanes[i][3];
        if (fabs( distance ) < dMin)
        {
            dMin = fabs( distance );
        }
        if (distance < 0)
        {
            outsideFrutum = true;
        }
    }

    if (dMin > _radius[indexVertex] && outsideFrutum)//totalmente fora
    {
        return 0;
    }
    else if (dMin > _radius[indexVertex] && !outsideFrutum)//totalmente dentro
    {
        return 1;
    }
    else //parcialmente 
    {
        return 2;
    }
}



unsigned int Map::getDimX( )
{
    return _dimX;
}



unsigned int Map::getDimY( )
{
    return _dimY;
}



float Map::getEspX( )
{
    return _espX;
}



float Map::getEspY( )
{
    return _espY;
}



float* Map::getCoordinates( )
{
    return _coordinates;
}



float* Map::getNormals( )
{
    return _normals;
}



float* Map::getTexture1D( )
{
    return _texture1D;
}



float* Map::getTexture2D( )
{
    return _texture2D;
}



unsigned int* Map::getTriangles( )
{
    return _triangles;
}



unsigned int Map::getNVertices( )
{
    return _nVertices;
}



unsigned int Map::getNTriangles( )
{
    return _nTriangles;
}



float Map::getHMax( )
{
    return _hMax;
}



float Map::getHMin( )
{
    return _hMin;
}



vector <float> Map::getErrors( )
{
    return _errors;
}



vector <float> Map::getRadius( )
{
    return _radius;
}



int Map::getLevel( )
{

    return _level;
}



vector<unsigned int> Map::getTrianglesRefined( )
{
    return _trianglesRefined;
}



unsigned int Map::computeIndex( unsigned int j, unsigned int i )
{
    return i * _dimY + j;
}



//Pre-Processamento



void Map::vertexPreCompute( int i, int j, int di, int dj, int n )
{
    //Calcula indice do vertice na posicao [j,i].
    unsigned int indexJI = computeIndex( j, i );
    unsigned int indexJDJIDIMinus = computeIndex( j - dj, i - di );
    unsigned int indexJDJIDIPlus = computeIndex( j + dj, i + di );

    //Inicializa erro e raio do vertice [j,i].
    _errors[indexJI] = fabs( _coordinates[3 * indexJI + 2] - 0.5f * ( _coordinates[3 * indexJDJIDIMinus + 2] + _coordinates[3 * indexJDJIDIPlus + 2] ) );
    _radius[indexJI] = 0.0f;

    //Verifica se existe uma matriz.
    if (n)
    {
        dj = ( di + dj ) / 2;
        di -= dj;
        int k = 4;
        do
        {
            if (( i != 0 || di >= 0 ) && ( i != n || di <= 0 ) &&
                ( j != 0 || dj >= 0 ) && ( j != n || dj <= 0 ))
            {
                indexJI = computeIndex( j, i );
                indexJDJIDIPlus = computeIndex( j + dj, i + di );

                //Atualiza erro.
                _errors[indexJI] = std::max( _errors[indexJI], _errors[indexJDJIDIPlus] );

                //Calcula a disntancia entre os vertices somado ao raio.
                float x1 = _coordinates[3 * indexJI + 0];
                float y1 = _coordinates[3 * indexJI + 1];
                float z1 = _coordinates[3 * indexJI + 2];

                float x2 = _coordinates[3 * indexJDJIDIPlus + 0];
                float y2 = _coordinates[3 * indexJDJIDIPlus + 1];
                float z2 = _coordinates[3 * indexJDJIDIPlus + 2];

                //Calcula distancia dos dois vertices somada ao raio da esfera do
                //vertice [j + dj, i + di]
                float r = sqrt( ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 ) +
                                ( z1 - z2 ) * ( z1 - z2 ) ) + _radius[indexJDJIDIPlus];

                //Atualiza o raio do vertice corrente.
                _radius[indexJI] = std::max( _radius[indexJI], r );
            }

            dj += di;
            di -= dj;
            dj += di;
        }
        while (--k);
    }
}



void Map::meshPreCompute( unsigned int level )
{
    //Aloca espaco para raios e erros.
    _errors.clear( );
    _radius.clear( );
    _errors.resize( _dimX * _dimY );
    _radius.resize( _dimX * _dimY );

    //Calcula o numero de vertices em uma dimensao.
    int n = 1 << ( level );

    //Calcula indice do ponto central na matriz. [m,m]
    int m = n / 2;

    //Variaveis do for. Não façam isso em casa.
    int s;
    int a, b, c;
    for (a = c = 1, b = 2, s = 0; a != n; a = c = b, b *= 2, s = n)
    {
        //Processa os niveis pretos da quadtree.
        for (int j = a; j < n; j += b)
        {
            for (int i = 0; i <= n; i += b)
            {
                vertexPreCompute( i, j, 0, a, s );
                vertexPreCompute( j, i, a, 0, s );
            }
        }

        //Processa os niveis brancos da quadtree
        for (int j = a; j < n; c = -c, j += b)
        {
            for (int i = a; i < n; c = -c, i += b)
            {
                vertexPreCompute( i, j, a, c, n );
            }
        }
    }

    //Calcula indices dos pontos de erro e esferas maximo.
    unsigned int index00 = computeIndex( 0, 0 );
    unsigned int indexN0 = computeIndex( n, 0 );
    unsigned int index0N = computeIndex( 0, n );
    unsigned int indexNN = computeIndex( n, n );
    unsigned int indexMM = computeIndex( m, m );

    //Atribui erro e raio maximo aos pontos de level 0.
    _errors[index00] = 10E20;
    _errors[indexN0] = 10E20;
    _errors[index0N] = 10E20;
    _errors[indexNN] = 10E20;
    _errors[indexMM] = 10E20;

    _radius[index00] = 10E20;
    _radius[indexN0] = 10E20;
    _radius[index0N] = 10E20;
    _radius[indexNN] = 10E20;
    _radius[indexMM] = 10E20;
}



Map::~Map( )
{
}

