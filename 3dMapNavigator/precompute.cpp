

void vertexPreCompute( int i, int j, int di, int dj, int n )
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



void meshPreCompute( unsigned int level )
{
    //Aloca espaco para raios e erros.
    _errors.clear( );
    _radius.clear( );
    _errors.resize( _dimensionX * _dimensionY );
    _radius.resize( _dimensionX * _dimensionY );

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

