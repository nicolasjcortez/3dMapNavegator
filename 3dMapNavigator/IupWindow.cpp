/* 
 * File:   IupWindow.cpp
 * Author: Nicolas
 * 
 * 
 */
#include <GL/glew.h>
#include "image.h"
#include "IupWindow.h"
#include "IupPreferencesWindow.h"
#include "Navigator.h"
#include <cstdlib>
#include <iup/iupgl.h>
#include <GL/gl.h>
#include <cstdlib>
#include <GL/glu.h>
#include<cmath>
#include <cstdio>
#include <iostream>
#include "vetor3d.h"
using namespace std;
//#include <Windows.h>



IupWindow::IupWindow( )
{
    _pX = 0;
    _pY = 0;
    _pZ = 0;
    delete _navi;
    _navi = new Navigator( _pX, _pY, _pZ );

    _frustumPlanes = new double* [6];
    for (int i = 0; i < 6; ++i)
    {
        _frustumPlanes[i] = new double[4];
    }
    _erroTol = 1;
    _tX = 100;
    _tY = 100;
    _tZ = 100;
    _texture = new float[12];
    _texture[0] = 0;
    _texture[1] = 0;
    _texture[2] = 1;

    _texture[3] = 0;
    _texture[4] = 1;
    _texture[5] = 0;

    _texture[6] = 1;
    _texture[7] = 1;
    _texture[8] = 0;

    _texture[9] = 1;
    _texture[10] = 0;
    _texture[11] = 0;

    for (int i = 0; i < 256; i++)
    {
        _keys[i] = false;
    }
    _wireframeState = false;
    createWindow( );

}



void IupWindow::show( )
{
    IupShow( _dialog );
}



void IupWindow::hide( )
{
    IupHide( _dialog );
}



void IupWindow::updateCanvas( )
{
    Ihandle *canvas = ( Ihandle* ) IupGetAttribute( _dialog, "_CANVAS" );
    //Marca o canvas para ser redesenhado·
    IupUpdate( canvas );
}



IupWindow::~IupWindow( )
{
}



int IupWindow::exitButton( Ihandle* ih )
{
    return IUP_CLOSE;
}




