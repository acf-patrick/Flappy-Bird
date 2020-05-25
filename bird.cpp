#include "bird.hpp"
using namespace std;

sf::Texture Bird::textures[3][3];
void Bird::load_texture()
{
    char tmp1[3], tmp2[6];
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
        {
            sprintf(tmp1, "%d/", i);
            sprintf(tmp2, "%d.png", j);
            textures[i][j].loadFromFile("assets/sprites/birds/"+string(tmp1)+string(tmp2));
        }
}
sf::Texture& Bird::get_texture(int couleur, int Phase)
{
    return textures[couleur][Phase];
}
Bird::Bird(int Type)
{
    setType(Type);
    sprite.setOrigin(get_texture().getSize().x*0.5, get_texture().getSize().y*0.5);
    replace();
}
void Bird::setType(int new_type)
{
    type=new_type;
    sprite.setTexture(get_texture(new_type));
}
void Bird::setPhase(int new_phase)
{
    phase=new_phase;
    sprite.setTexture(get_texture(type, phase));
}
void Bird::replace()
{
    sprite.setPosition(BIRD_POS);
}
float Bird::get_position() const
{
    return sprite.getPosition().y;
}
void Bird::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
}
void Bird::move(float y_offset)
{
    sprite.move(0, y_offset);
}
void Bird::rotate(float angle)
{
    sprite.rotate(angle);
}
void Bird::set_rotation(float angle)
{
    sprite.setRotation(angle);
}
void Bird::restart_clock()
{
    wing_clock.restart();
}
int Bird::elapsed_time()
{
    return wing_clock.getElapsedTime().asMilliseconds();
}
int Bird::cur_phase()
{
    return phase;
}
sf::FloatRect Bird::rect()
{
    return sprite.getGlobalBounds();
}
