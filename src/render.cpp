#include "inc.h"

TTF_Font * mainfont;

SDL_Color charcolors[3] = {{0,0,0},{255,255,255}};
SDL_Color colors[10] = { {189, 177, 145}, {117, 113, 100},{255,0,0}, {171, 184, 138}, {138, 177, 184}, {171, 138, 184}, {184, 138, 157}, {161, 82, 64}, {230, 170, 96}, {245, 241, 174} };

void MyRenderDrawColor(SDL_Renderer * renderer,SDL_Color color){
	SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
}

bool initChess(Capochess * capochess){
    capochess->quit = false;
    capochess->config.scroll = 1;
    capochess->config.posx = 0;
    capochess->config.posy = 0;
	capochess->gameval = 0;
    return 0;
}

bool GetErrors(Capochess *capochess){
	if(initChess(capochess)){
        return 1;
    }

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "ErrorSDL: " << SDL_GetError() << "\n";
		return true;
	}
	capochess->window = SDL_CreateWindow("Capochess",SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 1000,1000,SDL_WINDOW_SHOWN);
			// 800,
			// 600,
			// SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	capochess->renderer = SDL_CreateRenderer(capochess->window,-1,SDL_RENDERER_ACCELERATED);
	if(capochess->window == NULL || capochess->renderer == NULL || TTF_Init() == -1){
		std::cout << "ErrorSDL: " << SDL_GetError() << "\n";
		return true;
	} else {
		mainfont = TTF_OpenFont("font.ttf",FONTSIZE+24);
		if(mainfont == NULL){
			std::cout << "ERROR!!! No font loaded.\n";
		}
		SDL_Color set = {0,0,0,0};
		SDL_Surface * chartest = TTF_RenderText_Solid(mainfont,"a",set);
		capochess->charx = chartest->w;
		capochess->chary = chartest->h;
		SDL_FreeSurface(chartest);
		capochess->screenSurface = SDL_GetWindowSurface(capochess->window);
		SDL_GetWindowSize(capochess->window, &capochess->screenx, &capochess->screeny);
		SDL_FillRect(capochess->screenSurface,NULL,
			SDL_MapRGB(capochess->screenSurface->format,0,0,0));
		SDL_UpdateWindowSurface(capochess->window);
	}
	return false;
}

int hasColorGetDrawed(const char test, std::vector<char> values){
	for(int i = 0; i < values.size(); i++){
		if(values[i] == test){
			return i;
		}
	}
	return -1;
}

char * getPiecechar(const std::string set){
	char * ret = (char*)malloc(sizeof(2));
	ret[1] = '\0';
	ret[0] = set[0];

	return ret;
}

void DrawGame(Capochess capochess){
	int blocksize = 100 * capochess.config.scroll;
	int color = 0,player = 0;
	int posx = capochess.config.posx;
	int posy = capochess.config.posx;
	int dx = 0, dy = 0;

	std::vector<char> blocks;
	std::string piece;
	char* piecechar;

	SDL_Rect rect;
	SDL_Rect imgrect;

	SDL_Surface * img;
	SDL_Texture * texture;
	if(capochess.gameval == -1){
		return;
	}

	SDL_RenderClear(capochess.renderer);
	for(int i = 0 ; i < capochess.game.tabley; i++){
		for(int p = 0 ; p < capochess.game.tablex; p++){
			if(capochess.game.blocks[p][i].isUsable){
				rect.x = posx + dx;
				rect.y = posy + dy;
				rect.w = blocksize;
				rect.h = blocksize;
				int evalblock = hasColorGetDrawed(capochess.game.blocks[p][i].block,blocks);
				player = capochess.game.positions[p][i].playerpiece;
				piece = capochess.game.positions[p][i].piece.descriptor;
				if(evalblock == -1){
					blocks.push_back(capochess.game.blocks[p][i].block);
					color = blocks.size()-1;
				} else {
					color = evalblock;
				}
				piecechar = getPiecechar(piece);
					if(capochess.game.positions[p][i].piece.pieceflags.toProtect && capochess.game.positions[p][i].piece.pieceflags.isCheck){
						MyRenderDrawColor(capochess.renderer,colors[3]);
						SDL_RenderFillRect(capochess.renderer,&rect);
					} else {
						MyRenderDrawColor(capochess.renderer,colors[color]);
						SDL_RenderFillRect(capochess.renderer,&rect);
					}
				if(capochess.game.positions[p][i].haspiece){
					img = TTF_RenderUTF8_Solid(mainfont,piecechar,charcolors[player]);
					if(img != NULL){
						imgrect.x = posx+dx + (img->w)/2;
						imgrect.y = posy+dy + (img->h)/2;
						imgrect.w = img->w;
						imgrect.h = img->h;
						texture = SDL_CreateTextureFromSurface(capochess.renderer,img);
						SDL_RenderCopy(capochess.renderer,texture,NULL,&imgrect);
						SDL_DestroyTexture(texture);
					}
					SDL_FreeSurface(img);
				}
			}
			dx = dx + blocksize;
		}
		dx = 0;
		dy = dy + blocksize;
	}
	SDL_RenderPresent(capochess.renderer);
}
