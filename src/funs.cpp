#include "inc.h"

int max(int a, int b){
	if (a > b){
		return a;
	} else {
		return b;
	}
}

std::vector<std::vector<Blocks>> generateBlocks(Game game,std::string table){
	std::vector<std::vector<Blocks>> ret(game.tablex,std::vector<Blocks>(game.tabley));
	int p = 0, move = 0;
	for(int i = 0 ; i < table.size(); i++){
		if(table[i] == '\n'){
			p++;
			move = 0;
			continue;
		}
		ret[p][move].block = table[i];
		if(table[i] == 'n'){
			ret[p][move].isUsable = false;
		} else {
			ret[p][move].isUsable = true;
		}
		move++;
	}
	return ret;
};

std::vector<std::vector<Positions>> generatePositions(Game game,std::string table){
	std::vector<std::vector<Positions>> ret(game.tablex,std::vector<Positions>(game.tabley));
	int dx = 0;
	int dy = 0;
	int del = 0;
	for(int i = 0 ; i < table.size(); i++){
		dx = (i-del) % game.tablex;
		dy = (i-del) / game.tabley;
		if(table[i] == '\n'){
			del++;
			continue;
		}
		ret[dx][dy].playerpiece = -1;
		ret[dx][dy].haspiece = false;
		for(int p = 0; p < game.pieces.size(); p++){
			for(int q = 0 ; q < game.players.size(); q++){
				if(table[i] == game.pieces[p].descriptor[q]){
					ret[dx][dy].playerpiece = q;
					ret[dx][dy].piece = game.pieces[p];
					ret[dx][dy].haspiece = true;
				}
			}
		}
	}
	return ret;
}

