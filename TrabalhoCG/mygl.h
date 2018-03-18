#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************


//******************************************************************************************************************
//Definindo um estrutura que receberá valores de uma cor e após isso definiremos o pixel da tela que receberá essa cor
//******************************************************************************************************************

//Definindo uma struct para o sistema RGBA
typedef struct{
float R;
float G;
float B;
float A;
}TypeColor;

//Partindo das cores, agora vamos achar um pixel na tela.
//Depois disso vamos definir qual a cor que esse pixel vai receber.
typedef struct{
int pX;
int pY;
TypeColor color;
}TypePixel;

//*******************************************************************************************************************

//Definimos aqui uma função que vai colorir um determinado pixel
void PutPixel(TypePixel p){

    int position = (4*p.pX)+(4*p.pY*IMAGE_WIDTH);
    FBptr[position+0] = p.color.R;
    FBptr[position+1] = p.color.G;
    FBptr[position+2] = p.color.B;
    FBptr[position+3] = p.color.A;

}

//Definimos aqui uma função que vai colorir uma linha na memória com o uso dos pixels
//que estão sendo coloridos com a função PutPixel, para isso foi preciso usar o algoritmo
//de Bresenham
void DrawLine(TypePixel p1, TypePixel p2){

    //Primeiro vamos calcular o Delta entre os pontos
    int dX = (p2.pX - p1.pX);
    int dY = (p2.pY - p1.pY);
    int nAux, x, y, tX = 1, tY = 1, cont;

    //Aqui é onde verificamos se os deltas são negativos e fazemos a correção
    if(dX < 0){
        tX = -tX;
    }
    if(dY < 0){
        tY = -tY;
    }

    dX = abs(dX);
    dY = abs(dY);

    //Agora vamos definir auxiliares para os pontos X e Y do Pixel inicial
    x = p1.pX;
    y = p1.pY;

    //Também definimos uma auxiliar que vai modificar a cor dos pixels
    TypePixel aux;
    TypeColor cR;

    //Aqui nós definimos a intensidade em que iremos fazer a interpolação das cores
    float   fR = (p2.color.R - p1.color.R), fG = (p2.color.G - p1.color.G),
            fB = (p2.color.B - p1.color.B), fA = (p2.color.A - p1.color.A);

    aux.color = p1.color;

    //----------------------------------------------

    if(dX > dY){
        nAux = (dY << 1) - dX;//Movemos então os bits uma vez para a esquerda e subtraimos o dX
        fR /= dX; fG /= dX; fB /= dX; fA /= dX;//Aqui é onde começa a interpolação das cores nas linhas
        for(cont = 0; cont < dX; cont++){
            //Interpolando as cores
            aux.color.R += fR;
            aux.color.G += fG;
            aux.color.B += fB;
            aux.color.A += fA;
            //---------------------
            aux.pX = x;
            aux.pY = y;
            PutPixel(aux);//Adicionando o novo pixel para formação da linha
            if(nAux < 0){
                nAux += (dY << 1);
            }else{
                y += tY;
                nAux += (dY - dX) << 1;
            }
            x += tX;
        }
    }else{
        nAux = (dX << 1) - dY;
        fR /= dY; fG /= dY; fB /= dY; fA /= dY;
        for(cont = 0; cont < dY; cont++){
            //------------------------
            aux.color.R += fR;
            aux.color.G += fG;
            aux.color.B += fB;
            aux.color.A += fA;
            //------------------------
            aux.pX = x;
            aux.pY = y;
            PutPixel(aux);
            if(nAux < 0){
                nAux += (dX << 1);
            }else{
                x += tX;
                nAux += (dX - dY) << 1;
            }
            y += tY;
        }
    }
}

/*
    Para fazer o preenchimento de um triângulo, foi preciso fazer uma modificação na DrawLine.
    Com o uso dessa função podemos assim criar triângulos coloridos.
*/
void DrawLineColor(TypePixel p1, TypePixel p2, TypePixel p3){

    //Primeiro vamos calcular o Delta entre os pontos
    int dX = (p2.pX - p1.pX);
    int dY = (p2.pY - p1.pY);
    int nAux, x, y, tX = 1, tY = 1, cont;

    if(dX < 0){
        tX = -tX;
    }
    if(dY < 0){
        tY = -tY;
    }

    dX = abs(dX);
    dY = abs(dY);

    //Agora vamos definir auxiliares para o pontos X e Y do Pixel inicial
    x = p1.pX;
    y = p1.pY;

    //Também definimos uma auxiliar que vai modificar a cor dos pixels
    TypePixel aux;
    TypeColor cR;

    //Aqui nós definimos a intensidade em que iremos fazer a interpolação das cores
    float   fR = (p2.color.R - p1.color.R), fG = (p2.color.G - p1.color.G),
            fB = (p2.color.B - p1.color.B), fA = (p2.color.A - p1.color.A);

    aux.color = p1.color;

    //----------------------------------------------

    if(dX > dY){
        nAux = (dY << 1) - dX;
        fR /= dX; fG /= dX; fB /= dX; fA /= dX;//Aqui é onde começa a interpolação das cores nas linhas
        for(cont = 0; cont < dX; cont++){
            //Interpolando as cores
            aux.color.R += fR;
            aux.color.G += fG;
            aux.color.B += fB;
            aux.color.A += fA;
            //---------------------
            aux.pX = x;
            aux.pY = y;
            PutPixel(aux);
            /*
                Agora temos o diferencial da DrawLineColor, onde ela chama uma nova linha
                do pixel atual que está sendo formado pela linha, até o terceiro pixel, que
                passamos como parâmetro
            */
            DrawLine(aux, p3);//Diferencial da DrawLineColor
            if(nAux < 0){
                nAux += (dY << 1);
            }else{
                y += tY;
                nAux += (dY - dX) << 1;
            }
            x += tX;
        }
    }else{
        nAux = (dX << 1) - dY;
        fR /= dY; fG /= dY; fB /= dY; fA /= dY;
        for(cont = 0; cont < dY; cont++){
            //------------------------
            aux.color.R += fR;
            aux.color.G += fG;
            aux.color.B += fB;
            aux.color.A += fA;
            //------------------------
            aux.pX = x;
            aux.pY = y;
            PutPixel(aux);
            DrawLine(aux, p3);//Diferencial da DrawLineColor
            if(nAux < 0){
                nAux += (dX << 1);
            }else{
                x += tX;
                nAux += (dX - dY) << 1;
            }
            y += tY;
        }
    }
}

//Definimos aqui uma função que faz a criação de triângulos na tela, chamando 3 DrawLine
void DrawTriangle(TypePixel p1, TypePixel p2, TypePixel p3){
    DrawLine(p1, p2);
    DrawLine(p2, p3);
    DrawLine(p3, p1);
}

//Definimos essa classe para colorir um triângulo com o uso da DrawLineColor, que é uma extensão da Drawline
void DrawTriangleColor(TypePixel p1, TypePixel p2, TypePixel p3){
    DrawLineColor(p1, p2, p3);
    DrawLineColor(p2, p3, p1);
    DrawLineColor(p3, p1, p2);
}

#endif // _MYGL_H_