int IupWindow::itemAbrirCallback( Ihandle * ih )
{
    // abrindo o arquivo 
    IupWindow *window = ( IupWindow* ) IupGetAttribute( ih, "_THIS" );
    Ihandle *canvas = ( Ihandle* ) IupGetAttribute( ih, "_CANVAS" );
    IupGLMakeCurrent( canvas );
    Ihandle *dlg = IupFileDlg( );

    IupSetAttribute( dlg, "DIALOGTYPE", "OPEN" );
    IupSetAttribute( dlg, "TITLE", "Selecione o Arquivo" );

    IupPopup( dlg, IUP_CURRENT, IUP_CURRENT );


    if (IupGetInt( dlg, "STATUS" ) != -1)
    {
        //captura o nome do arquivo com as coordenadas dos vertices 
        const char *fileName = IupGetAttribute( dlg, "VALUE" );
        delete window->_Map;
        window->_Map = new Map( fileName );
    }
    window->_nVertices = window->_Map->getNVertices( );
    window->_nTriangles = window->_Map->getNTriangles( );



    window->_bufferVertices = window->_Map->getCoordinates( );
    window->_bufferNormals = window->_Map->getNormals( );
    window->_bufferTriangles = window->_Map->getTriangles( );
    window->_bufferTexture1D = window->_Map->getTexture1D( );
    window->_bufferTexture2D = window->_Map->getTexture2D( );


    // Gera um número de identificacao para a textura
    glGenTextures( 1, &window->_textureId );
    // Associa a textura aos comandos seguintes
    glBindTexture( GL_TEXTURE_1D, window->_textureId );
    // Define os filtros de magnificacao e minificacao.
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
    // Envia a textura para uso pela OpenGL. (processo de uppload)
    glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB, 4, 0, GL_RGB, GL_FLOAT, window->_texture );



    Image *image = imgReadBMP( "tx2.bmp" );

    //cria uma identificacao para a textura
    glGenTextures( 1, &window->_texture2DId );
    glBindTexture( GL_TEXTURE_2D, window->_texture2DId );

    //coloca a imagem na identificacao que foi dada bind 
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imgGetWidth( image ), imgGetHeight( image ), 0, GL_RGB, GL_FLOAT, imgGetData( image ) );
    // Define os filtros de magnificação e minificação
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    // Seleciona o modo de aplicação da textura
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
    // Ajusta os parâmetros de repetição
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    imgDestroy( image );



    glGenBuffers( 4, window->_buffersId );

    glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[0] );
    glBufferData( GL_ARRAY_BUFFER, 3 * ( window->_nVertices ) * sizeof ( float ), window->_bufferVertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[1] );
    glBufferData( GL_ARRAY_BUFFER, 3 * ( window->_nVertices ) * sizeof ( float ), window->_bufferNormals, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[2] );
    glBufferData( GL_ARRAY_BUFFER, ( window->_nVertices ) * sizeof ( float ), window->_bufferTexture1D, GL_STATIC_DRAW );


    glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[3] );
    glBufferData( GL_ARRAY_BUFFER, 2 * ( window->_nVertices ) * sizeof ( float ), window->_bufferTexture2D, GL_STATIC_DRAW );


    window->_hMin = window->_Map->getHMin( );
    window->_hMax = window->_Map->getHMax( );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    //gluLookAt( ( window->_Map->getDimX( ) * window->_Map->getEspX( ) ) / 2, ( window->_Map->getDimY( ) * window->_Map->getEspY( ) ) / 2, hMax, 0, 0, 0, 0, 0, 1 );
    gluLookAt( window->_pX, window->_pY, window->_pZ, window->_tX, window->_tY, window->_tZ, 0, 0, 1 );

    //Marca o canvas para ser redesenhado·
    IupUpdate( canvas );
    IupDestroy( dlg );
    return IUP_DEFAULT;
}



int IupWindow::itemPrintCallback( Ihandle * ih )
{
    IupWindow *window = ( IupWindow* ) IupGetAttribute( ih, "_THIS" );
    Image* print = imgCreate( window->_withCanvas, window->_heightCanvas, 3 );
    float *buffer = imgGetData( print );
    glReadPixels( 0, 0, window->_withCanvas, window->_heightCanvas, GL_RGB, GL_FLOAT, buffer );

    Ihandle* dlg = IupFileDlg( );
    IupSetAttribute( dlg, "TITLE", "Print Canvas" );
    IupSetAttribute( dlg, "DIALOGTYPE", "Save" );
    IupPopup( dlg, IUP_CURRENT, IUP_CURRENT );


    if (IupGetInt( dlg, "STATUS" ) != -1)
    {
        char* fileNameDialog = ( IupGetAttribute( dlg, "VALUE" ) );
        char fileName[1024];
        sprintf( fileName, "%s.bmp", fileNameDialog );
        imgWriteBMP( fileName, print );
    }

    return IUP_DEFAULT;

}



int IupWindow::itemSairCallback( Ihandle * ih )
{
    return IUP_CLOSE;
}



int IupWindow::itemWireframeCallback( Ihandle * ih )
{
    IupWindow *window = ( IupWindow* ) IupGetAttribute( ih, "_THIS" );
    Ihandle *canvas = ( Ihandle* ) IupGetAttribute( ih, "_CANVAS" );
    if (window->_wireframeState == false)
        window->_wireframeState = true;
    else
        window->_wireframeState = false;
    IupUpdate( canvas );
    return IUP_DEFAULT;
}



int IupWindow::itemPreferencesCallback( Ihandle * ih )
{
    IupPreferencesWindow::getInstance( )->show( );
    return IUP_DEFAULT;
}



