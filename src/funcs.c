#include "libs.h"

extern bool cabort;
char blocks[16] = "";

int max(int a, int b){
	if(a>b){
		return a;
	} else {
		return b;
	}
}

Pieceflags * getPieceFlags(const char * def,const int i){
    Pieceflags * ret = malloc(sizeof(Pieceflags));
    ret->toProtect = false;
    ret->canPromote = false;
    ret->useForCastle = false;
    ret->canCastle = false;
    ret->enPassant = false;
    ret->canUseEnpassant = false;
    ret->isCaptured = false;
    int k = 0;
    bool set = false;
    while(def[k] != '\0' && def[k] != '}'){
        if(def[k] == '{' || set == true){
            set = true;
            switch(def[k]){
                case 'p':
                    ret->toProtect = true;
                    break;
                case 'P':
                    ret->canPromote = true;
                    break;
                case 'c':
                    ret->useForCastle = true;
                    break;
                case 'C':
                    ret->canCastle = true;
                    break;
                case 'E':
                    ret->canUseEnpassant = true;
                    break;
            }
        }
        k++;
    }
    return ret;
}

char * getPieceName(const char * def,const int pos){
    int i = 0, set = 0, step = 0;
    while(def[step] != ' '){
        step++;
    }
    while(def[step+set+1] != ':'){
        set++;
    }
    char * ret = malloc(sizeof(char) * set);
    while(def[i+step+1] != ':'){
        ret[i] = def[i+step+1];
        i++;
        if(def[i+step+1] == '\n' || def[i+step+1] == EOF){
            printf("! -> in piece number %d, you missed ':'.\n",pos+1);
            CloseChess();
        }
    }
    ret[i] = '\0';
    return ret;
}

char * getPieceDescriptor(const char * def, const int i){
    int set = 0;
    for(int p = 0; p < strlen(def) && def[p] != ' '; p++){
        set++;
    }
    char * ret = malloc(sizeof(char) * (set+1));
    for(int i = 0; i < set+1; i++){
        ret[i] = def[i];
    }
    ret[set] = '\0';
    return ret;
}

// TODO: Que parsee las capturas para cada uno de los jugadores.
Movement * getPieceMovement(const char * def,const int pos){
    char pars[128];
    int n = 0;

    // verificación de sintaxis correcta -- {
    for(int i = 0; i < strlen(def);i++){
        if(def[i] == '(' || def[i] == ')'){
            pars[n] = def[i];
            pars[n+1] = '\0';
            n++;
        }
    }
    // printf("%s\n",pars); //DEBUG
    if(strlen(pars) %2 != 0){
        printf("! -> bad parenthesis assignation on piece %d.\n",pos+1);
        CloseChess();
        return NULL;
    }
    int set = (n-1)/2 + 1;
    while(n > 0){
        //printf("%d, %c\n",n-1, pars[n-1]); //DEBUG
        if((n-1) % 2 == 0){
            if(pars[n-1] != '(' && pars[n-2] != ')'){
                printf("! -> bad parenthesis assignation on piece %d.\n",pos+1);
                CloseChess();
            }
        } else {
            if(pars[n-1] != ')' && pars[n-2] != '('){
                printf("! -> bad parenthesis assignation on piece %d.\n",pos+1);
                CloseChess();
            }
        }
        n = n-1;
        pars[n] = '\0';
    }

    if(pars[0] != '\0'){
        printf("! -> bad parenthesis assignation on piece %d.\n",pos+1);
        CloseChess();
    }
    // verificación de sintaxis correcta -- }
    // seteo de movimiento al ret -- {
    bool intuple = false, canCapture = false;
    Movement * ret = malloc(sizeof(Movement) * (set+1));
    
    n = 0;
    int step = 0;
    for(int i = 2; i <strlen(def) || def[i] == EOF;i++){
        intuple = false;
        if(def[i] == '(' || def[i] == ','|| (def[i] == ' ' && intuple)){
            intuple = true;
            while(def[i + n+1] != ')' && def [i + n+1] != ','){
                pars[n] = def[i + n+1];
                n = n + 1;
            }
            pars[n] = '\0';
            ret[step].canCapture = canCapture;
            if(strstr(pars,"inf") != NULL){
                if(def[i + n+1] == ','){
                    ret[step].infx = true;
					pars[n-3] = '\0';
                } else {
                    ret[step].infy = true;
					pars[n-3] = '\0';
                }
            }
            if(def[i + n+1] == ','){
                ret[step].x = atoi(pars);
            } else {
                ret[step].y = atoi(pars);
            }
            ret[step].finalmovement = false;
            strcpy(pars,"");
            if(def[i+n+1] == ')')
                step = step+1;
            n = 0;
			pars[n] = '\0';
            intuple = false;
        }
        if(def[i+n+1] == ';'){
            canCapture = true;
        }
    }
    // seteo de movimiento al ret -- }
    ret[step].finalmovement = true;
	

    return ret;
}

