#ifndef LIBS
#define LIBS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#endif

#ifndef CONSTS
#define CONSTS

#define DUMMYCHARLENS 64
#define FONTSIZE 16

#endif

#ifndef STRUCTS
#define STRUCTS

typedef struct {
	int x;
	int y;
} Axis;

typedef struct {
	bool drawhelp;
    float scroll;
    int posx;
    int posy;
} Config;

typedef struct {
    bool quit;
    bool update;
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Surface * screenSurface;
	int charx;
	int chary;
    int * screenx;
    int * screeny;
    char bufferchord[DUMMYCHARLENS];
    Config * config;
    SDL_Color colors[16]; // esto sirve para hacer el dibujado de los distintos tableros
} Capochess;

typedef struct{
    bool infx;
    bool infy;
    bool canCapture;
    bool finalmovement;
    int x;
    int y;
} Movement;

// TODO: Añadir cooldown en tiempo y en movimientos
typedef struct {
    bool toProtect;         //p
    bool useForCastle;      //c
    bool canCastle;         //C
    bool enPassant;         //e
    bool canUseEnpassant;   //E
    bool canPromote;        //P
    bool isCaptured;
} Pieceflags;


typedef struct {
    bool myturn;
    bool lost;
    char mypieces;
    int movex; // ??? Para que esto???
    int movey;
} Player;

typedef struct {
    char * descriptor;
    char * name;
    Movement * movement;
	int movements;
    Pieceflags * pieceflags;
    // TODO: cargar iconos bmp.
} Pieces;

// TODO: Unificar Blocks con Positions
typedef struct {
    char block;
    Pieces * piece;
	int playerpiece;
    int posx;
    int posy;
} Positions;

typedef struct{
    char name;
    int color;
} Blocks;


typedef struct {
    bool file;
    int tablex;
    int tabley;
    Pieces *pieces;
    char ** table;
    Blocks ** tablee;
    Positions ** positions;
	Player * players;
    // TODO: Diseñar modos de juegos además del check mate.
} Game;

#endif

#ifndef FUNCS
#define FUNCS
void CloseChess();
void CleanChess(Capochess capochess, Game * game);
bool initChess(Capochess * capochess);
bool GetErrors(Capochess *capochess);
// Parse {
char * getPieceName(const char * def,const int pos);
char ** getTable(char * actline, FILE * file);
Movement * getPieceMovement(const char * def,const int pos);
Game processCC(char *filename);
int getPieceMovements(Movement * movements);
Pieceflags * getPieceFlags(const char * def,const int i);
Positions ** initPositions(char ** table);
Pieces * setPiece(const char def, Pieces * pieces,int numberPieces);
// Parse }
Pieceflags * CopyPieceflags(Pieces piece);
Movement * CopyMovements(Pieces piece);
int getNumberOfMovements(Movement * movements);
bool isValidMovement(Positions ** positions,Axis set, Axis move,int tablex, int tabley,int playerset, int totalplayers);
bool initChess(Capochess * capochess);
void updateChess(Capochess *capochess, Game * game, int mousex, int mousey);
void DrawBoard(Capochess * capochess, Game * game);
void DrawUI(Capochess * capochess, Game * game, int mousex, int mousey);
Blocks ** setGameTable(char ** table, int tablex, int tabley);
char * getTextPieceinPosition(Capochess * capochess, Game * game,int mousex, int mousey);
int getBlockPosition(Capochess * capochess,Game * game,int mousex, int mousey,int isposx);
Axis grabPiece(Capochess * capochess, Game * game,int savemovex1,int savemovey1);
void movePiece(Capochess * capochess, Game * game,Axis selected, int savemovex2,int savemovey2);

#endif
