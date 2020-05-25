#ifndef BIRD_HPP
#define BIRD_HPP
#include "const.hpp"

class Bird: public sf::Drawable
{
public:
    enum color{red, blue, yellow};
    static void load_texture();
    static sf::Texture& get_texture(int coleur=red, int Phase=0);
    Bird(int Type=red);
    void setType(int new_type);
    void setPhase(int new_phase);
    void move(float y_offset);
    void rotate(float angle);
    void set_rotation(float angle);
    float get_position() const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states=sf::RenderStates::Default) const;
    void restart_clock();
    int elapsed_time();
    int cur_phase();
    void replace();
    sf::FloatRect rect();
private:
    static sf::Texture textures[3][3];
    sf::Sprite sprite;
    sf::Clock wing_clock;
    int type;
    int phase;
};
#endif // BIRD_HPP
