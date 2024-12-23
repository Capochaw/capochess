#include "libs.h"

bool cabort = false;

int main(int argc, char *argv[]){
    Capochess capochess;
    Game game = processCC(argv[1]);
	int savex1, savey1, savex2, savey2;
	int savemovex1, savemovey1,savemovex2, savemovey2;
	bool mousel_held, isselected = false;
	Axis selected;
    // printf("Game loaded\n");

    if(GetErrors(&capochess)){
        return 1;
    }

    SDL_Event e;
    SDL_StartTextInput();

	capochess.update = true;
    while(capochess.quit == false && game.file == true){ // && cabort == false){
    // while(capochess.quit == false){
		const Uint8 *state = SDL_GetKeyboardState(NULL);
        while(SDL_PollEvent(&e)){
            switch (e.type){
                case SDL_QUIT:
                    capochess.quit = true;
                    break;
				case SDL_MOUSEWHEEL: 
                // Detecta si la tecla Ctrl está presionada
                if (state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL]) {
					capochess.update = true;
                    if (e.wheel.y > 0) {
                        capochess.config->scroll += 0.1;
                    } else if (e.wheel.y < 0) {
                        capochess.config->scroll -= 0.1;
                        
                    }
                }
				break;
				case SDL_WINDOWEVENT:
                    switch (e.window.event) {
                        case SDL_WINDOWEVENT_EXPOSED:
                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                        case SDL_WINDOWEVENT_RESIZED:
                        case SDL_WINDOWEVENT_SHOWN:
                            capochess.update = true; // Marcar para redibujar
                            break;
					}
				
                case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT) {
                        mousel_held = true; // Se presionó el botón izquierdo
						SDL_GetMouseState(&savemovex1,&savemovey1);
						selected = grabPiece(&capochess, &game, savemovex1,savemovey1);
						isselected = true;
						capochess.update = true;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        mousel_held = false; // Se soltó el botón izquierdo
						capochess.update = true;
                    }
                    break;
						//capochess.config->posx = capochess.config->posx - (savex-e.button.x);
						//capochess.config->posy = capochess.config->posy - (savey-e.button.y);
            }
        }
		SDL_GetMouseState(&savex1,&savey1);
       	if (state[SDL_SCANCODE_LCTRL] && mousel_held) {
       	    capochess.config->posx += (savex1 - savex2);
       	    capochess.config->posy += (savey1 - savey2);
			capochess.update = true;
       	} else if (mousel_held){
			SDL_GetMouseState(&savemovex2,&savemovey2);
			// evaluar si la pieza se puede mover. Si se puede mover, mover la pieza.
		}
		if(isselected && !mousel_held){
			movePiece(&capochess, &game,selected,savemovex2,savemovey2);
			isselected = false;
		}
		if(state[SDL_SCANCODE_TAB]){
			capochess.config->drawhelp = true;
			capochess.update = true;
		} else {
        	updateChess(&capochess,&game,savex1, savey1);
			capochess.config->drawhelp = false;
		}
		SDL_GetMouseState(&savex2,&savey2);
        usleep(12000);
		if(capochess.update == true){
        	updateChess(&capochess,&game,savex1, savey1);
			capochess.update = false;
		}
    }
    CleanChess(capochess, &game);

    return 0;
}
