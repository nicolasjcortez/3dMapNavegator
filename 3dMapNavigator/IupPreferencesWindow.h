/* 
 * File:   IupPreferencesWindow.h
 * Author: treinamento
 *
 * Created on September 26, 2014, 2:11 PM
 */

#ifndef IUPPREFERENCESWINDOW_H
#define	IUPPREFERENCESWINDOW_H

//#include <Windows.h>
#include <iup/iup.h>
#include "IupWindow.h"
#include <GL/gl.h>

class IupPreferencesWindow  {
public:
    
    static IupPreferencesWindow* getInstance(); 
	void setWindow(IupWindow* w);
    void show();
    void hide();
    int getTypeSurf();
    int getTypeColorSurf();
    int getNumDim();
    float getErroTol();
    float getColorBackR();
    float getColorBackG();
    float getColorBackB();
    float getColorSurfR();
    float getColorSurfG();
    float getColorSurfB();


    
private:
    static IupPreferencesWindow* instance;   
    
    Ihandle* _dialog;
    void createWindow();
    static int tFullSurfaceCallback( Ihandle *ih, int state );
    static int tLodCallback( Ihandle *ih, int state );  
    static int tLodViewCullingCallback( Ihandle *ih, int state ); 
    static int tOneColorCallback( Ihandle *ih, int state ); 
    static int tHeightsCallback( Ihandle *ih, int state );
    static int tTextureCallback( Ihandle *ih, int state ); 
    static int tDoisDCallback( Ihandle *ih, int state );
    static int tTresDCallback( Ihandle *ih, int state ); 
    static int spinboxCallback( Ihandle *ih, int inc );  
    static int buttonOkCallback(Ihandle* ih);
    static int buttonResetCallback(Ihandle* ih);
    static int buttonColorSurfCallback(Ihandle* ih);
    static int buttonColorBackCallback(Ihandle* ih);
    //coordenadas dos pontos que geram o desenho 
    int _typeSurf;
    int _typeColorSurf;
    int _numDim;
    float _erroTol;
    float _colorSurf[3];//sera uma cor no futuro(tipo vector) 
    float _colorBack[3];//sera uma cor no futuro (tipo vector)
    IupWindow* _mainWindow;
    IupPreferencesWindow();
    ~IupPreferencesWindow();
};

#endif	/* IUPPREFERENCESWINDOW_H */