// SET1
Positions ** initPositions(char ** table){
    int w = 0, l = 0;
    while(table[l] != NULL){
        l++;
    }
    for(int i = 0; i < l; i++){
        for(int p = 0; table[i][p] != '\0';p++){
            if(p > w){
                w = p;
            }
        }
    }
    Positions ** ret = malloc(sizeof(Positions *) * l);
    for(int i = 0; i<l;i++){
        ret[i] = malloc(sizeof(Positions) * w);
    }
    for(int i = 0; i < l; i++){
        for(int p = 0; p< w; p++){
            ret[i][p].block = table[i][p];
            ret[i][p].piece = NULL;
            ret[i][p].posx = i;
            ret[i][p].posy = p;
        }
    }
    // Las posiciones son pos[y][x];

    return ret;
}

int putPlayer(Pieces piece,char def){
	int ret = 0;
	while(piece.descriptor[ret] != def && piece.descriptor[ret] != EOF){
		ret++;
	}	
	return ret;
}

// TODO: hacer funciones de escape de memoria correctos.
Game processCC(char *filename){
    Game game;
    game.table = NULL;
    game.pieces = NULL;
    char actline[256] = "";
    int l = 0, w = 0;
    FILE *file = fopen(filename,"r");

    if(file == NULL){
        printf("No file was loaded.\n");
        // fclose(file);
        game.file = false;
    } else {
        game.file = true;

        // Procesar tablero -- {
        // TODO: Implementar para que detecte tableros no cuadrados.
        fgets(actline, sizeof(actline),file); 
        while(strcmp(actline,"-0-\n") != 0 && (feof(file) == 0)){
            if(strncmp(actline,"//",2) != 0 || strncmp(actline,"\n",1) != 0){
                // printf("%s",actline); // DEBUG
                if(strlen(actline) > l)
                    l = strlen(actline);
                w++;
            }
            fgets(actline, sizeof(actline),file); 
            }
        game.table = malloc(sizeof(char *) * (w+1));
        rewind(file);
        fgets(actline, sizeof(actline),file); 
        for(int i = 0; i < w; i++){
            if(strncmp(actline,"//",2) != 0){
                game.table[i] = malloc(sizeof(char) * strlen(actline));
                strcpy(game.table[i],actline);
                // printf("%s",actline);
            }else {
                i--;
            }
            fgets(actline, sizeof(actline),file); 
        }
        game.table[w] = NULL;
        while(strcmp(actline,"-0-\n") != 0 && (feof(file) == 0)){
            fgets(actline, sizeof(actline),file); 
        }
        game.tablex = l-1;
        game.tabley = w;
        // printf("%d,%d\n",game->tablex,game->tabley); // DEBUG
        game.tablee = setGameTable(game.table,game.tabley, game.tablex);
        // Tablero procesado -- }
        // Procesar piezas -- {   
        w = 0;
        l = 0;
        fpos_t pos;
        fgetpos(file,&pos);
        fgets(actline, sizeof(actline),file); 
        while(strcmp(actline,"-0-\n") != 0 && (feof(file) == 0)){
            if(strncmp(actline,"//",2) != 0){
                if(strlen(actline) > l)
                    l = strlen(actline);
                w++;
            }
            fgets(actline, sizeof(actline),file); 
        }
        game.pieces = malloc(sizeof(Pieces)*w);
        fsetpos(file,&pos);
        for(int i = 0; i < w; i++){
            fgets(actline, sizeof(actline),file); 
            if(strncmp(actline,"//",2) != 0){
                game.pieces[i].descriptor = getPieceDescriptor(actline,i);
                game.pieces[i].name = getPieceName(actline,i);
                game.pieces[i].pieceflags = getPieceFlags(actline,i);
                game.pieces[i].movement = getPieceMovement(actline,i);
				game.pieces[i].movements = getPieceMovements(game.pieces[i].movement);
				printf("move: %d\n",game.pieces[i].movements);
            } else {
                i--;
            }
        }
        // printf("%s",actline); //DEBUG
        // Piezas procesadas -- }
        // Procesar posiciones -- {
        fgets(actline, sizeof(actline),file); 
        fgets(actline, sizeof(actline),file); 
        game.positions = initPositions(game.table);
        for(int i = 0; i < game.tabley;i++){
            for(int p = 0; p < game.tablex;p++){
                if(game.positions[i][p].block == actline[p]){
                    game.positions[i][p].piece = NULL;
    				game.positions[i][p].playerpiece = -1;
                } else {
                    game.positions[i][p].piece = setPiece(actline[p] , game.pieces,w);
    				for(int k = 0; k < w; k++){
    				    if(strchr(game.pieces[k].descriptor,actline[p])){
    				        game.positions[i][p].playerpiece = putPlayer(game.pieces[k],actline[p]);
    				        break;
    				    }
    				}
                }
            }
            fgets(actline, sizeof(actline),file); 
        }
        // Posiciones procesadas -- }
        fclose(file);
    }
    return game;
}

