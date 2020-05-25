#ifndef PIPE_HPP
#define PIPE_HPP
#include "const.hpp"
#include <cmath>

int randint(int a, int b); //random integer in [a, b[

class Pipe: public sf::Drawable
{
public:
    enum{green, red, upper=0, lower};
    Pipe(sf::Vector2f position = sf::Vector2f()); //Vector2<float> argument is the position of the lower pipe,
                                                  // relative to the top-left point of the pipe sprite
    static sf::Texture& get_texture(int Color = green);
    static void load_texture();
    void set_color(int new_color);
    void move(float x_offset);
    sf::Vector2f get_position() const; //Lower Pipe
    void set_position(sf::Vector2f new_position);
    bool reached();
    bool collide(sf::FloatRect rect);
    bool passed;
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    int color;
    int cnt;
    sf::Sprite sprite[2];
    static sf::Texture textures[2];
};

#endif // PIPE_HPP
