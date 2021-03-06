#include <stdio.h>
#include <graphics.h>

// lets define the player struct
struct player {
    unsigned char orientation; // E(ast), W(est), N(orth), S(outh)
    unsigned short x;
    unsigned short y;
    unsigned short bombs;
    unsigned short life;
} players[2];

// defination for tiles
struct tile {
    unsigned char type;
    // B: Blank
    // O: Obastacle
    // D: Destroyable

    unsigned short top;
    unsigned short left;
} tiles[17][17];

// defination for bombs
struct bomb {
    unsigned short x;
    unsigned short y;
    float timeToExplode;
    unsigned short placed;
    unsigned short placedBy;
} bombs[6];

// some global variable declaration
unsigned short tileSize = 26;
unsigned short tileSizeHalf = 13;
unsigned short offset = 0;
unsigned short playerSize = 18;
unsigned short playerSizeHalf = 9;
unsigned short bombRadius = 6;
unsigned short explosionRadius = 4;
unsigned short gameState = 0;

// all functions definations
void setup ();
void movePlayer (unsigned short, char);
void placeBomb (unsigned short);
void drawBomb (unsigned short);
void explodeBomb (unsigned short);
void playerHit (unsigned short);
void changeTile (unsigned short, unsigned short);
void gameOver ();
void resultScreen ();

