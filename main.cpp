#include <iostream>
#include <sdl_wrapper.h>
#include <random>
#include <chrono>

bool get_random();

void coin_spin(int&, int&, const bool&, bool&);

const int delay_time = 80;

int main(int argc, char* args[])
{
    int delay = 0;

    if (brb::sdl::init()) {
        brb::sdl::WindowRenderer renderer("Coin Flip");
        brb::sdl::Font font1("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 74);
        brb::sdl::Texture victory_text(renderer, font1);
        brb::sdl::Texture background(renderer, font1);
        brb::sdl::Texture coin(renderer, font1);
        const int coin_width = 410;
        const int coin_height = 406;
        SDL_Rect current_coin = { 0, 0, coin_width, coin_height };

        background.load_pic("bg.png");
        coin.load_pic("sprite.png");
        bool quit = false;
        background.set_basecolor(brb::sdl::col::basics::black, 255);

        SDL_Event e;
        int sprite_num = 0;
        bool heads = get_random();
        bool ready = false;

        while (!quit) {
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                    case SDLK_RETURN:
                        heads = get_random();
                        ready = false;
                        delay = 0;
                    }
                }
            }
            background.clear();
            background.draw_fullscreen();
            coin.draw(100, 100, &current_coin);
            if (ready) {
                if (heads) {
                    victory_text.load_text("Heads", brb::sdl::col::whites::ivory);
                }
                else {
                    victory_text.load_text("Tails", brb::sdl::col::greens::forestGreen);
                }
                victory_text.draw(550, 250);
            }
            renderer.present();
            coin_spin(sprite_num, delay, heads, ready);
            current_coin.x = sprite_num * coin_width;
        }
    }
    brb::sdl::shutdown();
#ifndef _DEBUG
    brb::cleanup();
#endif
    return 0;
}

bool get_random()
{
    bool result;
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    // static std::mt19937 e{ seed };
    static std::random_device e("/dev/urandom");
    static std::bernoulli_distribution d(0.5);

    result = d(e);
    std::cout << result << std::flush;
    return !result;
}

void coin_spin(int& sprite_num, int& delay, const bool& heads, bool& ready)
{
    if (!ready) {
        sprite_num++;
        if (sprite_num >= 12)
            sprite_num = 0;
        SDL_Delay(delay);
        delay++;
        if (delay > delay_time && (!heads && sprite_num == 0||heads && sprite_num == 6)) {
            ready = true;
            delay = 0;
        }
    }
}