void initGame(Capochess * capochess){
	capochess->game.emptypiece.name = "";
	capochess->game.emptypiece.descriptor = "";
	capochess->game.emptypiece.pieceflags = {false,false,false,false,false,false};
	capochess->game.emptypiece.colidenum = -1;
	

	switch(capochess->gameval){
		case 0:
			capochess->game.name = "Chess";
			capochess->game.description = "Traditional chess game.";
			capochess->game.tablex = 8;
			capochess->game.tabley = 8;
			capochess->config.move.resize(2);
			capochess->config.moved.resize(2);

			capochess->game.player = 1;
			capochess->game.players.resize(2);
			capochess->game.players[1].myturn = true;

			std::vector<std::vector<bool>> valid;
			valid.resize(capochess->game.tablex);
			for(int i = 0; i < capochess->game.tablex;i++){
				valid[i].resize(capochess->game.tabley);
				for(int p = 0; p < capochess->game.tabley; p++){
					valid[i][p] = false;
				}
			}

			capochess->game.players[0].validmovements.resize(capochess->game.tablex);
			capochess->game.players[1].validmovements.resize(capochess->game.tablex);
			for(int i = 0; i < capochess->game.tablex;i++){
				capochess->game.players[0].validmovements[i].resize(capochess->game.tabley);
				capochess->game.players[1].validmovements[i].resize(capochess->game.tabley);
			}
			for(int i = 0; i < capochess->game.tablex;i++){
				for(int p = 0; p < capochess->game.tabley; p++){
					capochess->game.players[0].validmovements[i][p].valid = valid;
					capochess->game.players[1].validmovements[i][p].valid = valid;
				}
			}

			capochess->game.blocks = generateBlocks(capochess->game,
					"09090909\n90909090\n09090909\n90909090\n09090909\n90909090\n09090909\n90909090");

			Pieces pawn;
			Pieces knight;
			Pieces bishop;
			Pieces rook;
			Pieces queen;
			Pieces king;

			pawn.descriptor = "pP";
			pawn.name = "pawn";
			pawn.points = 1;
			pawn.movement.resize(2);
			pawn.movement[0].push_back({false,false,false,false,false,0,1});
			pawn.movement[0].push_back({false,false,false,false,false,0,2});
			pawn.movement[0].push_back({false,false,false,true ,true,1,1});
			pawn.movement[0].push_back({false,false,false,true ,true,-1,1});
			pawn.movement[1].push_back({false,false,false,false,false,0,-1});
			pawn.movement[1].push_back({false,false,false,false,false,0,-2});
			pawn.movement[1].push_back({false,false,false,true ,true,1,-1});
			pawn.movement[1].push_back({false,false,false,true ,true,-1,-1});
			pawn.pieceflags = {false, false, false, false, true, true,false};

			knight.descriptor = "nN";
			knight.name = "Knight";
			knight.points = 3;
			knight.movement.resize(1);
			knight.movement[0].push_back({false,false,false,true,false,2,1});
			knight.movement[0].push_back({false,false,false,true,false,2,-1});
			knight.movement[0].push_back({false,false,false,true,false,-2,1});
			knight.movement[0].push_back({false,false,false,true,false,-2,-1});
			knight.movement[0].push_back({false,false,false,true,false,1,2});
			knight.movement[0].push_back({false,false,false,true,false,-1,2});
			knight.movement[0].push_back({false,false,false,true,false,1,-2});
			knight.movement[0].push_back({false,false,false,true,false,-1,-2});
			knight.pieceflags = {false,false,false,false,false,false,false};

			bishop.descriptor = "bB";
			bishop.name = "Bishop";
			bishop.points = 3;
			bishop.movement.resize(1);
			bishop.movement[0].push_back({false,false,true,true,false,1,1});
			bishop.movement[0].push_back({false,false,true,true,false,1,-1});
			bishop.movement[0].push_back({false,false,true,true,false,-1,1});
			bishop.movement[0].push_back({false,false,true,true,false,-1,-1});
			bishop.pieceflags = {false,false,false,false,false,false,false};

			rook.descriptor = "rR";
			rook.name = "Rook";
			rook.points = 5;
			rook.movement.resize(1);
			rook.movement[0].push_back({false,true,false,true,false,0,1});
			rook.movement[0].push_back({false,true,false,true,false,0,-1});
			rook.movement[0].push_back({true,false,false,true,false,1,0});
			rook.movement[0].push_back({true,false,false,true,false,-1,0});
			rook.pieceflags = {false,true,true,false,false,false,false};

			queen.descriptor = "qQ";
			queen.name = "queen";
			rook.points = 9;
			queen.movement.resize(1);
			queen.movement[0].push_back({false,true,false,true,false,0,1});
			queen.movement[0].push_back({false,true,false,true,false,0,-1});
			queen.movement[0].push_back({true,false,false,true,false,1,0});
			queen.movement[0].push_back({true,false,false,true,false,-1,0});
			queen.movement[0].push_back({false,false,true,true,false,1,1});
			queen.movement[0].push_back({false,false,true,true,false,1,-1});
			queen.movement[0].push_back({false,false,true,true,false,-1,1});
			queen.movement[0].push_back({false,false,true,true,false,-1,-1});
			queen.pieceflags = {false,false,false,false,false,false,false};

			king.descriptor = "kK";
			king.name = "king";
			king.points = -1;
			king.movement.resize(1);
			king.movement[0].push_back({false,false,false,true,false,0,1});
			king.movement[0].push_back({false,false,false,true,false,0,-1});
			king.movement[0].push_back({false,false,false,true,false,1,0});
			king.movement[0].push_back({false,false,false,true,false,-1,0});
			king.movement[0].push_back({false,false,false,true,false,1,1});
			king.movement[0].push_back({false,false,false,true,false,1,-1});
			king.movement[0].push_back({false,false,false,true,false,-1,1});
			king.movement[0].push_back({false,false,false,true,false,-1,-1});
			king.pieceflags = {true,false,true,false,false,false,false};

			
			capochess->game.pieces.push_back(pawn);
			capochess->game.pieces.push_back(knight);
			capochess->game.pieces.push_back(bishop);
			capochess->game.pieces.push_back(rook);
			capochess->game.pieces.push_back(queen);
			capochess->game.pieces.push_back(king);

			capochess->game.positions = generatePositions(capochess->game,
					"rnbqkbnr\npppppppp\n09090909\n90909090\n09090909\n90909090\nPPP9PPPP\nRNBQKBNR");
			break;
	}
}

