#include "inc.h"

int main(int argc, char *argv[]){
	Capochess capochess;

	int savex1, savey1, savex2, savey2;
	int dx, dy, movedx, movedy, movex,movey; // TODO: Generalizar a un vector ndimensional
	int savex, savey;
	int savemovex1, savemovey1,savemovex2, savemovey2;

	bool mousel_held, isselected = false;


	if(GetErrors(&capochess)){
		return 1;
	}

	capochess.config.posx = 100;
	capochess.config.posy = 100;

	SDL_Event e;
	SDL_StartTextInput();
	capochess.update = true;

	initGame(&capochess);
	while(!capochess.quit){
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
                        capochess.config.scroll += 0.1;
                    } else if (e.wheel.y < 0) {
                        capochess.config.scroll -= 0.1;
                        
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
						movex = (savemovex1-capochess.config.posx) / (int)(100*capochess.config.scroll);
						movey = (savemovey1-capochess.config.posy) / (int)(100*capochess.config.scroll);
						std::cout << movex << " " << movey << "\n";
						//selected = grabPiece(&capochess, &game, savemovex1,savemovey1);
						isselected = true;
						capochess.update = true;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (e.button.button == SDL_BUTTON_LEFT) {
						SDL_GetMouseState(&savemovex2,&savemovey2);
						movedx = (savemovex2-capochess.config.posx) / (int)(100*capochess.config.scroll);
						movedy = (savemovey2-capochess.config.posy) / (int)(100*capochess.config.scroll);
						MakeMove(&capochess.game,{movex,movey},{movedx,movedy});
						
                        mousel_held = false; // Se soltó el botón izquierdo
						capochess.update = true;
                    }
                    break;
            }
        }
		if(capochess.update){
			DrawGame(capochess);
			//DrawStatusline(capochess);
			//DrawHelp(capochess)
			capochess.update = false;
		}
	}
	return 0;
}