// run this function to initilize variable and load the level
void setup () {
    unsigned int color;
    unsigned short i, j;
    unsigned short top, bottom, left, right;

    // setup the level design
    char levelDesign[17][17] = {
    {'O',   'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O',   'O'},


    {'O',   'B', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'B',   'O'},
    {'O',   'B', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'B',   'O'},

    {'O',   'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D',   'O'},
    {'O',   'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B',   'O'},
    {'O',   'O', 'D', 'B', 'O', 'D', 'B', 'O', 'D', 'B', 'O', 'D', 'B', 'O', 'D', 'B',   'O'},
    {'O',   'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B',   'O'},
    {'O',   'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D',   'O'},

    {'O',   'O', 'O', 'O', 'O', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'O', 'O', 'O', 'O',   'O'},

    {'O',   'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D',   'O'},
    {'O',   'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B',   'O'},
    {'O',   'O', 'D', 'B', 'O', 'D', 'B', 'O', 'D', 'B', 'O', 'D', 'B', 'O', 'D', 'B',   'O'},
    {'O',   'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B',   'O'},
    {'O',   'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D',   'O'},

    {'O',   'B', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'D', 'O', 'B',   'O'},
    {'O',   'B', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D', 'B', 'B',   'O'},


    {'O',   'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O',   'O'}
    };

    // initialize the level
    for (i=0; i<17; i++) {
        for (j=0; j<17; j++) {
            // set fill style and boundry color
            if (levelDesign[i][j] == 'O') {
                color = WHITE;
            } else if (levelDesign[i][j] == 'B') {
                color = BLACK;
            } else if (levelDesign[i][j] == 'D') {
                color = DARKGRAY;
            }
            // set the type of tile
            tiles[i][j].type = levelDesign[i][j];

            setcolor(color);
            setfillstyle(1, color);

            // lets calc the coords
            top = offset + (i)*tileSize;
            bottom = offset + (i+1)*tileSize;

            left = offset + (j)*tileSize;
            right = offset + (j+1)*tileSize;

            // lets make a rectangle
            rectangle(top, left, bottom, right);
            floodfill(top+1, left+1, color);
        }
    }

    // initialize the bombs array
    for (i=0; i<6; ++i) {
        bombs[i].placed = 0;
    }

    // initialize the players
    players[0].x = 1;
    players[0].y = 1;
    players[1].x = 15;
    players[1].y = 15;

    // printf("players: %d %d %d %d\n", players[0].x,players[0].y,players[1].x,players[1].y);

    for (i=0; i<2; ++i) {
        if (0==i)
            color = BLUE;
        else
            color = RED;

        players[i].bombs = 3;
        players[i].life = 3;

        top = (tileSizeHalf - playerSizeHalf) + players[i].x*tileSize;
        left = (tileSizeHalf - playerSizeHalf) + players[i].x*tileSize;
        bottom = (tileSizeHalf + playerSizeHalf) + players[i].x*tileSize;
        right = (tileSizeHalf + playerSizeHalf) + players[i].x*tileSize;

        setcolor(color);
        rectangle(top, left, bottom , right);
        setfillstyle(1, color);
        floodfill(top+1, left+1, color);
    }
}

// move player identified by PlayerIndex in the given direction
void movePlayer (unsigned short PlayerIndex, char direction) {
    unsigned short xCord = players[PlayerIndex].x;
    unsigned short yCord = players[PlayerIndex].y;
    unsigned short xActual = tileSizeHalf + xCord*tileSize;
    unsigned short yActual = tileSizeHalf + yCord*tileSize;
    unsigned short xTop,yTop,xBottom,yBottom;
    unsigned int color;
    setfillstyle(1,BLACK);
    floodfill(xActual,yActual,BLACK);

    // printf("\nplayers: %d %d %d %d", players[0].x,players[0].y,players[1].x,players[1].y);
    // printf("\ntiles: %c %c %c %c %c", tiles[xCord][yCord].type, tiles[xCord+1][yCord].type, tiles[xCord-1][yCord].type, tiles[xCord][yCord+1].type, tiles[xCord][yCord-1].type);

    switch (direction) {
        case 'N':
            if(tiles[xCord-1][yCord].type=='B')
                xCord--;
            break;

        case 'E':
            if(tiles[xCord][yCord+1].type=='B')
                yCord++;
            break;

        case 'W':
            if(tiles[xCord][yCord-1].type=='B')
                yCord--;
            break;

        case 'S':
            if(tiles[xCord+1][yCord].type=='B')
                xCord++;
            break;

        case 'B':
            placeBomb(PlayerIndex);
        break;
    }

    players[PlayerIndex].x = xCord;
    players[PlayerIndex].y = yCord;

    if(PlayerIndex==0)
        color = BLUE;
    else
        color = RED;

    xTop = (tileSizeHalf - playerSizeHalf) + xCord*tileSize;
    yTop = (tileSizeHalf - playerSizeHalf) + yCord*tileSize;
    xBottom = (tileSizeHalf + playerSizeHalf) + xCord*tileSize;
    yBottom = (tileSizeHalf + playerSizeHalf) + yCord*tileSize;

    setcolor(color);
    rectangle(xTop,yTop,xBottom,yBottom);
    setfillstyle(1,color);
    floodfill(xTop+1,yTop+1,color);

    players[PlayerIndex].x = xCord;
    players[PlayerIndex].y = yCord;
}

void placeBomb (unsigned short PlayerIndex) {
    unsigned short xCord = players[PlayerIndex].x;
    unsigned short yCord = players[PlayerIndex].y;
    unsigned short i;

    if (players[PlayerIndex].bombs>0) {
        players[PlayerIndex].bombs--;
        for (i=0; i<6; i++)
            if (bombs[i].placed == 0)
                break;

        bombs[i].placed = 1;
        bombs[i].x = xCord;
        bombs[i].y = yCord;
        bombs[i].timeToExplode = 5000;
        bombs[i].placedBy = PlayerIndex;
        drawBomb(i);
    }
}

void drawBomb (unsigned short index) {
    unsigned short xCord = bombs[index].x;
    unsigned short yCord = bombs[index].y;
    unsigned short xCenter = tileSizeHalf + xCord*tileSize;
    unsigned short yCenter = tileSizeHalf + yCord*tileSize;
    unsigned int bombColor = YELLOW;

    setcolor(bombColor);
    circle(xCenter,yCenter,bombRadius);
    setfillstyle(1,bombColor);
    floodfill(xCenter,yCenter,bombColor);
}

void explodeBomb (unsigned short index) {
    unsigned short xCord = bombs[index].x;
    unsigned short yCord = bombs[index].y;
    unsigned short xCenter = tileSizeHalf + xCord*tileSize;
    unsigned short yCenter = tileSizeHalf + yCord*tileSize;
    unsigned short xp1 = players[0].x;
    unsigned short yp1 = players[0].y;
    unsigned short xp2 = players[1].x;
    unsigned short yp2 = players[1].y;
    unsigned short playerIndex = 3;
    int i;

    setcolor(YELLOW);
    setfillstyle(1,YELLOW);

    for (i=0; i<3; i++) {
        if(xCord==xp1&&yCord-1-i==yp1)
            playerIndex = 0;
        else if(xCord==xp2&&yCord-1-i==yp2)
            playerIndex = 1;

        if(xCord-1-i==xp1&&yCord==yp1)
            playerIndex = 0;
        else if(xCord-1-i==xp2&&yCord==yp2)
            playerIndex = 1;

        if(xCord+1+i==xp1&&yCord==yp1)
            playerIndex = 0;
        else if(xCord+1+i==xp2&&yCord==yp2)
            playerIndex = 1;

        if(xCord==xp1&&yCord+i+1==yp1)
            playerIndex = 0;
        else if(xCord==xp2&&yCord+i+1==yp2)
            playerIndex = 1;

        playerHit (playerIndex);

        setfillstyle(1, YELLOW);
        if(tiles[xCord][yCord-i].type!='O')
        {
            //changeTile (xCord, yCord-i);
            circle(xCenter,yCenter-(i*tileSize),  explosionRadius);
            floodfill(xCenter,yCenter-(i*tileSize), YELLOW);
        }

        if(tiles[xCord+i][yCord].type!='O')
        {
            //changeTile (xCord+i, yCord);
            circle(xCenter+(i*tileSize),yCenter, explosionRadius);
            floodfill(xCenter+(i*tileSize),yCenter,YELLOW);
        }

        if(tiles[xCord][yCord+i].type!='O')
        {
            //changeTile (xCord, yCord+i);
            circle(xCenter,yCenter+(i*tileSize), explosionRadius);
            floodfill(xCenter,yCenter+(i*tileSize),YELLOW);
        }

        if(tiles[xCord-i][yCord].type!='O')
        {
            //changeTile (xCord-i, yCord);
            circle(xCenter-(i*tileSize),yCenter, explosionRadius);
            floodfill(xCenter-(i*tileSize),yCenter,YELLOW);
        }

        delay(200);
        playerIndex = 3;
    }
    delay(200);
    changeTile(xCord, yCord);
    for (i=0; i<3; i++) {
        if(tiles[xCord][yCord-i].type!='O')
            changeTile (xCord, yCord-i);

        if(tiles[xCord+i][yCord].type!='O')
            changeTile (xCord+i, yCord);

        if(tiles[xCord][yCord+i].type!='O')
            changeTile (xCord, yCord+i);

        if(tiles[xCord-i][yCord].type!='O')
            changeTile (xCord-i, yCord);
    }
}

void changeTile (unsigned short xCord, unsigned short yCord) {
    unsigned short top, bottom, left, right;
    tiles[xCord][yCord].type = 'B';
    setcolor(BLACK);
    setfillstyle(1, BLACK);
    top = offset + (xCord)*tileSize;
    bottom = offset + (xCord+1)*tileSize;
    left = offset + (yCord)*tileSize;
    right = offset + (yCord+1)*tileSize;
    rectangle(top, left, bottom, right);
    floodfill(top+1, left+1, BLACK);
}

void playerHit(unsigned short playerIndex) {
    if (playerIndex != 3)
        if (--players[playerIndex].life <= 0) {
            gameOver();
        }
}

void gameOver() {
    // game is now over
    // walk on home boy!
    // and do something useful to actually make game over
    // or simply make games state = 0
    gameState = 0;
}

void resultScreen () {
    if (players[0].life == players[1].life) {
        printf("THE GAME IS DRAW");
    } else if(players[0].life > 0) {
        printf("PLAYER 0 WINS!");
    } else {
        printf("PLAYER 1 WINS!");
    }

    printf("\n\n\n Thank you for playing our game!");
}

void main () {
    int gd=DETECT, gm, flag=0, i;
    unsigned short player;
    char ch, chPre, dir;

    //clrscr();

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    // lets setup the level
    setup();
    gameState = 1;

    while (gameState) {
        if (kbhit()) {
            ch = getch();
            if (chPre != ch)
            flag = 1;
        }

        if (ch == 'q' || ch == 'Q')
            break;

        if (flag) {
            //printf("%d",ch);
            switch (ch) {
            // cases for player 1:
            // using wasd keys
            // and SPACE BAR for placing bomb
            case 'w':
                player = 0;
                dir = 'W';
                break;
            case 'd':
                player = 0;
                dir = 'S';
                break;
            case 'a':
                player = 0;
                dir = 'N';
                break;
            case 's':
                player = 0;
                dir = 'E';
                break;
            case 32:
                player = 0;
                dir = 'B';
                break;

            // cases for player 2:
            // using arrow keys
            // and 0 for placing bomb
            case 72: // up key
                player = 1;
                dir = 'W';
                break;
            case 77: // right key
                player = 1;
                dir = 'S';
                break;
            case 75: // left key
                player = 1;
                dir = 'N';
                break;
            case 80: // down key
                player = 1;
                dir = 'E';
                break;
            case 48: // for zero
                player = 1;
                dir = 'B';
                break;
            }
            movePlayer(player, dir);
        } else {
            chPre = 'g';
        }

        // reset flags etc.
        flag = 0;

        for (i=0; i<6; ++i) {
            if (bombs[i].placed) {
                drawBomb(i);
                bombs[i].timeToExplode -= 1.1;
                if (bombs[i].timeToExplode < 0) {
                    explodeBomb(i);
                    bombs[i].placed = 0;
                    players[bombs[i].placedBy].bombs++;
                }
            }
        }

        // delay frame for 1 millisec
        delay(1);
    }
    closegraph();

    // a thank you screen if you want!
    resultScreen();
    getch();
}
