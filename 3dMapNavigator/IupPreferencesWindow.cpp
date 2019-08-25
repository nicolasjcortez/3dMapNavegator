/* 
 * File:   IupPreferencesWindow.cpp
 * Author: treinamento
 * 
 * Created on September 26, 2014, 2:11 PM
 */

#include "IupPreferencesWindow.h"
#include "IupWindow.h"
#include <cstdlib>
#include <iup/iupgl.h>
#include <iup/iup.h>
#include <GL/gl.h>
#include <cstdlib>
#include <GL/glu.h>
#include<cmath>
#include <cstdio>
#include <clocale>
//#include <Windows.h>
#include <vector>
#include <iostream>
using namespace std;

IupPreferencesWindow*  IupPreferencesWindow::instance = NULL;

IupPreferencesWindow::IupPreferencesWindow()
{
    _typeSurf = 1;
    _typeColorSurf = 1;
    _numDim = 3;
    _erroTol = 1;
    _colorSurf[0] = 1;
	_colorSurf[1] = 0;
	_colorSurf[2] = 0;
    _colorBack[0] = 0.5;
	_colorBack[1] = 0.5;
	_colorBack[2] = 0.5;   
    this->createWindow( );
}

void IupPreferencesWindow::setWindow(IupWindow* w)

{
	_mainWindow = w;
}

IupPreferencesWindow*  IupPreferencesWindow::getInstance()
{
        if (!instance) instance = new IupPreferencesWindow();
        return instance;
}




void IupPreferencesWindow::show( )
{
    IupShow( _dialog );
}



void IupPreferencesWindow::hide( )
{
    IupHide( _dialog );
}

int IupPreferencesWindow::getTypeSurf()
{
    return _typeSurf;
}


int IupPreferencesWindow::getTypeColorSurf()
{
    return _typeColorSurf;
}


int IupPreferencesWindow::getNumDim()
{
    return _numDim;
}


float IupPreferencesWindow::getErroTol()
{
    return _erroTol;
}


float IupPreferencesWindow::getColorSurfR()
{
    return _colorSurf[0];
}



float IupPreferencesWindow::getColorSurfG()
{
    return _colorSurf[1];
}


float IupPreferencesWindow::getColorSurfB()
{
    return _colorSurf[2];
}



float IupPreferencesWindow::getColorBackR()
{
    return _colorBack[0];
}



float IupPreferencesWindow::getColorBackG()
{
    return _colorBack[1];
}


float IupPreferencesWindow::getColorBackB()
{
    return _colorBack[2];
}


IupPreferencesWindow::~IupPreferencesWindow( )
{
}


int IupPreferencesWindow::tFullSurfaceCallback( Ihandle * ih, int state )
{

    IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
    if (state == 1)
    {
        window->_typeSurf = 1;
    }
    
    //Marca o canvas para ser redesenhado·
    window->_mainWindow->updateCanvas();
    
    return IUP_DEFAULT;
}



int IupPreferencesWindow::tLodCallback( Ihandle * ih, int state )
{

    IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
    if (state == 1)
    {
        window->_typeSurf = 2;
    }
    
    //Marca o canvas para ser redesenhado·
    window->_mainWindow->updateCanvas();
    return IUP_DEFAULT;
}



int IupPreferencesWindow::tLodViewCullingCallback( Ihandle * ih, int state )
{

    IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
    if (state == 1)
    {
        window->_typeSurf = 3;
    }
    
    //Marca o canvas para ser redesenhado·
    window->_mainWindow->updateCanvas();
    return IUP_DEFAULT;
}


int IupPreferencesWindow::tOneColorCallback( Ihandle * ih, int state )
{

    IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
    if (state == 1)
    {
        window->_typeColorSurf = 1;
    }

    //Marca o canvas para ser redesenhado·
    window->_mainWindow->updateCanvas();
    return IUP_DEFAULT;
}


int IupPreferencesWindow::tHeightsCallback( Ihandle * ih, int state )
{

    IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
    if (state == 1)
    {
        window->_typeColorSurf = 2;
    }

    //Marca o canvas para ser redesenhado·
    window->_mainWindow->updateCanvas();
    
    return IUP_DEFAULT;
}


int IupPreferencesWindow::tTextureCallback( Ihandle * ih, int state )
{

    IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
    if (state == 1)
    {
        window->_typeColorSurf = 3;
    }

    //Marca o canvas para ser redesenhado·
    window->_mainWindow->updateCanvas();
    
    return IUP_DEFAULT;
}


int IupPreferencesWindow::tDoisDCallback( Ihandle * ih, int state )
{

    IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
    if (state == 1)
    {
        window->_numDim = 2;
    }

    //Marca o canvas para ser redesenhado·
    window->_mainWindow->updateCanvas();
    
    return IUP_DEFAULT;
}


