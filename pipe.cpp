#include "pipe.hpp"

int randint(int a, int b)
{
    if (a>b) return randint(b, a);
    return (int)((rand()/(float)RAND_MAX)*(b-a)+a);
}

sf::Texture Pipe::textures[2];
void Pipe::load_texture()
{
    textures[green].loadFromFile("assets/sprites/pipe-green.png");
    textures[red].loadFromFile("assets/sprites/pipe-red.png");
}

sf::Texture& Pipe::get_texture(int Color)
{
    return textures[Color];
}

Pipe::Pipe(sf::Vector2f new_position)
{
    set_color(rand()%2);
    cnt=0;
    passed=false;
    sprite[upper].setPosition(new_position.x+pipe_width*0.5, new_position.y-(VERTICAL_ECART+pipe_height*0.5));
    sprite[lower].setPosition(new_position);
}

void Pipe::set_color(int new_color)
{
    color = new_color;
    for (int i=0; i<2; i++) sprite[i].setTexture(get_texture(color));
    sprite[upper].setOrigin(pipe_width*0.5, pipe_height*0.5);
    sprite[upper].rotate(180);
}

void Pipe::move(float x_offset)
{
    for (int i=0; i<2; i++) sprite[i].move(x_offset, 0);
}

sf::Vector2f Pipe::get_position() const
{
    return sprite[lower].getPosition();
}

void Pipe::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite[upper], states);
    target.draw(sprite[lower], states);
}

bool Pipe::reached()
{
    if (get_position().x<=WIDTH-(HORIZONTAL_ECART+pipe_width)) cnt++;
    return (cnt==1)?true:false;
}

bool Pipe::collide(sf::FloatRect rect)
{
    return sprite[upper].getGlobalBounds().intersects(rect) || sprite[lower].getGlobalBounds().intersects(rect);
}
