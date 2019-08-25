
void computePlan( const Point frustumPoints[], int p1, int p2, int p3, int plane )
{
    //Calcula vetores
    Vector3D<double> v1 = frustumPoints[p3] - frustumPoints[p1];
    Vector3D<double> v2 = frustumPoints[p2] - frustumPoints[p1];

    //Realiza produto vetorial para encontrar a normal do plano.
    Vector3D<double> normal = v1 % v2;

    //Normaliza a normal para se obter uma metrica de distancia constante.
    normal.normalize( );

    //Calcula a constante d do plano pelo produto escalar da normal com um ponto
    //do plano.
    double d = -( frustumPoints[p1].dot( normal ) );

    //Salva o plano no vetor da classe.
    _frustumPlanes[plane][0] = normal.x;
    _frustumPlanes[plane][1] = normal.y;
    _frustumPlanes[plane][2] = normal.z;
    _frustumPlanes[plane][3] = d;
}



void computeFrustumPlanes( const Point frustumPoints[] )
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



void computeFrustumPlanes( )
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
    Point frustumPoints[8];

    //Ponto 1 : inferior esquerdo near.
    gluUnProject( 0, 0, z1, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[0].x, &frustumPoints[0].y, &frustumPoints[0].z );

    //Ponto 2: inferio direito near.
    gluUnProject( width, 0, z1, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[1].x, &frustumPoints[1].y, &frustumPoints[1].z );

    //Ponto 3: superior esquerdo near.
    gluUnProject( 0, height, z1, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[2].x, &frustumPoints[2].y, &frustumPoints[2].z );


    //Ponto 4: superior direito, near.
    gluUnProject( width, height, z1, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[3].x, &frustumPoints[3].y, &frustumPoints[3].z );


    //Ponto 5: inferior esquerdo  far.
    gluUnProject( 0, 0, z2, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[4].x, &frustumPoints[4].y, &frustumPoints[4].z );

    //Ponto 6: inferior direito far.
    gluUnProject( width, 0, z2, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[5].x, &frustumPoints[5].y, &frustumPoints[5].z );

    //Ponto 7: superior esquedo far.
    gluUnProject( 0, height, z2, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[6].x, &frustumPoints[6].y, &frustumPoints[6].z );


    //Ponto 8: superior direito far.
    gluUnProject( width, height, z2, modelViewDouble,
                  projectionDouble, viewport, &frustumPoints[7].x, &frustumPoints[7].y, &frustumPoints[7].z );

    //Calcula as equacoes dos planos.
    computeFrustumPlanes( frustumPoints );
}
