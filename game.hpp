#ifndef GAME_HPP
#define GAME_HPP
#include "const.hpp"
#include "bird.hpp"
#include "pipe.hpp"
#include <ctime>
#include <cmath>
#include <utility>
#include <deque>
#include <map>
#include <fstream>

class Game
{
public:
    enum{day=0, night};
    enum{welcome, game, Game_Over};
    Game();
    void run();
private:
    void draw_score();
    void load_texture();
    void load_sprite();
    void Set();
    bool main(); //false: Close window, true: hold on
    void update_bird_wings();
    int background_type;
    unsigned int score;
    sf::Texture t_numbers[10], t_background[2], t_message, t_game_over, t_base, t_cursor[2];
    std::map<std::string, sf::Sound> sounds;
    sf::Sprite bg, background, message, game_over, base, cursor;
    std::deque<Pipe> pipes;
    Bird bird;
    sf::RenderWindow* window;
    sf::Event event;
    sf::Clock clock;
    sf::Font font;
};
#endif // GAME_HPP