bool isAttackingMovement(Game game, int player, std::vector<int> pos, std::vector<int> movement){
	int posx = pos[0], posy = pos[1], dx = movement[0], dy = movement[1], playerval;
	int ddx = movement[0] - pos[0], ddy = movement[1] - pos[1];
	int collision = 0;

	if(posx >= game.tablex || posy >= game.tabley || dx >= game.tablex || dy >= game.tabley){
		return false;
	}

	if(game.blocks[dx][dy].isUsable == false || game.positions[dx][dy].playerpiece == player){
		return false;
	}
	Pieces *movepiece = &game.positions[posx][posy].piece;
	if(movepiece == NULL){
		return false;
	}
	if(movepiece->movement.size() == 1){
		playerval = 0;
	} else {
		playerval = player;
	}
	Movement * move = nullptr;
	bool existmovement = false;

	//Si la pieza no es del jugador
	if(game.positions[posx][posy].playerpiece != player){
		return false;
	}

	for(int i = 0; i < movepiece->movement[playerval].size() && !existmovement; i++){
		// evaluar info de infx, infy y mutualinf.
		int movex = movepiece->movement[playerval][i].x;
		int movey = movepiece->movement[playerval][i].y;
		collision = 0;
		if(movepiece->movement[playerval][i].mutualinf){
			for(int q = 0; q < max(game.tablex,game.tabley); q++){
				if(posx + q*movex >= game.tablex || posy + q*movey >= game.tabley 
						|| posx + q*movex < 0 || posy + q*movey < 0){
					break;
				}
				if(q*movex == ddx && q*movey == ddy){
					existmovement = true;
					move = &movepiece->movement[playerval][i];
					break;
				} else {
					if(game.positions[posx+q*movex][posy+q*movey].haspiece){
						collision++;
					}
				}
			}
		} else if (movepiece->movement[playerval][i].infx) {
			for(int q = 0; q < game.tablex;q++){
				if(movepiece->movement[playerval][i].infy){
					for(int p = 0;p < game.tabley; p++){
						if(posx + q*movex >= game.tablex || posy + p*movey >= game.tabley 
								|| posx + q*movex < 0
								|| p*posy + movey < 0){
							break;
						}
						if(q*movex == ddx && p*movey == ddy){
							existmovement = true;
							move = &movepiece->movement[playerval][i];
							break;
						} else {
							if(game.positions[posx+q*movex][posy+p*movey].haspiece){
								collision++;
							}
						}
					}
				} else {
					if(posx + q*movex >= game.tablex 
							|| posy + movey >= game.tabley  ||
					posx + q*movex < 0 || posy + movey < 0){
						break;
					}
					if(q*movex == ddx && movey == ddy){
						existmovement = true;
						move = &movepiece->movement[playerval][i];
						break;
					} else {
						if(game.positions[posx+q*movex][posy+movey].haspiece){
							collision++;
						}
					}
				}
			}
		} else if (movepiece->movement[playerval][i].infy) {
			for(int q = 0; q < game.tabley;q++){
				if(movepiece->movement[playerval][i].infx){
					for(int p = 0;p < game.tablex; p++){
						if(posx + p*movex >= game.tablex
								|| posy + q*movey >= game.tabley
								|| posx + p*movex < 0 || q*posy + movey < 0){
							break;
						}
						if(p*movex == ddx && q*movey == ddy){
							existmovement = true;
							move = &movepiece->movement[playerval][i];
							break;
						} else {
							if(game.positions[posx+p*movex][posy+q*movey].haspiece){
								collision++;
							}
						}
					}
				} else {
					if(posx + movex >= game.tablex 
							|| posy + q*movey >= game.tabley 
							|| posx + movex < 0 || posy + q*movey < 0){
						break;
					}
					if(movex == ddx && q*movey == ddy){
						existmovement = true;
						move = &movepiece->movement[playerval][i];
						break;
					} else {
						if(game.positions[posx+movex][posy+q*movey].haspiece){
							collision++;
						}
					}
				}
			}
		} else if(ddx == movepiece->movement[playerval][i].x
				&& ddy == movepiece->movement[playerval][i].y){
			existmovement = true;
			move = &movepiece->movement[playerval][i];
			break;
		}
	}

	if(move == NULL)
		return false;
	if((game.positions[dx][dy].haspiece && !move->canCapture)
			|| (!game.positions[dx][dy].haspiece && move->onlyforCapture)
			|| collision > movepiece->colidenum){
		return false;
	}

	if(!existmovement){
		return false;
	}
	// NO tiene que ser un bloque inusable

	return true;
}