int IupWindow::renderScene( Ihandle *ih )
{
    IupGLMakeCurrent( ih );
    IupWindow *window = ( IupWindow* ) IupGetAttribute( ih, "_THIS" );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    window->_navi->computeCamera( window->_keys, window->_pX, window->_pY, window->_pZ, window->_tX, window->_tY, window->_tZ );
    gluLookAt( window->_pX, window->_pY, window->_pZ, window->_tX, window->_tY, window->_tZ, 0, 0, 1 );


    float colorBack[3];
    colorBack[0] = IupPreferencesWindow::getInstance( )->getColorBackR( );
    colorBack[1] = IupPreferencesWindow::getInstance( )->getColorBackG( );
    colorBack[2] = IupPreferencesWindow::getInstance( )->getColorBackB( );
    glClearColor( colorBack[0], colorBack[1], colorBack[2], 0.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    float colorSurf[3];
    colorSurf[0] = IupPreferencesWindow::getInstance( )->getColorSurfR( );
    colorSurf[1] = IupPreferencesWindow::getInstance( )->getColorSurfG( );
    colorSurf[2] = IupPreferencesWindow::getInstance( )->getColorSurfB( );
    glColor3f( colorSurf[0], colorSurf[1], colorSurf[2] );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //captura dados da janela de preferencia
    int typeSurf = IupPreferencesWindow::getInstance( )->getTypeSurf( );
    int typeColorSurf = IupPreferencesWindow::getInstance( )->getTypeColorSurf( );
    int numDim = IupPreferencesWindow::getInstance( )->getNumDim( );
    float erroTol = IupPreferencesWindow::getInstance( )->getErroTol( );
    int dimension = IupPreferencesWindow::getInstance( )->getNumDim( );
    
    if (window->_Map != NULL && dimension == 2)//caso 2d
    {
        glDisable( GL_LIGHTING );

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );
        gluOrtho2D( 0, ( window->_Map->getDimX( ) - 1 ) * window->_Map->getEspX( ), 0, ( window->_Map->getDimY( ) - 1 ) * window->_Map->getEspY( ) );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity( );

        if (window->_wireframeState)
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }

        //passa o buffer de coordenadas
        glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[0] );
        glEnableClientState( GL_VERTEX_ARRAY );
        //ignora a altura
        glVertexPointer( 2, GL_FLOAT, 3 * sizeof(float), ( void* ) 0 );


        if (typeColorSurf == 2)
        {

            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glColor3f( 1, 1, 1 );

            //caso seja cor por altura passa o buffer com os paramentros de cada vertice
            glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[2] );
            glEnable( GL_TEXTURE_1D );

            //passa as cores que irao variar segundo o parametro 
            glBindTexture( GL_TEXTURE_1D, window->_textureId );
            glEnableClientState( GL_TEXTURE_COORD_ARRAY );
            glTexCoordPointer( 1, GL_FLOAT, 0, ( void* ) 0 );
        }
        if (typeColorSurf == 3)
        {
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glColor3f( 1, 1, 1 );

            //caso seja textura 2d, passa o buffer com os parametros 
            glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[3] );
            glEnable( GL_TEXTURE_2D );

            //passa a imagem 
            glBindTexture( GL_TEXTURE_2D, window->_texture2DId );
            glEnableClientState( GL_TEXTURE_COORD_ARRAY );
            glTexCoordPointer( 2, GL_FLOAT, 0, ( void* ) 0 );
        }
        if (typeSurf == 1)
        {
            glDrawElements( GL_TRIANGLES, 3 * window->_nTriangles, GL_UNSIGNED_INT, window->_bufferTriangles );
        }
        else if (typeSurf == 2)
        {
            //Refine *refine = new Refine(window->_Map, erroTol, window->_pX, window->_pY, window->_pZ, 0, window->_heightCanvas );
            //vector <unsigned int> trianglesRefine = window->_Map->getTrianglesRefined();
            //glDrawElements( GL_TRIANGLES, trianglesRefine.size(), GL_UNSIGNED_INT, &trianglesRefine[0]);
            window->_Map->meshRefine( erroTol, window->_pX, window->_pY, window->_pZ, window->_heightCanvas );
            const vector <unsigned int>& trianglesRefine = window->_Map->getTrianglesRefined( );
            glDrawElements( GL_TRIANGLE_STRIP, trianglesRefine.size( ), GL_UNSIGNED_INT, &trianglesRefine[0] );
        }
        else if (typeSurf == 3)
        {
            //planos do viewCulling
            window->computeFrustumPlanes( );
            window->_Map->meshRefineCulling( erroTol, window->_pX, window->_pY, window->_pZ, window->_heightCanvas, window->_frustumPlanes );
            const vector <unsigned int>& trianglesRefine = window->_Map->getTrianglesRefined( );
            glDrawElements( GL_TRIANGLE_STRIP, trianglesRefine.size( ), GL_UNSIGNED_INT, &trianglesRefine[0] );
        }

        if (window->_wireframeState)
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        
        glDisableClientState( GL_VERTEX_ARRAY );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        glDisable( GL_TEXTURE_1D );
        glDisable( GL_TEXTURE_2D );
        

    }
    if (window->_Map != NULL && dimension == 3)
    {
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );
        gluPerspective( 60.0, ( GLfloat ) window->_withCanvas / ( GLfloat ) window->_heightCanvas, 0.1, 1000 );
        
        if (window->_wireframeState)
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }

        //passa o buffer de coordenadas
        glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[0] );
        glEnableClientState( GL_VERTEX_ARRAY );
        glVertexPointer( 3, GL_FLOAT, 0, ( void* ) 0 );

        //passa o buffer de normal 
        glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[1] );
        glEnableClientState( GL_NORMAL_ARRAY );
        glNormalPointer( GL_FLOAT, 0, ( void* ) 0 );

        if (typeColorSurf == 2)
        {

            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glColor3f( 1, 1, 1 );

            //caso seja cor por altura passa o buffer com os paramentros de cada vertice
            glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[2] );
            glEnable( GL_TEXTURE_1D );

            //passa as cores que irao variar segundo o parametro 
            glBindTexture( GL_TEXTURE_1D, window->_textureId );
            glEnableClientState( GL_TEXTURE_COORD_ARRAY );
            glTexCoordPointer( 1, GL_FLOAT, 0, ( void* ) 0 );
        }
        if (typeColorSurf == 3)
        {
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glColor3f( 1, 1, 1 );

            //caso seja textura 2d, passa o buffer com os parametros 
            glBindBuffer( GL_ARRAY_BUFFER, window->_buffersId[3] );
            glEnable( GL_TEXTURE_2D );

            //passa a imagem 
            glBindTexture( GL_TEXTURE_2D, window->_texture2DId );
            glEnableClientState( GL_TEXTURE_COORD_ARRAY );
            glTexCoordPointer( 2, GL_FLOAT, 0, ( void* ) 0 );
        }
        if (typeSurf == 1)
        {
            glDrawElements( GL_TRIANGLES, 3 * window->_nTriangles, GL_UNSIGNED_INT, window->_bufferTriangles );
        }
        else if (typeSurf == 2)
        {
            //Refine *refine = new Refine(window->_Map, erroTol, window->_pX, window->_pY, window->_pZ, 0, window->_heightCanvas );
            //vector <unsigned int> trianglesRefine = window->_Map->getTrianglesRefined();
            //glDrawElements( GL_TRIANGLES, trianglesRefine.size(), GL_UNSIGNED_INT, &trianglesRefine[0]);
            window->_Map->meshRefine( erroTol, window->_pX, window->_pY, window->_pZ, window->_heightCanvas );
            const vector <unsigned int>& trianglesRefine = window->_Map->getTrianglesRefined( );
            glDrawElements( GL_TRIANGLE_STRIP, trianglesRefine.size( ), GL_UNSIGNED_INT, &trianglesRefine[0] );
        }
        else if (typeSurf == 3)
        {
            //planos do viewCulling
            window->computeFrustumPlanes( );
            window->_Map->meshRefineCulling( erroTol, window->_pX, window->_pY, window->_pZ, window->_heightCanvas, window->_frustumPlanes );
            const vector <unsigned int>& trianglesRefine = window->_Map->getTrianglesRefined( );
            glDrawElements( GL_TRIANGLE_STRIP, trianglesRefine.size( ), GL_UNSIGNED_INT, &trianglesRefine[0] );
        }

        if (window->_wireframeState)
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        glDisableClientState( GL_VERTEX_ARRAY );
        glDisableClientState( GL_NORMAL_ARRAY );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        glDisable( GL_TEXTURE_1D );
        glDisable( GL_TEXTURE_2D );
    }

    IupGLSwapBuffers( ih );

    return IUP_DEFAULT;
}



