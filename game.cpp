#include "game.hpp"
#define MAX_VEL 10
using namespace std;
sf::Vector2f bg_speed(-0.5, 0), base_speed(-2, 0);
float b_velocity, b_acceleration, b_impulsion, angular_vel;
sf::SoundBuffer buffer[4];

void Game::load_texture()
{
    //NUMBERS
    char tmp[6];
    for (int i=0; i<10; i++)
    {
        sprintf(tmp, "%d.png", i);
        t_numbers[i].loadFromFile("assets/sprites/"+string(tmp));
    }
    //BACKGROUND
    t_background[day].loadFromFile("assets/sprites/background-day.png");
    t_background[night].loadFromFile("assets/sprites/background-night.png");
    ///////////////////////////////////////////////////
    t_message.loadFromFile("assets/sprites/message.png");
    t_game_over.loadFromFile("assets/sprites/gameover.png");
    t_base.loadFromFile("assets/sprites/base.png");
    ///////////////////////////////////////////////////
    t_cursor[0].loadFromFile("assets/sprites/cursors/cursor1.png");
    t_cursor[1].loadFromFile("assets/sprites/cursors/cursor2.png");
}
void Game::load_sprite()
{
    background.setTexture(t_background[background_type]);
    bg.setTexture(t_background[background_type]);
    message.setTexture(t_message);
    game_over.setTexture(t_game_over);
    base.setTexture(t_base);
    cursor.setTexture(t_cursor[0]);
}
void Game::Set()
{
    bird.setType(rand()%3);
    background_type=rand()%2;
    load_sprite();
}
Game::Game()
{
    srand(time(NULL));
    score=0;
    b_impulsion=-6.25;
    b_velocity=1;
    b_acceleration=0.25;
    //Importing textures
    load_texture();
    Bird::load_texture();
    Pipe::load_texture();
    //Define bird
    bird=Bird();
    Set();
    //Loading sounds
    sounds["die"]=sf::Sound();
    sounds["hit"]=sf::Sound();
    sounds["point"]=sf::Sound();
    sounds["wing"]=sf::Sound();
    int i=0;
    for (std::map<std::string, sf::Sound>::iterator it=sounds.begin(); it!=sounds.end(); ++it, i++)
    {
       buffer[i].loadFromFile("assets/audio/"+it->first+".ogg");
       it->second.setBuffer(buffer[i]);
    }
    //Define rendering objects
    window=new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Flappy Bird", sf::Style::Close);
    sf::VideoMode desktop_mode(sf::VideoMode::getDesktopMode());
    window->setPosition(sf::Vector2i(desktop_mode.width/4, 0));
    font.loadFromFile("assets/arial.ttf");
}
bool Game::main()
{
    bool running=true;
    int section(welcome);
    std::ifstream r_file("assets/scr");
    int high_score(0);
    if (r_file) r_file>>high_score;
    r_file.close();
    std::ofstream w_file("assets/scr");
    window->setFramerateLimit(FPS);
    game_over.setOrigin(t_game_over.getSize().x*0.5, t_game_over.getSize().y*0.5);
    message.setOrigin(t_message.getSize().x/2, t_message.getSize().y/2);
    game_over.setPosition(WIDTH*0.5, HEIGHT*0.5);
    message.setPosition(WIDTH/2, HEIGHT/2);
    sf::Sprite b(t_base);
    bg.setPosition(WIDTH, 0);
    clock.restart();
    bird.restart_clock(); bird.setType(Bird::yellow);
    base.setPosition(0, 820);
    b.setPosition(WIDTH, 820);
    bird.setType(Bird::red);
    float dy(1.5);
    window->setMouseCursorVisible(false);
    cursor.scale(0.75, 0.75);
    char tmp[4]; sprintf(tmp, "%d", high_score);
    sf::Text h_s("High Score: "+std::string(tmp), font);

    while (running)
    {
        while (window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::MouseEntered:
                window->setMouseCursorVisible(false);
                break;
            case sf::Event::MouseLeft:
                window->setMouseCursorVisible(true);
                break;
            case sf::Event::LostFocus:
                while (window->waitEvent(event) && event.type!=sf::Event::GainedFocus);
                break;
            case sf::Event::Closed:
                if (score>high_score) w_file<<score;
                else w_file<<high_score;
                return false;
                break;
            case sf::Event::MouseButtonPressed:
                cursor.setTexture(t_cursor[1]);
                break;
            case sf::Event::MouseButtonReleased:
                cursor.setTexture(t_cursor[0]);
                if (section == game)
                {
                    angular_vel = -30;
                    b_velocity = b_impulsion;
                    sounds["wing"].play();
                }
                if (section == welcome)
                {
                    section = game;
                    b_velocity = 0;
                }
                if (section == Game_Over)
                {
                    section = welcome;
                    high_score = (score>high_score)?score:high_score;
                    sprintf(tmp, "%d", high_score);
                    h_s.setString("High Score: "+std::string(tmp));
                    Set();
                    bird.replace();
                }
                break;
            case sf::Event::MouseMoved:
                cursor.setPosition(event.mouseMove.x, event.mouseMove.y);
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::Space)
                    while (window->waitEvent(event) && event.key.code != sf::Keyboard::Space);
                break;
            default: ;
            }
        }
        window->draw(background);
        window->draw(bg);
        /////////////////////////////////////////////////////////////////////////
        background.move(bg_speed);
        bg.move(bg_speed);
        if (background.getPosition().x<=-WIDTH) background.setPosition(WIDTH, 0);
        if (bg.getPosition().x<=-WIDTH) bg.setPosition(WIDTH, 0);
        base.move(base_speed);
        b.move(base_speed);
        if (base.getPosition().x<=-WIDTH) base.setPosition(WIDTH, 820);
        if (b.getPosition().x<=-WIDTH) b.setPosition(WIDTH, 820);
        /////////////////////////////////////////////////////////////////////////
        switch (section)
        {
        case welcome:
            score=0;
            base_speed.x=-2;
            pipes.clear();
            if (pipes.size()==0)
                pipes.push_back(Pipe(sf::Vector2f(WIDTH, randint(HEIGHT*0.25, HEIGHT*0.75))));
            bird.move(dy);
            bird.set_rotation(0);
            if (bird.get_position()>=BIRD_POS.y+50 || bird.get_position()<=BIRD_POS.y)
            {
                bird.move(-dy/fabs(dy));
                dy*=-1;
            }
            window->draw(message);
            update_bird_wings();
            window->draw(bird);
            break;

        case game:
            angular_vel=(angular_vel>90)?90:angular_vel+1;
            b_velocity = (b_velocity>=MAX_VEL)?MAX_VEL:(b_velocity+b_acceleration);
            bird.move(b_velocity);
            bird.set_rotation(angular_vel);
            update_bird_wings();
            window->draw(bird);
            for (int i=0; i<(int)pipes.size(); i++)
            {
                pipes[i].move(base_speed.x);
                window->draw(pipes[i]);
                if (pipes[i].reached())
                    pipes.push_back(Pipe(sf::Vector2f(WIDTH, randint(HEIGHT*0.25, HEIGHT*0.75))));
            }
            if (!pipes[0].passed && pipes[0].get_position().x+pipe_width*0.5<=BIRD_POS.x)
            {
                pipes[0].passed=true;
                score++;
                sounds["point"].play();
            }
            if (bird.get_position()<-48 || pipes[0].collide(bird.rect()) || bird.get_position() + Bird::get_texture(0, 0).getSize().y > 820)
            {
                sounds["hit"].play();
                section=Game_Over;
            }
            if (pipes[0].get_position().x+pipe_width<=0)
                pipes.pop_front();
            draw_score();
            break;

        case Game_Over:
            base_speed.x=0;
            if (bird.get_position() + Bird::get_texture(0, 0).getSize().y <= 820)
            {
                sounds["die"].play();
                bird.move(b_velocity);
                bird.set_rotation(angular_vel);
                b_velocity+=b_acceleration;
                for (int i=0; i<(int)pipes.size(); ++i) window->draw(pipes[i]);
                window->draw(bird);
            }
            window->draw(game_over);
            draw_score();
            break;
        default: ;
        }
        window->draw(b);
        window->draw(base);
        window->draw(cursor);
        window->draw(h_s);
        window->display();
    }
    return true;
}
void Game::update_bird_wings()
{
    if (bird.elapsed_time()>125)
    {
        bird.restart_clock();
        bird.setPhase((bird.cur_phase()+1)%3);
    }
}
void Game::run()
{
    while (window->isOpen()) if (!main()) window->close();
}
void Game::draw_score()

{
    unsigned int c, d, u; //Pas sûr que quelqu'un arrive jusqu'à mille :D
    c = score/100;
    d = (score-c*100)/10;
    u = score-(c*100+d*10);
    sf::Sprite numbers[3];
    numbers[0].setTexture(t_numbers[u]);
    numbers[0].setPosition(300, 100);
    window->draw(numbers[0]);

    numbers[1].setTexture(t_numbers[d]);
    numbers[1].setPosition(300-t_numbers[d].getSize().x, 100);
    window->draw(numbers[1]);
    if (c)
    {
        numbers[2].setTexture(t_numbers[c]);
        numbers[2].setPosition(300-(t_numbers[c].getSize().x+t_numbers[d].getSize().x), 100);
        window->draw(numbers[2]);
    }
}