int IupPreferencesWindow::tTresDCallback( Ihandle * ih, int state )
{

    IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
    if (state == 1)
    {
        window->_numDim = 3;
    }
    
    //Marca o canvas para ser redesenhado·
    window->_mainWindow->updateCanvas();
    
    return IUP_DEFAULT;
}


int IupPreferencesWindow::buttonOkCallback( Ihandle * ih )
{
	IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
	window->hide();
	return IUP_DEFAULT;
}

int IupPreferencesWindow::buttonResetCallback( Ihandle * ih )
{
	IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
	Ihandle* t1 = ( Ihandle* ) IupGetAttribute( ih, "t1" );
	Ihandle* t2 = ( Ihandle* ) IupGetAttribute( ih, "t2" );
	Ihandle* t3 = ( Ihandle* ) IupGetAttribute( ih, "t3" );
	Ihandle* textBox1 = ( Ihandle* ) IupGetAttribute( ih, "textBox1" );
	window->_typeSurf = 1;
    window->_typeColorSurf = 1;
    window->_numDim = 3;
    window->_erroTol = 1;
    window->_colorSurf[0] = 1;
	window->_colorSurf[1] = 0;
	window->_colorSurf[2] = 0;
    window->_colorBack[0] = 0.5;
	window->_colorBack[1] = 0.5;
	window->_colorBack[2] = 0.5;
	IupSetAttribute(t1,"VALUE","ON" );
	IupSetAttribute(t2,"VALUE","ON" );
	IupSetAttribute(t3,"VALUE","ON" );
    IupSetStrAttribute(textBox1,"VALUE","1.0");
    window->_mainWindow->updateCanvas();
	return IUP_DEFAULT;
}

int IupPreferencesWindow::buttonColorSurfCallback( Ihandle * ih )
{
	Ihandle* dlg = IupColorDlg();
	IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );

	IupSetAttribute(dlg, "TITLE", "Escolha a cor da Superficie");

	IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);

	if (IupGetInt(dlg, "STATUS"))
	{
		char *colors = IupGetAttribute(dlg, "VALUE");
		float R,G,B;
		sscanf (colors,"%f %f %f",&R,&G,&B);

	  	window->_colorSurf[0] = R/255;
		window->_colorSurf[1] = G/255;
		window->_colorSurf[2] = B/255;
	  
	}
	else
	  printf("CANCEL\n");
    window->_mainWindow->updateCanvas();
	IupDestroy(dlg);
	return IUP_DEFAULT;

}


int IupPreferencesWindow::buttonColorBackCallback( Ihandle * ih )
{
	Ihandle* dlg = IupColorDlg();
	IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );

	IupSetAttribute(dlg, "TITLE", "Escolha a cor da Superficie");

	IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);

	if (IupGetInt(dlg, "STATUS"))
	{
		char *colors = IupGetAttribute(dlg, "VALUE");
		float R,G,B;
		sscanf (colors,"%f %f %f",&R,&G,&B);

	  	window->_colorBack[0] = R/255;
		window->_colorBack[1] = G/255;
		window->_colorBack[2] = B/255;
	}
	else
	  printf("CANCEL\n"); 
	window->_mainWindow->updateCanvas();
	IupDestroy(dlg);
	return IUP_DEFAULT;

}

int IupPreferencesWindow::spinboxCallback( Ihandle * ih, int inc )
{
    Ihandle* textBox1=(Ihandle*) IupGetAttribute(ih,"textBox1");
    IupPreferencesWindow* window = ( IupPreferencesWindow* ) IupGetAttribute( ih, "_THIS" );
    float i = IupGetFloat(textBox1,"VALUE");
    if(i>0 || (i==0 && inc>0)){
        i=i+(inc*0.1);
    }
    window->_erroTol = i;
    char value[1024];
    sprintf(value,"%0.1f",i);
    IupSetStrAttribute(textBox1,"VALUE",value);
    window->_mainWindow->updateCanvas();
    return IUP_DEFAULT; 
}