void IupWindow::initCanvas( )
{
    GLenum err = glewInit( );
    if (err != GLEW_OK)
    {
        std::cerr << "Error: " << glewGetErrorString( err ) << std::endl;
    }
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    GLfloat matSpecular[] = { 1, 1, 1, 1.0 };
    GLfloat matShininess[] = { 50 };
    GLfloat lightPosition[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat lightPosition1[] = { -1.0, -1.0, -1.0, 0.0 };
    GLfloat whiteLight[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lmodelAmbient[] = { 0.3, 0.3, 0.3, 1.0 };
    float colorBack[3];
    colorBack[0] = IupPreferencesWindow::getInstance( )->getColorBackR( );
    colorBack[1] = IupPreferencesWindow::getInstance( )->getColorBackG( );
    colorBack[2] = IupPreferencesWindow::getInstance( )->getColorBackB( );
    glClearColor( colorBack[0], colorBack[1], colorBack[2], 0.0 );

    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, matShininess );

    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, whiteLight );
    glLightfv( GL_LIGHT0, GL_SPECULAR, whiteLight );


    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition1 );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, whiteLight );
    glLightfv( GL_LIGHT1, GL_SPECULAR, whiteLight );


    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lmodelAmbient );

    glEnable( GL_COLOR_MATERIAL );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHT1 );
    glEnable( GL_DEPTH_TEST );

}



