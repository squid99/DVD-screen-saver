#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./constants.h"

//gcc -Wall -std=c99 -I./include -L./lib ./src/DVD.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -o DVD

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture *dvdTexture = NULL;
int imgFlags;

int last_frame_time = 0;

struct ball{
    float x;
    float y;
    float width;
    float height;
    int speedx;
    int speedy;
} ball;

int initialize_window(void){
    if(SDL_Init(SDL_INIT_EVERYTHING) !=0){
        fprintf(stderr,"Error Initializing SDL.\n");
        return FALSE;   
    }
    window = SDL_CreateWindow(
        "DVD",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if(!window){
        fprintf(stderr,"Error creating SDL window.\n");
        return FALSE;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer){
        fprintf(stderr,"Error creasinf SDL renderer.\n");
        return FALSE;
    }
    imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)){
        fprintf(stderr,"Error initializing SDL_image\n");
    }

    //loading DVD logo
    dvdTexture = IMG_LoadTexture(renderer,"DVD_logo.png");
    if(dvdTexture == NULL){
        fprintf(stderr,"Error loading image\n");
    }

    
    return TRUE;
}

void setup(){
    ball.x=WINDOW_WIDTH/2;
    ball.y=WINDOW_HEIGHT/2;
    ball.width=200;
    ball.height=100;
    ball.speedx=BALL_SPEED_X;
    ball.speedy=BALL_SPEED_Y;
}

void process_input(){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type){
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE){ game_is_running = FALSE; }
            break;
    }
}

void update(){

//target time in millisecond
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    //delay if there is more time
    if(time_to_wait>0 && time_to_wait<= FRAME_TARGET_TIME){
        SDL_Delay(time_to_wait);
    }

//time gap for speed
    float delta_time = (SDL_GetTicks() - last_frame_time)/1000.0f;
    last_frame_time = SDL_GetTicks();

    ball.x += ball.speedx * delta_time;
    ball.y += ball.speedy * delta_time;
    int hit=0;
//ball hitting sides
    if(ball.y<=2){ball.y=2;ball.speedy*=-1; hit=1;}
    else if(ball.y>=WINDOW_HEIGHT-ball.height-2){ball.y=WINDOW_HEIGHT-ball.height-2;ball.speedy*=-1; hit=1;}
    if(ball.x<=2){ball.x=2;ball.speedx*=-1; hit=1;}
    else if(ball.x>=WINDOW_WIDTH-ball.width-2){ball.x=WINDOW_WIDTH-ball.width-2;ball.speedx*=-1; hit=1;}

    if(hit){
        Uint8 r=50 + rand() % 206;
        Uint8 g=50 + rand() % 206;
        Uint8 b=50 + rand() % 206;    
        SDL_SetTextureColorMod(dvdTexture,r,g,b);
    }
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect ball_rect ={(int)ball.x, (int)ball.y, (int)ball.width, (int)ball.height};

    SDL_RenderCopy(renderer,dvdTexture, NULL, &ball_rect);

    SDL_RenderPresent(renderer);
}

void destroy_window(){
    SDL_DestroyTexture(dvdTexture);
    IMG_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    game_is_running = initialize_window();

    setup();

    while(game_is_running){
        process_input();
        update();
        render();
    }
    
    destroy_window();

    return 0;
}