void IupPreferencesWindow::createWindow( )
{



    Ihandle *tFullSurface = IupToggle( "Toda a Superficie    ", NULL );
    Ihandle *tLod = IupToggle( "LOD     ", NULL );
    Ihandle *tLodViewCulling = IupToggle( "LOD + View Culling   ", NULL );
    Ihandle *togglehbox1 = IupHbox( tFullSurface, tLod, tLodViewCulling, NULL );
    Ihandle *radio1 = IupRadio( togglehbox1 );
    
    Ihandle *tOneColor = IupToggle( "Cor Solida    ", NULL );
    Ihandle *tHeights = IupToggle( "Alturas     ", NULL );
    Ihandle *tTexture = IupToggle( "Textura 2D   ", NULL );
    Ihandle *togglehbox2 = IupHbox( tOneColor, tHeights, tTexture, NULL );
    Ihandle *radio2 = IupRadio( togglehbox2 );
    
    Ihandle *tDoisD = IupToggle( "2D          ", NULL );
    Ihandle *tTresD = IupToggle( "3D           ", NULL );
    Ihandle *togglehbox3 = IupHbox(  tTresD,tDoisD, NULL );
    Ihandle *radio3 = IupRadio( togglehbox3 );
    
    
    Ihandle *buttonOk = IupButton( "OK", NULL );
    Ihandle *buttonReset = IupButton( "Reset", NULL );
    Ihandle *buttonColorSurf = IupButton( "Cor da Superficie", NULL );
    Ihandle *buttonColorBack = IupButton( "Cor do fundo", NULL );
    


    //Cria caixa de texto.
    Ihandle *textBox1 = IupText( NULL );

    
    //cria spin box
    Ihandle* spin = IupSpinbox( textBox1 ); 


    //Cria composicoes.
    Ihandle *hbox1 = IupHbox( radio1, NULL );
    Ihandle *hbox2 = IupHbox(  radio2, NULL );
    Ihandle *hbox3 = IupHbox(  radio3, NULL );
    Ihandle *hbox4 = IupHbox(  spin ,NULL );
    Ihandle *hbox5 = IupHbox(  buttonColorSurf,buttonColorBack ,NULL );
    Ihandle *hbox6 = IupHbox( IupFill(), buttonOk ,buttonReset,NULL );

    //Cria frame
    Ihandle* frame1 = IupFrame( hbox1 );
    Ihandle* frame2 = IupFrame( hbox2 );
    Ihandle* frame3 = IupFrame( hbox3 );
    Ihandle* frame4 = IupFrame( hbox4 );
    Ihandle* frame5 = IupFrame( hbox5 );

    
    Ihandle *vbox = IupVbox( frame1, frame2,frame3,frame4,frame5, hbox6, NULL );
   

    
    
    //Cria dialogo.
    _dialog = IupDialog( vbox );

  //atributos do dialogo
    IupSetAttribute( _dialog,"TITLE","Janela de Preferencias" );
 

    //atributos das caixas de texto
    IupSetAttribute(textBox1,"MASK",IUP_MASK_FLOAT);
    IupSetAttribute(textBox1,"VALUE","1.0" );
    IupSetAttribute(vbox,"MARGIN","10x10");
    IupSetAttribute(vbox,"GAP","10");


    //atributos dos frames
    IupSetAttribute(frame1,"TITLE","Tipo da Superficie:" );
    IupSetAttribute(frame2,IUP_TITLE,"Padrao de Cor:" );
    IupSetAttribute(frame3,IUP_TITLE,"Numero De Dimensoes:" );
    IupSetAttribute(frame4,IUP_TITLE,"Tolerancia de Erro:" );
    IupSetAttribute(frame5,IUP_TITLE,"Selecao de Cor:" );
    IupSetAttribute(frame2,"SIZE","230x30" );
    IupSetAttribute(frame1,"SIZE","230x30" );
    IupSetAttribute(frame3,"SIZE","230x30" );
    IupSetAttribute(frame4,"SIZE","230x30" );
    IupSetAttribute(frame5,"SIZE","230x30" );


    //Criar forma de passagem de valores para callbacks
    IupSetAttribute(_dialog,"textBox1",(char*)textBox1);
    IupSetAttribute( _dialog, "_THIS", ( char* ) this );
    IupSetAttribute(_dialog,"t1",(char*)tFullSurface);
	IupSetAttribute(_dialog,"t2",(char*)tOneColor);
	IupSetAttribute(_dialog,"t3",(char*)tTresD);


    //definir callbacks
    
    
    IupSetCallback( tFullSurface, IUP_ACTION, ( Icallback ) tFullSurfaceCallback );
    IupSetCallback( tLod, IUP_ACTION, ( Icallback ) tLodCallback );
    IupSetCallback( tLodViewCulling, IUP_ACTION, ( Icallback ) tLodViewCullingCallback );
    IupSetCallback( tOneColor, IUP_ACTION, ( Icallback ) tOneColorCallback );
    IupSetCallback( tHeights, IUP_ACTION, ( Icallback ) tHeightsCallback );
    IupSetCallback( tTexture, IUP_ACTION, ( Icallback ) tTextureCallback );
    IupSetCallback( tDoisD, IUP_ACTION, ( Icallback ) tDoisDCallback );
    IupSetCallback( tTresD, IUP_ACTION, ( Icallback ) tTresDCallback );   
    IupSetCallback(spin,"SPIN_CB",(Icallback)spinboxCallback);
	IupSetCallback( buttonOk, IUP_ACTION, ( Icallback ) buttonOkCallback );  
	IupSetCallback( buttonReset, IUP_ACTION, ( Icallback ) buttonResetCallback );   
	IupSetCallback( buttonColorSurf, IUP_ACTION, ( Icallback ) buttonColorSurfCallback );   
	IupSetCallback( buttonColorBack, IUP_ACTION, ( Icallback ) buttonColorBackCallback );   

    IupMap( _dialog );

}
   