int IupWindow::reshapeCallback( Ihandle *ih, int w, int h )
{

    IupWindow *window = ( IupWindow* ) IupGetAttribute( ih, "_THIS" );

    IupGLMakeCurrent( ih );
    window->_withCanvas = w;
    window->_heightCanvas = h;
    glViewport( 0, 0, ( GLsizei ) w, ( GLsizei ) h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 60.0, ( GLfloat ) w / ( GLfloat ) h, 0.1, 1000 );

    IupUpdate( ih );

    return IUP_DEFAULT;
}



int IupWindow::keyboardCallback( Ihandle *ih, int c, int press )
{
    IupWindow *window = ( IupWindow* ) IupGetAttribute( ih, "_THIS" );
    Ihandle *canvas = ( Ihandle* ) IupGetAttribute( ih, "_CANVAS" );
    if (press == 1)
    {
        window->_keys[c] = true;

    }
    else if (press == 0)
    {
        window->_keys[c] = false;
    }
    IupUpdate( canvas );
    return IUP_DEFAULT;
}



int IupWindow::mouseMotionCallback( Ihandle *ih, int x, int y, char *status )
{
    IupWindow *window = ( IupWindow* ) IupGetAttribute( ih, "_THIS" );
    Ihandle *canvas = ( Ihandle* ) IupGetAttribute( ih, "_CANVAS" );

    if (window->_pressedMouse == 1)
    {
        window->_navi->setDeltaRotation( x - window->_xMouse, window->_yMouse - y );
        window->_xMouse = x;
        window->_yMouse = y;
    }
    IupUpdate( canvas );
    return IUP_DEFAULT;
}