Axis grabPiece(Capochess * capochess, Game * game, int savemovex1,int savemovey1){
	Axis ret;
	int posx = getBlockPosition(capochess, game,savemovex1,savemovey1,1);
	int posy = getBlockPosition(capochess, game,savemovex1,savemovey1,0);
	ret.x = posx;
	ret.y = posy;
	// printf("%d, %d\n",posx, posy);
	return ret;
}
// FIXME: Para tableros grandes el programa falla
void movePiece(Capochess * capochess, Game * game, Axis selected,int savemovex2,int savemovey2){
	Axis exit;
	exit = grabPiece(capochess, game,savemovex2,savemovey2);
	bool isvalidmovement = isValidMovement(game->positions,selected,exit,game->tablex,game->tabley,0,1);
	if(isvalidmovement){
		game->positions[exit.y][exit.x].piece = game->positions[selected.y][selected.x].piece;
		game->positions[exit.y][exit.x].playerpiece = game->positions[selected.y][selected.x].playerpiece;
		game->positions[selected.y][selected.x].playerpiece = -1;
		// FIXME: Liberar memoria al ser movida la pieza.
		game->positions[selected.y][selected.x].piece = NULL;
		// Mas condiciones sobre el cambio del movimiento.
	}
}

int getBlocksColor(char blocks[16], char blockname){
    bool isnameinblock = false;
    int i = 0;
    while(blocks[i] != '\0'){
        if(blocks[i] == blockname){
            isnameinblock = true;
            break;
        }
        i++;
    }
    if(!isnameinblock){
        i = 0;
        while(blocks[i] != '\0'){
            i++;
        }
        blocks[i] = blockname;
        blocks[i+1] = '\0';
    }
    return i;
}

// TODO: Hacer que tablee sea table.
// Tanto el tablero como los bloques son pares ordenados (y,x)
Blocks ** setGameTable(char ** table, int tablex, int tabley){
    Blocks ** ret = malloc(sizeof(Blocks*) * (tablex));
    for(int i = 0; i < tablex; i++){
        ret[i] = malloc(sizeof(Blocks) * (tabley));
    }
    for(int i = 0; i < tablex; i++){
        for(int p = 0; p < tabley; p++){
            ret[i][p].name = table[i][p];
            ret[i][p].color = getBlocksColor(blocks,table[i][p]);

        }
    }
    return ret;
}

