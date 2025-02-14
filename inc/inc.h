#ifndef LIBS
#define LIBS

#include <iostream>
#include <string>
#include <vector>

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
	std::vector<int> move;
	std::vector<int> moved;
    int posx;
    int posy;
} Config;


typedef struct movement{
    bool infx;
    bool infy;
	bool mutualinf;
    bool canCapture;
	bool onlyforCapture;
	//TODO: Cambiar a que sea un vector n dimensional en funcion del juego.
    int x;
    int y;
	struct movement& operator=(const movement& val){
		if(this != &val){
			infx = val.infx;
			infy = val.infy;
			mutualinf = val.mutualinf;
			canCapture = val.canCapture;
			onlyforCapture = val.onlyforCapture;
			x = val.x;
			y = val.y;
		}
		return *this;
	}
} Movement;

// TODO: Añadir cooldown en tiempo y en movimientos
typedef struct pieceflags{
    bool toProtect;
    bool useForCastle;
    bool canCastle;
    bool enPassant;
    bool canUseEnpassant;
    bool canPromote;
	bool isCheck;
	struct pieceflags& operator=(const pieceflags& val){
		if(this != &val){
    		toProtect = val.toProtect;
    		useForCastle = val.useForCastle;
    		canCastle = val.canCastle;
    		enPassant = val.enPassant;
    		canUseEnpassant = val.canUseEnpassant;
    		canPromote = val.canPromote;
			isCheck = val.isCheck;
		}
		return *this;
	}

} Pieceflags;

// TODO: Agregar endflags
typedef struct {
} Endflags;

typedef struct pieces {
	std::string descriptor = "";
    std::string name = "";
	std::vector<std::vector<Movement>> movement;
    Pieceflags pieceflags;
	int colidenum = 1;
	int points = 0;
    // TODO: cargar iconos bmp.
} Pieces;

// TODO: Unificar Blocks con Positions
typedef struct {
    Pieces piece;
	bool haspiece;
	int playerpiece;
	bool isgrabbed = 0;
} Positions;

typedef struct {
	char block;
	bool isUsable;
} Blocks;

typedef struct {
	std::vector<std::vector<bool>> valid;
} validMovements;

typedef struct {
    bool myturn = 0;
    bool lost = 0;
	bool isCheck = 0;
	std::vector<Pieces> capturedPieces;
	std::vector<std::vector<validMovements>> validmovements;
	int points = 0;
} Player;

typedef struct {
	std::string name;
	std::string description;
	//TODO: Cambiar a que sea un vector n dimensional en funcion del juego.
    int tablex;
    int tabley;
	int player;

	Pieces emptypiece;
	
	std::vector<Pieces> pieces;
	std::vector<std::vector<Blocks>> blocks;
	std::vector<std::vector<Positions>> positions;
	std::vector<Player> players;

	Endflags endflags;
} Game;

typedef struct {
    bool quit;
    bool update;
	bool initgame;
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Surface * screenSurface;
	int charx;
	int chary;
    int screenx;
    int screeny;
	std::string bufferchord;

	int gameval;

	Game game;
	Config config;
    //SDL_Color colors[16]; // esto sirve para hacer el dibujado de los distintos tableros
} Capochess;


#endif

#ifndef FUNCS
#define FUNCS
bool GetErrors(Capochess *capochess);
void DrawGame(Capochess capochess);
void initGame(Capochess * capochess);
bool isValidMovement(Game game, int player, std::vector<int> pos, std::vector<int> movement);
bool isAttackingMovement(Game game, int player, std::vector<int> pos, std::vector<int> movement);
void MakeMove(Game * game, std::vector<int> pos, std::vector<int> movement);
void isinCheck(Game * game);

#endif
