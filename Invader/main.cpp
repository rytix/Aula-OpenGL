#include <windows.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <list>
#include <time.h>
#include <cstdlib>

using namespace std;
// *** Definição da nave ***
//Constantes
const GLfloat H_NAVE = 30.0f;
const GLfloat W_NAVE = 20.0f;
const GLfloat X_STEP_NAVE = 10.0f;
const int TEMPO_RECARGA_NAVE = 20;
//Variaveis
struct Nave{
    GLfloat x;
    GLfloat y;
    int tempo_recarga;
};
// *** Definição do tiro ***
const GLfloat H_TIRO = 15.0f;
const GLfloat W_TIRO = 5.0f;
struct Tiro{
    GLfloat x;
    GLfloat y;
    bool tiro_do_jogador;
};
// *** Definição do Alien ***
//Constantes
const GLfloat W_ALIEN = 25.0f;
const GLfloat H_ALIEN = 15.0f;
const GLfloat ESPACAMENTO = 0.0f;
const int FRAMES_ALIEN = 2;
const int LINHAS_ALIEN = 8;
const int COLUNAS_ALIEN = 11;
const int TEMPO_RECARGA_ALIEN = 20;
//Desenhos Aliens
//Alien Normal
GLfloat ALIEN_1_FRAME[FRAMES_ALIEN][LINHAS_ALIEN][COLUNAS_ALIEN] =
{
    {
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,1,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,1,1,0,1,1,1,0,1,1,0},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,1,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,1,0,1},
    {0,0,0,1,1,0,1,1,0,0,0}
    },
    {
    {0,0,1,0,0,0,0,0,1,0,0},
    {0,0,0,1,0,0,0,1,0,0,0},
    {0,0,1,1,1,1,1,1,1,0,0},
    {0,1,1,0,1,1,1,0,1,1,0},
    {1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,1,1,1,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,1,0,1},
    {0,0,0,1,1,0,1,1,0,0,0}
    }
};
GLfloat ALIEN_1_RGB[FRAMES_ALIEN][3] = {{0,0,0},{0,0,0}};
//Variaveis
struct Alien{
    GLfloat x;
    GLfloat y;
    GLfloat frame[FRAMES_ALIEN][LINHAS_ALIEN][COLUNAS_ALIEN];
    GLfloat rgb[FRAMES_ALIEN][3];
    int tempo_recarga;
};
// *** FIM DE DEFINIÇÕES ***
// *** OPENGL E CABEÇALHO ***
// Largura e altura da janela
GLfloat windowWidth;
GLfloat windowHeight;
//Cabeçalho
void copiar_matriz_frames(GLfloat origem[FRAMES_ALIEN][LINHAS_ALIEN][COLUNAS_ALIEN], GLfloat destino[FRAMES_ALIEN][LINHAS_ALIEN][COLUNAS_ALIEN]);
void copiar_matriz_rgb(GLfloat origem[FRAMES_ALIEN][3],GLfloat destino[FRAMES_ALIEN][3]);
void desenhar_nave(Nave nave);
void desenhar_alien(Alien alien);
void desenhar_tiro(Tiro tiro);
void atirar(int x, int y, bool jogador_atirando);
// *** FIM DO OPENGL E CABEÇALHO ***
//Variaveis do Jogo
Nave nave;
list<Alien*> aliens;
list<Tiro*> tiros;
bool estado_reiniciar_jogo = true;
//Funções do jogo
void reiniciar_jogo(){
    if(estado_reiniciar_jogo){
        estado_reiniciar_jogo = false;
        nave.x = 100;
        nave.y = 20;
        nave.tempo_recarga = TEMPO_RECARGA_NAVE;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 6; j++){
                Alien *alien;
                alien = new Alien;
                alien->x = j*60 + 20;
                alien->y = 60*i + 180;
                copiar_matriz_frames(ALIEN_1_FRAME,alien->frame);
                copiar_matriz_rgb(ALIEN_1_RGB,alien->rgb);
                aliens.push_back(alien);
            }
        }
    }
}
void acao_tiros(){
    list<Tiro*>::iterator it = tiros.begin();
    while(it != tiros.end()){
        if((*it)->tiro_do_jogador){
            (*it)->y += 10;
        }else{
            (*it)->y -= 10;
        }
        if((*it)->y > windowHeight || (*it)->y < 0){
            Tiro *tiro = *it;
            tiros.erase(it++);
            delete tiro;
        }else{
            it++;
        }
    }
}
void acao_aliens(){
    list<Alien*>::iterator it = aliens.begin();
    while(it != aliens.end()){
        if(rand() % 100 < 1){ // Chance de atirar
            atirar((*it)->x+W_ALIEN/2 - W_TIRO/2, (*it)->y + H_ALIEN/2,false);
        }
        it++;
    }

}
void testar_colisao(){

}
void atirar(int x, int y, bool jogador_atirando){
    Tiro *tiro;
    tiro = new Tiro;
    tiro->x = x;
    tiro->y = y;
    tiro->tiro_do_jogador = jogador_atirando;
    tiros.push_back(tiro);
}
//*** OpenGL ***
//Desenhos
void desenhar_tudo(){
    desenhar_nave(nave);
    list<Alien*>::iterator it = aliens.begin();
    while(it != aliens.end()){
        desenhar_alien(**it++);
    }
    list<Tiro*>::iterator it2 = tiros.begin();
    while(it2 != tiros.end()){
        desenhar_tiro(**it2++);
    }
}
void desenhar_tiro(Tiro tiro){
    glBegin(GL_POLYGON);
    glVertex2f(tiro.x,tiro.y);
    glVertex2f(tiro.x+W_TIRO,tiro.y);
    glVertex2f(tiro.x+W_TIRO,tiro.y+H_TIRO);
    glVertex2f(tiro.x,tiro.y+H_TIRO);
    glEnd();
}
void desenhar_nave(Nave nave){
    glColor3f(0.5f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
        glVertex2i(nave.x,nave.y);
        glVertex2i(nave.x+W_NAVE,nave.y);
        glVertex2i(nave.x+W_NAVE/2,nave.y+H_NAVE);
    glEnd();
}
void desenhar_alien(Alien alien){
    GLfloat altura_pixel = H_ALIEN / LINHAS_ALIEN;
    GLfloat largura_pixel = W_ALIEN / COLUNAS_ALIEN;
    for(int i = 0; i < LINHAS_ALIEN; i++){
        for(int j = 0; j < COLUNAS_ALIEN; j++){
            if(alien.frame[0][i][j] == 1){
                glBegin(GL_POLYGON);
                glVertex2f(alien.x+j*largura_pixel,alien.y+(LINHAS_ALIEN-i)*altura_pixel);
                glVertex2f(alien.x+j*largura_pixel+largura_pixel,alien.y+(LINHAS_ALIEN-i)*altura_pixel);
                glVertex2f(alien.x+j*largura_pixel+largura_pixel,alien.y+(LINHAS_ALIEN-i)*altura_pixel+altura_pixel);
                glVertex2f(alien.x+j*largura_pixel,alien.y+(LINHAS_ALIEN-i)*altura_pixel+altura_pixel);
                glEnd();
            }
        }
    }
}
//Definições do OPENGL
void Timer(int value) //"Main Loop"
{
    reiniciar_jogo();
    acao_aliens();
    acao_tiros();
    //testar_colisao();
    // Redesenha o quadrado com as novas coordenadas
    glutPostRedisplay();
    glutTimerFunc(33,Timer, 1);
}

void LerTeclado(unsigned char key, int x, int y){
    switch (key) {
        case 'D':
        case 'd':
            nave.x += X_STEP_NAVE;
            break;
        case 'A':
        case 'a':
            nave.x -= X_STEP_NAVE;
            break;
        case 'W':
        case 'w':
            atirar(nave.x+W_NAVE/2-W_TIRO/2,nave.y+H_NAVE/2,true);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void Desenha(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Limpa a janela de visualização com a cor de fundo especificada
    glClear(GL_COLOR_BUFFER_BIT);

    desenhar_tudo();
    // Executa os comandos OpenGL
    glutSwapBuffers();
}

// Função callback chamada pela GLUT a cada intervalo de tempo
// (a window não está sendo redimensionada ou movida)

// Inicializa parâmetros de rendering
void Inicializa (void)
{
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.5f, 1.0f, 0.0f);
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Evita a divisao por zero
    if(h == 0) h = 1;

    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Estabelece a janela de seleção (left, right, bottom, top)
    if (w <= h)  {
        windowHeight = 350.0f*h/w;
        windowWidth = 350.0f;
    }
    else  {
        windowWidth = 350.0f*w/h;
        windowHeight = 350.0f;
    }

    gluOrtho2D(0.0f, windowWidth, 0.0f, windowHeight);
}

// Programa Principal
int main(int argc, char** argv)
{
    srand(time(NULL));
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInit(&argc, argv);
    glutInitWindowSize(550,500);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Space Invader");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(LerTeclado);
    glutTimerFunc(33, Timer, 1);
    Inicializa();
    glutMainLoop();
}
//HELPERS
void copiar_matriz_frames(GLfloat origem[FRAMES_ALIEN][LINHAS_ALIEN][COLUNAS_ALIEN], GLfloat destino[FRAMES_ALIEN][LINHAS_ALIEN][COLUNAS_ALIEN]){
    for(int i = 0; i < FRAMES_ALIEN; i++){
        for(int j = 0; j < LINHAS_ALIEN; j++){
            for(int z = 0; z < COLUNAS_ALIEN; z++){
                destino[i][j][z] = origem[i][j][z];
            }
        }
    }
}
void copiar_matriz_rgb(GLfloat origem[FRAMES_ALIEN][3],GLfloat destino[FRAMES_ALIEN][3]){
    for(int i = 0; i < FRAMES_ALIEN; i++){
        for(int j = 0; j < 3; j++){
            destino[i][j] = origem[i][j];
        }
    }
}