Pieceflags * CopyPieceflags(Pieces piece){
    Pieceflags * ret = malloc(sizeof(Pieceflags));
    if(piece.pieceflags->toProtect == true){
        ret->toProtect = true;
    } else{
        ret ->toProtect = false;
    }
    if(piece.pieceflags->useForCastle == true){
        ret->useForCastle = true;
    } else{
        ret ->useForCastle = false;
    }
    if(piece.pieceflags->canCastle == true){
        ret->canCastle = true;
    } else{
        ret ->canCastle = false;
    }
    if(piece.pieceflags->enPassant == true){
        ret->enPassant = true;
    } else{
        ret ->enPassant = false;
    }
    if(piece.pieceflags->canUseEnpassant == true){
        ret->canUseEnpassant = true;
    } else{
        ret ->canUseEnpassant = false;
    }
    if(piece.pieceflags->canPromote == true){
        ret->canPromote = true;
    } else{
        ret ->canPromote = false;
    }

    return ret;
}

Movement * CopyMovements(Pieces piece){
    int set = 0;
    for(int i = 0; piece.movement[i].finalmovement != true; i++){
        set = set + 1;
    }
    Movement * ret = malloc(sizeof(Movement) * set);
    for(int i = 0; i < set; i++){
        ret[i].infx = piece.movement[i].infx;
        ret[i].infy =  piece.movement[i].infy;
        ret[i].canCapture =  piece.movement[i].canCapture;
        ret[i].finalmovement = piece.movement[i].finalmovement;
        ret[i].x = piece.movement[i].x;
        ret[i].y = piece.movement[i].y;
    }

    return ret;
}


Pieces * setPiece(const char def, Pieces * pieces, int numberPieces){
    Pieces * ret = malloc(sizeof(Pieces));
    int set = 0;

    for(int k = 0; k < numberPieces; k++){
        if(strchr(pieces[k].descriptor,def)){
            set = k;
            break;
        }
    }
    ret->descriptor = malloc(sizeof(char)* strlen(pieces[set].descriptor));
    ret->name = malloc(sizeof(char)* strlen(pieces[set].name));
    strcpy(ret->descriptor,pieces[set].descriptor);
    strcpy(ret->name,pieces[set].name);
	ret->movements = pieces[set].movements;
    ret->pieceflags = CopyPieceflags(pieces[set]);
    ret->movement = CopyMovements(pieces[set]);
    return ret;
}


// TODO: Función que cierre el programa anulando cualquier otra acción que haya en el medio.
void CloseChess(){
    cabort = true;
}

int getPieceMovements(Movement * movements){
	int ret = 0;
	while(movements[ret].finalmovement != true){
		ret++;
	}
	return ret;
}