int IupWindow::mouseInicialCallback( Ihandle *ih, int button, int pressed, int x, int y )
{
    IupWindow *window = ( IupWindow* ) IupGetAttribute( ih, "_THIS" );
    Ihandle *canvas = ( Ihandle* ) IupGetAttribute( ih, "_CANVAS" );

    if (button == IUP_BUTTON1)
    {
        //atualiza estado
        window->_pressedMouse = pressed;
        window->_xMouse = x;
        window->_yMouse = y;
    }
    IupUpdate( canvas );
    return IUP_DEFAULT;
}



void IupWindow::createWindow( )
{

    //Cria botao.
    Ihandle *button2 = IupButton( "Sair", NULL );

    Ihandle *canvas = IupGLCanvas( NULL );

    Ihandle *hbox = IupHbox( IupFill( ), button2, NULL );



    Ihandle *itemAbrir = IupItem( "Abrir", NULL );
    Ihandle *itemPrint = IupItem( "Salvar Print", NULL );
    Ihandle *itemSair = IupItem( "Sair", NULL );
    Ihandle *itemWireframe = IupItem( "wireframe", NULL );
    Ihandle *itemPreferences = IupItem( "Preferencias", NULL );
    Ihandle *menuFile = IupMenu( itemAbrir, itemPrint, itemSair, NULL );
    Ihandle *menuEdit = IupMenu( itemWireframe, itemPreferences, NULL );
    Ihandle *submenuEdit = IupSubmenu( "Editar", menuEdit );
    Ihandle *submenuFile = IupSubmenu( "Arquivo", menuFile );
    Ihandle *menuMain = IupMenu( submenuFile, submenuEdit, NULL );

    Ihandle *vbox = IupVbox( canvas, hbox, NULL );

    //Cria dialogo.
    _dialog = IupDialog( vbox );

    //atributos do dialogo
    IupSetAttribute( _dialog, IUP_TITLE, "Projeto Final" );
    IupSetAttribute( button2, "TIP", "Fechar programa" );

    IupSetAttribute( canvas, IUP_RASTERSIZE, "1024x768" );

    //inclui menu no dialogo
    IupSetHandle( "menu", menuMain );
    IupSetAttribute( _dialog, "MENU", "menu" );


    //Criar forma de passagem de valores para callbacks
    // IupSetAttribute( _dialog, "porFace", ( char* ) porFace );

    IupSetAttribute( _dialog, "_THIS", ( char* ) this );
    IupSetAttribute( _dialog, "_CANVAS", ( char* ) canvas );
    IupSetAttribute( _dialog, IUP_MARGIN, "10x10" );


    //definir callbacks
    IupSetCallback( button2, IUP_ACTION, ( Icallback ) exitButton );
    IupSetCallback( canvas, IUP_ACTION, ( Icallback ) renderScene );
    IupSetCallback( canvas, "RESIZE_CB", ( Icallback ) reshapeCallback );
    IupSetCallback( canvas, "KEYPRESS_CB", ( Icallback ) keyboardCallback );
    IupSetCallback( canvas, "MOTION_CB", ( Icallback ) mouseMotionCallback );
    IupSetCallback( canvas, "BUTTON_CB", ( Icallback ) mouseInicialCallback );

    IupSetCallback( itemAbrir, IUP_ACTION, ( Icallback ) itemAbrirCallback );
    IupSetCallback( itemPrint, IUP_ACTION, ( Icallback ) itemPrintCallback );
    IupSetCallback( itemSair, IUP_ACTION, ( Icallback ) itemSairCallback );
    IupSetCallback( itemWireframe, IUP_ACTION, ( Icallback ) itemWireframeCallback );
    IupSetCallback( itemPreferences, IUP_ACTION, ( Icallback ) itemPreferencesCallback );


    IupMap( _dialog );
    IupGLMakeCurrent( canvas );

    initCanvas( );
}