//FIXME: Optimizar agregando en la estructura de Movement un array que contenga todos los posibles movimientos
//para una pieza y comparar en función de este array si el movimiento es posible o no.
//FIXME: OPTIMIZAR
bool isValidMovement(Game game, int player, std::vector<int> pos, std::vector<int> movement){
	bool ret = true;

	//TODO: [Colisión de piezas]
	//		Promoción
	//		Enroque
	
	
	int posx = pos[0], posy = pos[1], dx = movement[0], dy = movement[1], playerval;
	int ddx = movement[0] - pos[0], ddy = movement[1] - pos[1];
	int collision = 0;

	if(posx >= game.tablex || posy >= game.tabley || dx >= game.tablex || dy >= game.tabley){
		return false;
	}

	if(game.blocks[dx][dy].isUsable == false
			|| game.positions[dx][dy].playerpiece == player
			|| game.positions[dx][dy].piece.pieceflags.toProtect == true){
		return false;
	}
	Pieces *movepiece = &game.positions[posx][posy].piece;
	if(movepiece == NULL){
		return false;
	}
	if(movepiece->movement.size() == 1){
		playerval = 0;
	} else {
		playerval = player;
	}
	Movement * move = nullptr;
	bool existmovement = false;

	//Si la pieza no es del jugador
	if(game.positions[posx][posy].playerpiece != player){
		return false;
	}

	for(int i = 0; i < movepiece->movement[playerval].size() && !existmovement; i++){
		// evaluar info de infx, infy y mutualinf.
		int movex = movepiece->movement[playerval][i].x;
		int movey = movepiece->movement[playerval][i].y;
		collision = 0;
		if(movepiece->movement[playerval][i].mutualinf){
			for(int q = 0; q < max(game.tablex,game.tabley); q++){
				if(posx + q*movex >= game.tablex || posy + q*movey >= game.tabley 
						|| posx + q*movex < 0 || posy + q*movey < 0){
					break;
				}
				if(q*movex == ddx && q*movey == ddy){
					existmovement = true;
					move = &movepiece->movement[playerval][i];
					break;
				} else {
					if(game.positions[posx+q*movex][posy+q*movey].haspiece){
						collision++;
					}
				}
			}
		} else if (movepiece->movement[playerval][i].infx) {
			for(int q = 0; q < game.tablex;q++){
				if(movepiece->movement[playerval][i].infy){
					for(int p = 0;p < game.tabley; p++){
						if(posx + q*movex >= game.tablex || posy + p*movey >= game.tabley 
								|| posx + q*movex < 0
								|| p*posy + movey < 0){
							break;
						}
						if(q*movex == ddx && p*movey == ddy){
							existmovement = true;
							move = &movepiece->movement[playerval][i];
							break;
						} else {
							if(game.positions[posx+q*movex][posy+p*movey].haspiece){
								collision++;
							}
						}
					}
				} else {
					if(posx + q*movex >= game.tablex 
							|| posy + movey >= game.tabley  ||
					posx + q*movex < 0 || posy + movey < 0){
						break;
					}
					if(q*movex == ddx && movey == ddy){
						existmovement = true;
						move = &movepiece->movement[playerval][i];
						break;
					} else {
						if(game.positions[posx+q*movex][posy+movey].haspiece){
							collision++;
						}
					}
				}
			}
		} else if (movepiece->movement[playerval][i].infy) {
			for(int q = 0; q < game.tabley;q++){
				if(movepiece->movement[playerval][i].infx){
					for(int p = 0;p < game.tablex; p++){
						if(posx + p*movex >= game.tablex
								|| posy + q*movey >= game.tabley
								|| posx + p*movex < 0 || q*posy + movey < 0){
							break;
						}
						if(p*movex == ddx && q*movey == ddy){
							existmovement = true;
							move = &movepiece->movement[playerval][i];
							break;
						} else {
							if(game.positions[posx+p*movex][posy+q*movey].haspiece){
								collision++;
							}
						}
					}
				} else {
					if(posx + movex >= game.tablex 
							|| posy + q*movey >= game.tabley 
							|| posx + movex < 0 || posy + q*movey < 0){
						break;
					}
					if(movex == ddx && q*movey == ddy){
						existmovement = true;
						move = &movepiece->movement[playerval][i];
						break;
					} else {
						if(game.positions[posx+movex][posy+q*movey].haspiece){
							collision++;
						}
					}
				}
			}
		} else if(ddx == movepiece->movement[playerval][i].x
				&& ddy == movepiece->movement[playerval][i].y){
			existmovement = true;
			move = &movepiece->movement[playerval][i];
			break;
		}
	}

	if(move == NULL)
		return false;
	if((game.positions[dx][dy].haspiece && !move->canCapture)
			|| (!game.positions[dx][dy].haspiece && move->onlyforCapture)
			|| collision > movepiece->colidenum){
		return false;
	}

	if(!existmovement){
		return false;
	}
	// NO tiene que ser un bloque inusable

	return ret;
}