bool isValidMovement(Positions ** positions,Axis set, Axis move,int tablex, int tabley,int playerset, int totalplayers){
	bool movementexist = false, canmove = true;
	int pos = 0,existpiece = 0;
	int player= totalplayers-playerset;
	Axis count = {move.x-set.x,move.y-set.y};
	//printf("%s",positions[set.y][set.x].piece->name);
	printf("count: %d, %d  ñ\n",count.x,count.y);
	if(positions[set.y][set.x].piece == NULL){
		return false;
	}
	for(int i = 0; i < positions[set.y][set.x].piece->movements/2;i++){
		printf("movement: %d,%d\n",positions[set.y][set.x].piece->movement[player*i].x, positions[set.y][set.x].piece->movement[player*i].y );
		if(positions[set.y][set.x].piece->movement[pos].infx && positions[set.y][set.x].piece->movement[pos].infy){
			for(int p = 0;p<max(tablex,tabley);p++){
				if(p*positions[set.y][set.x].piece->movement[player*i].x == count.x && p*positions[set.y][set.x].piece->movement[player*i].y == count.y){
					movementexist = true;
					pos = i;
					break;
				}
			}
		} else if(positions[set.y][set.x].piece->movement[pos].infx && !positions[set.y][set.x].piece->movement[pos].infy){
			for(int p = 0;p<max(tablex,tabley);p++){
				if(p*positions[set.y][set.x].piece->movement[player*i].x == count.x && positions[set.y][set.x].piece->movement[player*i].y == count.y){
					movementexist = true;
					pos = i;
					break;
				}
			}
		} else if (!positions[set.y][set.x].piece->movement[pos].infx && positions[set.y][set.x].piece->movement[pos].infy){
			for(int p = 0;p<max(tablex,tabley);p++){
				if(positions[set.y][set.x].piece->movement[player*i].x == count.x && p*positions[set.y][set.x].piece->movement[player*i].y == count.y){
					movementexist = true;
					pos = i;
					break;
				}
			}
		} else {
			for(int p = 0;p<max(tablex,tabley);p++){
				if(positions[set.y][set.x].piece->movement[player*i].x == count.x && positions[set.y][set.x].piece->movement[player*i].y == count.y){
					movementexist = true;
					pos = i;
					break;
				}
			}
		}
	}

	printf("is: %d, %d\n",positions[set.y][set.x].piece->movement[pos].x, positions[set.y][set.x].piece->movement[pos].y);
		// Si hay pieza nuestra o no capturable, el movimiento no es valido.
	
	if(movementexist && canmove){
		return true;
	} else {
		return false;
	}
}

int getBlockPosition(Capochess * capochess,Game * game,int mousex, int mousey,int isposx){
	int ret = 0;
	float blocksize = 50*capochess->config->scroll;

	if(isposx){
		int initposx = (*capochess->screenx-(blocksize*game->tablex))/2 + capochess->config->posx;
		while(ret != game->tablex){
			if(mousex - (initposx +ret*blocksize) <blocksize &&
					mousex - (initposx +ret*blocksize) >=0){
				break;
			}
			ret++;
		}
	} else {
	int initposy = (*capochess->screeny-(blocksize*game->tablex))/2 + capochess->config->posy;
		while(ret != game->tabley){
			if(mousey - (initposy +ret*blocksize) <blocksize &&
					mousey - (initposy +ret*blocksize) >=0){
				break;
			}
			ret++;
		}
	}
	return ret;
}

char * getTextPieceinPosition(Capochess * capochess,Game * game,int mousex, int mousey){
	float blocksize = 50*capochess->config->scroll;
	int i = 0, p = 0;
	bool findx = false, findy = true;
	char * ret;
	int initposx = (*capochess->screenx-(blocksize*game->tablex))/2 + capochess->config->posx;
	int initposy = (*capochess->screeny-(blocksize*game->tablex))/2 + capochess->config->posy;
	
	while(i != game->tablex){
		if(mousex - (initposx +i*blocksize) <blocksize &&
				mousex - (initposx +i*blocksize) >=0){
			findx = true;
			break;
		}
		i++;
	}
	while(p != game->tabley){
		if(mousey - (initposy +p*blocksize) <blocksize &&
				mousey - (initposy +p*blocksize) >=0){
			findy = true;
			break;
		}
		p++;
	}
	// printf("%d,%d : %d,%d\n",mousex,mousey,i,p);
 

	if(i >= game->tablex || p >= game->tabley){
		findx = false;
		findy = false;
	}else if(game->positions[p][i].piece == NULL){
		findx = false;
		findy = false;
	}
	if(findx && findy){
		ret = malloc(sizeof(char) * strlen(game->positions[p][i].piece->name));
		strcpy(ret,game->positions[p][i].piece->name);
	} else {
		ret = malloc(sizeof(char) * 4);
		strcpy(ret,"???");
	}
	return ret;
}

// TODO: Funcion que limpie la memoria del juego
void CleanChess(Capochess capochess, Game * game){
}
