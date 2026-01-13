#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600
#define FPS 60
#define FRAME_DELAY (1000 / FPS)

/* GAME STATES */
#define MENU 0
#define PLAY 1
#define PAUSE 2
#define SETTINGS 3
#define GAMEOVER 4

typedef struct {
    SDL_Rect rect;
    const char *text;
    float scale;
    float alpha;
    int hovered;
    int wasHovered;
} Button;

/* ---------- FILE HELPERS ---------- */
int loadHighScore() {
    FILE *f = fopen("../score.dat", "r");
    int h = 0;
    if (f) { fscanf(f, "%d", &h); fclose(f); }
    return h;
}

void saveHighScore(int s) {
    FILE *f = fopen("../score.dat", "w");

    if (f) { fprintf(f, "%d", s); fclose(f); }
}

/* ---------- BUTTON DRAW ---------- */
void drawButton(SDL_Renderer *r, TTF_Font *font, Button *b) {
    b->scale += b->hovered ? 0.08f : -0.08f;
    if (b->scale < 1.0f) b->scale = 1.0f;
    if (b->scale > 1.12f) b->scale = 1.12f;

    if (b->alpha < 255) b->alpha += 8;

    SDL_Rect rc = {
        b->rect.x - (int)(b->rect.w * (b->scale - 1) / 2),
        b->rect.y - (int)(b->rect.h * (b->scale - 1) / 2),
        (int)(b->rect.w * b->scale),
        (int)(b->rect.h * b->scale)
    };

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 30, 30, 30, (Uint8)b->alpha);
    SDL_RenderFillRect(r, &rc);

    SDL_Color white = {255,255,255};
    SDL_Surface *s = TTF_RenderText_Solid(font, b->text, white);
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);

    SDL_Rect tr = {
        rc.x + (rc.w - s->w) / 2,
        rc.y + (rc.h - s->h) / 2,
        s->w, s->h
    };

    SDL_RenderCopy(r, t, NULL, &tr);
    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
}

