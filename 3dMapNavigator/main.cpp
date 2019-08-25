/* 
 * File:   main.cpp
 * Author: treinamento
 *
 * Created on September 26, 2014, 9:57 AM
 */

#include <iup/iup.h>
#include <iup/iupgl.h>
#include "IupWindow.h"
#include "IupPreferencesWindow.h"
using namespace std;
//#include <Windows.h>

int main(int argc, char** argv) {
    //Inicializa IUP.
    IupOpen(&argc, &argv);
    IupGLCanvasOpen();
    //    

    IupWindow* window = new IupWindow();

	IupPreferencesWindow::getInstance()->setWindow(window);
    window->show();

    //Coloca a IUP em loop.
    IupMainLoop();

    //Fecha a IUP e libera os espacos alocados.
    IupClose();
    
    delete window;


    return 0;
}