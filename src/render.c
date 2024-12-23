#include "libs.h"

extern TTF_Font * mainfont;
extern SDL_Color Colors[16];
extern SDL_Color Players[16];

void RenderDrawColor(SDL_Renderer * renderer,SDL_Color color){
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
}

bool GetErrors(Capochess *capochess){
	if(initChess(capochess)){
        return 1;
    }

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("ErrorSDL: %s\n",SDL_GetError());
		return true;
	}
	capochess->window = SDL_CreateWindow("Capochess",SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 1000,1000,SDL_WINDOW_SHOWN);
			// 800,
			// 600,
			// SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	capochess->renderer = SDL_CreateRenderer(capochess->window,-1,SDL_RENDERER_ACCELERATED);
	if(capochess->window == NULL || capochess->renderer == NULL || TTF_Init() == -1){
		printf("ErrorSDL: %s\n",SDL_GetError());
		return true;
	} else {
		mainfont = TTF_OpenFont("font.ttf",FONTSIZE);
		SDL_Color set = {0,0,0,0};
		SDL_Surface * chartest = TTF_RenderText_Solid(mainfont,"a",set);
		capochess->charx = chartest->w;
		capochess->chary = chartest->h;
		SDL_FreeSurface(chartest);
		capochess->screenSurface = SDL_GetWindowSurface(capochess->window);
		SDL_GetWindowSize(capochess->window, capochess->screenx, capochess->screeny);
		SDL_FillRect(capochess->screenSurface,NULL,
			SDL_MapRGB(capochess->screenSurface->format,0,0,0));
		SDL_UpdateWindowSurface(capochess->window);
	}
	return false;
}

bool initChess(Capochess * capochess){
    capochess->quit = false;
    capochess->screenx = malloc(sizeof(int));
    capochess->screeny = malloc(sizeof(int));
    capochess->config = malloc(sizeof(Config));
    capochess->config->scroll = 1.5;
    capochess->config->posx = 0;
    capochess->config->posy = 0;
    return 0;
}

void DrawPiece(SDL_Renderer * renderer, Game * game, int placex, int placey,int sizeblock, int posx, int posy,int charx, int chary,float factor){
	char text[2];
	if(game->positions[placey][placex].piece != NULL){
		text[0] = game->positions[placey][placex].piece->descriptor[game->positions[placey][placex].playerpiece];
		text[1] = '\0';
	} else {
		return;
	}
	int ptype = game->positions[placey][placex].playerpiece; //game->positions[placex][placey].piecetype;
	SDL_Surface * scharone = TTF_RenderText_Solid(mainfont,text,Players[2*ptype]);
	SDL_Surface * schartwo = TTF_RenderText_Solid(mainfont,text,Players[2*ptype+1]);
	SDL_Texture * charone = SDL_CreateTextureFromSurface(renderer,scharone);
	SDL_Texture * chartwo = SDL_CreateTextureFromSurface(renderer,schartwo);
	SDL_FreeSurface(scharone);
	SDL_FreeSurface(schartwo);
	int setx1 = posx +(sizeblock-factor*charx)/2;
	int sety1 = posy +(sizeblock-factor*chary)/2;
	SDL_Rect drect = {setx1,sety1,factor*charx,factor*chary};
	SDL_RenderCopy(renderer,charone,NULL,&drect);
	drect.x = setx1-2;
	drect.y = sety1-2;
	SDL_RenderCopy(renderer,chartwo,NULL,&drect);
	SDL_DestroyTexture(charone);
	SDL_DestroyTexture(chartwo);
}

void DrawBoard(Capochess * capochess, Game * game){
	float blocksize;
    SDL_Rect block;
    int step = 0;
    for(int i = 0; i < game->tabley; i++){
        for(int p = 0; p < game->tablex; p++){
			blocksize = 50*capochess->config->scroll;
			block.w = blocksize;
			block.h = blocksize;
            block.x = ((*capochess->screenx-(blocksize*game->tablex))/2)+ capochess->config->posx + p*blocksize;
            block.y = ((*capochess->screeny-(blocksize*game->tabley))/2)+ capochess->config->posy + i*blocksize;
            RenderDrawColor(capochess->renderer,Colors[1+game->tablee[i][p].color]);
            SDL_RenderFillRect(capochess->renderer,&block);
			DrawPiece(capochess->renderer, game,p,i,blocksize,block.x,block.y,capochess->charx, capochess->chary,3*capochess->config->scroll);
            step++;
        }
    }
    RenderDrawColor(capochess->renderer,Colors[0]);
}
void DrawUI(Capochess * capochess, Game * game, int mousex, int mousey){
	// Dibujar las piezas comidas, puntos a favor y movimientos posibles

	if(capochess->config->drawhelp){
		int offset = 10;
		char * test = getTextPieceinPosition(capochess,game,mousex,mousey);
		SDL_Surface * shelptext = TTF_RenderText_Solid(mainfont,test,Colors[3]);
		SDL_Texture * helptext = SDL_CreateTextureFromSurface(capochess->renderer,shelptext);
		SDL_FreeSurface(shelptext);
		RenderDrawColor(capochess->renderer,Colors[3]);
		SDL_Rect pieceinfo = {offset +mousex+4,offset +mousey +4,30+capochess->charx * 2*strlen(test),40};
    	SDL_RenderFillRect(capochess->renderer,&pieceinfo);
		pieceinfo.x = pieceinfo.x+2;
		pieceinfo.y = pieceinfo.y+2;
		pieceinfo.w = pieceinfo.w - 4;
		pieceinfo.h = pieceinfo.h - 4;
		RenderDrawColor(capochess->renderer,Colors[4]);
    	SDL_RenderFillRect(capochess->renderer,&pieceinfo);
    	RenderDrawColor(capochess->renderer,Colors[0]);
		SDL_Rect drect = {offset+ mousex + capochess->charx * 2, offset+mousey + (16-capochess->chary/2),2 * capochess->charx * (strlen(test)), 2 * capochess->chary};
		SDL_RenderCopy(capochess->renderer,helptext,NULL,&drect);
		SDL_DestroyTexture(helptext);
		free(test);
	} else {
    	RenderDrawColor(capochess->renderer,Colors[0]);
	}
}

void updateChess(Capochess *capochess, Game * game, int mousex, int mousey){
    SDL_RenderClear(capochess->renderer);
	SDL_GetWindowSize(capochess->window, capochess->screenx, capochess->screeny);
    DrawBoard(capochess,game);
    DrawUI(capochess,game, mousex, mousey);
    SDL_RenderPresent(capochess->renderer);
}