int main() {
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_AllocateChannels(16);

    SDL_Window *window = SDL_CreateWindow(
        "Fly Bird - Pro",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0);

    SDL_Renderer *r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* ASSETS */
    SDL_Texture *bgFar  = IMG_LoadTexture(r, "../assets/bg_far.png");
    SDL_Texture *bgNear = IMG_LoadTexture(r, "../assets/bg_near.png");
    SDL_Texture *birdT  = IMG_LoadTexture(r, "../assets/bird.png");
    SDL_Texture *pipeT  = IMG_LoadTexture(r, "../assets/pipe.png");


    TTF_Font *font = TTF_OpenFont("../assets/font.ttf", 28);


    /* SOUNDS */
    Mix_Chunk *jumpSound     = Mix_LoadWAV("../assets/jump.wav");
    Mix_Chunk *hoverSound    = Mix_LoadWAV("../assets/hover.wav");
    Mix_Chunk *clickSound    = Mix_LoadWAV("../assets/click.wav");
    Mix_Chunk *gameOverSound = Mix_LoadWAV("../assets/gameover.wav");


    /* BUTTONS */
    Button startBtn   = {{300,220,200,50},"START",1,0,0,0};
    Button settingsBtn={{300,280,200,50},"SETTINGS",1,0,0,0};
    Button exitBtn    = {{300,340,200,50},"EXIT",1,0,0,0};

    Button resumeBtn  = {{300,260,200,50},"RESUME",1,0,0,0};
    Button quitBtn    = {{300,320,200,50},"QUIT",1,0,0,0};

    Button restartBtn = {{300,300,200,50},"RESTART",1,0,0,0};
    Button menuBtn    = {{300,360,200,50},"MAIN MENU",1,0,0,0};

    int running = 1;
    int state = MENU;
    int score = 0;
    int highScore = loadHighScore();
    int gameOverPlayed = 0;

    float birdY = HEIGHT/2;
    float vel = 0;
    int pipeX = WIDTH;
    int gapY = 200;

    SDL_Event e;
    SDL_Color white = {255,255,255};

    while (running) {
        Uint32 start = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;

            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    if (state == PLAY) state = PAUSE;
                    else if (state == PAUSE) state = PLAY;
                }
                if (e.key.keysym.sym == SDLK_SPACE && state == PLAY) {
                    vel = -8;
                    Mix_PlayChannel(-1, jumpSound, 0);
                }
            }

            if (e.type == SDL_MOUSEMOTION) {
                int mx = e.motion.x, my = e.motion.y;
                Button *btns[] = {
                    &startBtn,&settingsBtn,&exitBtn,
                    &resumeBtn,&quitBtn,
                    &restartBtn,&menuBtn
                };

                for (int i = 0; i < 7; i++) {
                    Button *b = btns[i];
                    int h = SDL_PointInRect(&(SDL_Point){mx,my}, &b->rect);
                    if (h && !b->wasHovered)
                        Mix_PlayChannel(-1, hoverSound, 0);
                    b->hovered = h;
                    b->wasHovered = h;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (state == MENU) {
                    if (startBtn.hovered) {
                        Mix_PlayChannel(-1, clickSound, 0);
                        score = 0; birdY = HEIGHT/2; vel = 0; pipeX = WIDTH;
                        state = PLAY;
                    }
                    if (exitBtn.hovered) {
                        Mix_PlayChannel(-1, clickSound, 0);
                        running = 0;
                    }
                }
                if (state == PAUSE) {
                    if (resumeBtn.hovered) {
                        Mix_PlayChannel(-1, clickSound, 0);
                        state = PLAY;
                    }
                    if (quitBtn.hovered) {
                        Mix_PlayChannel(-1, clickSound, 0);
                        running = 0;
                    }
                }
                if (state == GAMEOVER) {
                    if (restartBtn.hovered) {
                        Mix_PlayChannel(-1, clickSound, 0);
                        score = 0; birdY = HEIGHT/2; vel = 0; pipeX = WIDTH;
                        gameOverPlayed = 0;
                        state = PLAY;
                    }
                    if (menuBtn.hovered) {
                        Mix_PlayChannel(-1, clickSound, 0);
                        state = MENU;
                    }
                }
            }
        }

        if (state == PLAY) {
            vel += 0.5f;
            birdY += vel;
            pipeX -= 4;

            if (pipeX < -80) {
                pipeX = WIDTH;
                gapY = 150 + rand() % 250;
                score++;
            }

            if (birdY < 0 || birdY > HEIGHT ||
               (pipeX < 160 && pipeX > 80 &&
               (birdY < gapY || birdY > gapY + 160))) {

                if (score > highScore) saveHighScore(score);
                state = GAMEOVER;
                gameOverPlayed = 0;
            }
        }

        if (state == GAMEOVER && !gameOverPlayed) {
            Mix_PlayChannel(-1, gameOverSound, 0);
            gameOverPlayed = 1;
        }

        SDL_RenderClear(r);
        SDL_RenderCopy(r, bgFar, NULL, NULL);
        SDL_RenderCopy(r, bgNear, NULL, NULL);

        SDL_RenderCopy(r, pipeT, NULL, &(SDL_Rect){pipeX,0,80,gapY});
        SDL_RenderCopy(r, pipeT, NULL, &(SDL_Rect){pipeX,gapY+160,80,HEIGHT});
        SDL_RenderCopy(r, birdT, NULL, &(SDL_Rect){120,(int)birdY,40,40});

        char buf[64];
        sprintf(buf,"Score: %d  High: %d", score, highScore);
        SDL_Surface *s = TTF_RenderText_Solid(font, buf, white);
        SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
        SDL_RenderCopy(r, t, NULL, &(SDL_Rect){20,20,s->w,s->h});
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);

        if (state == MENU) {
            drawButton(r,font,&startBtn);
            drawButton(r,font,&settingsBtn);
            drawButton(r,font,&exitBtn);
        }
        if (state == PAUSE) {
            drawButton(r,font,&resumeBtn);
            drawButton(r,font,&quitBtn);
        }
        if (state == GAMEOVER) {
            drawButton(r,font,&restartBtn);
            drawButton(r,font,&menuBtn);
        }

        SDL_RenderPresent(r);

        Uint32 ft = SDL_GetTicks() - start;
        if (FRAME_DELAY > ft) SDL_Delay(FRAME_DELAY - ft);
    }

    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(hoverSound);
    Mix_FreeChunk(clickSound);
    Mix_FreeChunk(gameOverSound);

    Mix_CloseAudio();
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
