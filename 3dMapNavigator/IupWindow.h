/* 
 * File:   IupWindow.h
 * Author: treinamento
 *
 * Created on September 26, 2014, 9:59 AM
 */

#ifndef IUPWINDOW_H
#define	IUPWINDOW_H

#include "Map.h"
#include <iup/iup.h>
#include <GL/gl.h>
#include "Navigator.h"
#include "image.h"
//#include <Windows.h>

class IupWindow {
public:

    /**
 *	Construtor da classe
 */
    IupWindow();

/**
 *	Mostra a janela principal
 */
    void show();
    
    
    /**
 *	Esconde a janela principal
 */
    void hide();
    
    /**
 *	Atualiza o canvas redesenhando 
 */
    void updateCanvas();
    
    /**
 *	Destrutor da classe
 */
    ~IupWindow();


    
/**
 *	Gera um plano ultilizado no View-Culling
 *
 *	@param frustumPoints[]   vetor de pontos do viewport
 *	@param p1  indice de um ponto
 *	@param p2  indice de outro ponto
 *  	@param p3 indice de outro ponto
 *  	@param plane indice do plano a ser gerado 
 */
    void computePlan(  Vetor3D <double> frustumPoints[], int p1, int p2, int p3, int plane );
    
    
 /**
 *	Gera todos planos ultilizados no View-Culling
 *
 *	@param frustumPoints[]   vetor de pontos do viewport
 */
    void computeFrustumPlanes( Vetor3D <double> frustumPoints[] );
    
    
 /**
 *	Gera todos planos ultilizados no View-Culling 
 */
    void computeFrustumPlanes( );

private:

    //recupera o dialogo
    Ihandle* _dialog;
    
    //mapa de alturas corrente
    Map* _Map; 
    
    //dimensao do canvas
    float _withCanvas;
    float _heightCanvas;
    
    //buffer para guardar as coordenadas dos vertices
    float *_bufferVertices;
    
    //numeros de vertices
    unsigned int _nVertices;
    
    //buffer para guardar as normais dos vertices
    float *_bufferNormals;
    
    //buffer para guardar a triangulacao com os indices dos vertices
    unsigned int *_bufferTriangles;
    
    //numero de triangulos 
    unsigned int _nTriangles;
    
    //buffer para guardar os parametros da textura 1d para cada vertice 
    float *_bufferTexture1D;
    
    //buffer para guradar o sparamentros da textura 2d para cada vertice
    float *_bufferTexture2D;
    
    //gurada todos os buffers 
    GLuint _buffersId[4];
    GLuint _textureId, _texture2DId;
    
    //cores da textura 1d
    float *_texture;
    
    //posicao da camera no canvas
    double _pX, _pY, _pZ;
    
    //target da camera no canvas
    double _tX, _tY, _tZ;
    
    //navegador 
    Navigator *_navi;
    
    //teclas apertadas
    bool _keys[256]; 
    
    //posicao do mouse 
    int _xMouse;
    int _yMouse;
    int _pressedMouse;
    
    bool _wireframeState;
    
    //altura maxima e altura minima
    float _hMax;
    float _hMin;
    float _erroTol;
    
    //viewCulling
    double** _frustumPlanes;
    
    
/**
 *	Cria a janela iup
 */
    void createWindow();
    
    
    
 /**
 *	Fecha o programa
 *
 *	@param ih do botao 
 *
 *	@return Iup_Default
 */
    static int exitButton(Ihandle* ih);
    
    
      
 /**
 *	Desenha a cena no canvas
 *
 *	@param ih do canvas 
 *
 *	@return Iup_Default
 */
    static int renderScene(Ihandle *ih);
  
    
    
/**
 *	Redimensiona o canvas mantendo a razao de aspecto
 *
 *	@param ih do canvas
 *      @param w largura do canvas
 *      @param h altura do canvas
 *
 *	@return Iup_Default
 */
    static int reshapeCallback(Ihandle *ih, int w, int h );
    
    
/**
 *	Capta os teclas precionadas do teclado atualizando o vetor keys
 *
 *	@param ih do canvas
 *      @param c identificador da tecla
 *      @param press estado da tecla
 *
 *	@return Iup_Default
 */
    static int keyboardCallback(Ihandle *ih, int c, int press );
    
    
    
 /**
 *	Calcula a varaicao da posicao do mouse precionado  
 *
 *	@param ih do canvas
 *      @param x posicao corrente do mouse no canvas
 *      @param y posicao corrrente do mouse no canvas
 *      @param status 
 * 
 *	@return Iup_Default
 */
    static int mouseMotionCallback(Ihandle *ih, int x, int y,char* status );
    
    
  
/**
 *	Redimensiona o canvas mantendo a razao de aspecto
 *
 *	@param ih do canvas
 *      @param button identificador do botao do mouse pressionado
 *      @param pressed estado do botao
 *      @param x posicao corrente do mouse no canvas
 *      @param y posicao corrrente do mouse no canvas@param h altura do canvas
 * 
 *	@return Iup_Default
 */
    static int mouseInicialCallback(Ihandle *ih, int button, int pressed,int x, int y);
    
    
    
/**
 *     Abrir um arquivo com os dados do mapa de altura e ler-lo capturando suas informacoes e calculando algumas
 *
 *	@param ih da opcao de menu Abrir
 * 
 *	@return Iup_Default
 */
    static int itemAbrirCallback(Ihandle *ih);
    
    
    
/**
 *	Salvar o print de uma janela 
 *
 *	@param ih da opcao de menu print
 * 
 *	@return Iup_Default
 */    
    static int itemPrintCallback(Ihandle *ih);
   
    
    
/**
 *	Fechar o programa
 *
 *	@param ih da opcao de menu sair
 * 
 *	@return Iup_Default
 */ 
    static int itemSairCallback(Ihandle *ih);

    
    
/**
 *	Ativa o modulo wireframe
 *
 *	@param ih da opcao de menu wireframe
 * 
 *	@return Iup_Default
 */ 
    static int itemWireframeCallback(Ihandle *ih);
    
    
    
 /**
 *	Abre a janela de preferencias
 *
 *	@param ih da opcao de menu preferencias
 * 
 *	@return Iup_Default
 */ 
    static int itemPreferencesCallback(Ihandle *ih);
    
    
 /**
 *	Inicializa o canvas
 */ 
    void initCanvas();
    
    

};

#endif	/* IUPWINDOW_H */