void IupWindow::computePlan( Vetor3D <double> frustumPoints[], int p1, int p2, int p3, int plane )
{
    //Calcula vetores
    Vetor3D<double> v1;
    v1 = frustumPoints[p3] - frustumPoints[p1];
    Vetor3D<double> v2;
    v2 = frustumPoints[p2] - frustumPoints[p1];

    //Realiza produto vetorial para encontrar a normal do plano.
    Vetor3D<double> normal;
    normal = v1 % v2;

    //Normaliza a normal para se obter uma metrica de distancia constante.
    normal.normalizaVet( );

    //Calcula a constante d do plano pelo produto escalar da normal com um ponto
    //do plano.
    double d;
    d = -( frustumPoints[p1].produtoEscalar( normal ) );

    //Salva o plano no vetor da classe.
    _frustumPlanes[plane][0] = normal.getX( );
    _frustumPlanes[plane][1] = normal.getY( );
    _frustumPlanes[plane][2] = normal.getZ( );
    _frustumPlanes[plane][3] = d;
}



void IupWindow::computeFrustumPlanes( Vetor3D<double> frustumPoints[] )
{
    //Plano near[0]: pontos[0,1,2]=> (2-0)x(1-0)
    computePlan( frustumPoints, 0, 1, 2, 0 );

    //Plano far[1]: [4,5,6]=> (5-4)X(6-4)
    computePlan( frustumPoints, 4, 6, 5, 1 );

    //Plano de baixo[2]: [0,1,4]=> (1-0)X(4-0)
    computePlan( frustumPoints, 0, 4, 1, 2 );

    //Plano de cima[3]: [2,3,6]=> (6-2)X(3-2)
    computePlan( frustumPoints, 2, 3, 6, 3 );

    //Plano de esquerda[4]: [0, 2, 4]=> (4-0)X(2-0)
    computePlan( frustumPoints, 0, 2, 4, 4 );

    //Plano de direita[5]: [1, 3, 5]=> (3-1)X(5-1)
    computePlan( frustumPoints, 1, 5, 3, 5 );
}



void IupWindow::computeFrustumPlanes( )
{
    //Recupera o viewport para obrerm as dimensoes da janela.
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );

    //Recupera as dimensoes da janela.
    int width = viewport[2];
    int height = viewport[3];

    //Valores de z do cubo projetado.
    double z1 = 0.0, z2 = 1.0;

    //Recupera as matriz de projecao e modelview.
    GLdouble modelViewDouble[16], projectionDouble[16];
    glGetDoublev( GL_MODELVIEW_MATRIX, modelViewDouble );
    glGetDoublev( GL_PROJECTION_MATRIX, projectionDouble );

    //Declara vetor para armazenar pontos do frustum.
    Vetor3D <double> frustumPoints[8];

    //Ponto 1 : inferior esquerdo near.
    gluUnProject( 0, 0, z1, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[0]._x, &frustumPoints[0]._y, &frustumPoints[0]._z );

    //Ponto 2: inferio direito near.
    gluUnProject( width, 0, z1, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[1]._x, &frustumPoints[1]._y, &frustumPoints[1]._z );

    //Ponto 3: superior esquerdo near.
    gluUnProject( 0, height, z1, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[2]._x, &frustumPoints[2]._y, &frustumPoints[2]._z );


    //Ponto 4: superior direito, near.
    gluUnProject( width, height, z1, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[3]._x, &frustumPoints[3]._y, &frustumPoints[3]._z );


    //Ponto 5: inferior esquerdo  far.
    gluUnProject( 0, 0, z2, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[4]._x, &frustumPoints[4]._y, &frustumPoints[4]._z );

    //Ponto 6: inferior direito far.
    gluUnProject( width, 0, z2, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[5]._x, &frustumPoints[5]._y, &frustumPoints[5]._z );

    //Ponto 7: superior esquedo far.
    gluUnProject( 0, height, z2, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[6]._x, &frustumPoints[6]._y, &frustumPoints[6]._z );


    //Ponto 8: superior direito far.
    gluUnProject( width, height, z2, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[7]._x, &frustumPoints[7]._y, &frustumPoints[7]._z );

    //Calcula as equacoes dos planos.
    computeFrustumPlanes( frustumPoints );
}