void isinCheck(Game * game){
	for(int i = 0; i < game->players.size(); i++){
		game->players[i].isCheck = false;
	}
	for(int x = 0; x < game->tablex; x++){
		for(int y = 0; y < game->tabley; y++){
			if(!game->positions[x][y].piece.pieceflags.toProtect){
				continue;
			}
			game->positions[x][y].piece.pieceflags.isCheck = false;
			for(int q = 0; q < game->players.size(); q++){
				for(int xx = 0; xx < game->tablex; xx++){
					for(int yy = 0; yy < game->tabley; yy++){
						if(game->positions[xx][yy].haspiece && game->positions[xx][yy].playerpiece
								!= game->positions[x][y].playerpiece){
							if(isAttackingMovement(*game,q,{xx,yy},{x,y})){
								game->positions[x][y].piece.pieceflags.isCheck = true;
								game->players[game->positions[x][y].playerpiece].isCheck = true;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void setPiece(Pieces * set, Pieces * ret){
	ret->descriptor = set->descriptor;
	ret->name = set->name;
	ret->movement = set->movement;
	ret->pieceflags = set->pieceflags;
}

void MovePiece(Game * game,int player, std::vector<int> pos, std::vector<int> mpos){
	std::vector<int> dpos = {mpos[0]-pos[0],mpos[1]-pos[1]};
	if(game->positions[pos[0]][pos[1]].haspiece){
		game->players[game->positions[pos[0]][pos[1]].playerpiece].capturedPieces.push_back(game->positions[pos[0]][pos[1]].piece);
		game->positions[pos[0]][pos[1]].haspiece = false;
		game->positions[pos[0]][pos[1]].playerpiece = -1;
	}
	game->positions[pos[0]+dpos[0]][pos[1] + dpos[1]].haspiece = true;
	game->positions[pos[0]+dpos[0]][pos[1] + dpos[1]].playerpiece = player;
	game->positions[pos[0]+dpos[0]][pos[1] + dpos[1]].piece	= game->positions[pos[0]][pos[1]].piece;
	game->positions[pos[0]][pos[1]].piece = game->emptypiece;
}

void getPoints(Player * player){
	player->points = 0;
	for(int i = 0; i < player->capturedPieces.size(); i++){
		player->points += player->capturedPieces[i].points;
	}
	
}

void MakeMove(Game * game, std::vector<int> pos, std::vector<int> movement){
	//TODO: Hacer que tenga registro de la partida con notación.
	if(isValidMovement(*game,game->player, pos, movement)){
		MovePiece(game, game->player, pos, movement);
		isinCheck(game);
		if(game->players[game->player].isCheck){
			MovePiece(game, game->player, movement, pos);
			return;
		}
		getPoints(&game->players[game->player]);
		game->player = (game->player + 1) % game->players.size();
	}
}

bool EvaluateEndGame(){
	bool ret = false;
	return ret;
}

