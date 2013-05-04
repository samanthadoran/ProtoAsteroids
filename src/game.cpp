#include "../include/game.h"

#include <time.h>
#include <cstdlib>
#include <sstream>
Game::Game()
{
    //ctor
    App.create(sf::VideoMode(1920,1030), "DerpyVaders");
    App.setVerticalSyncEnabled(true);
    srand(time(0));
    timeFrame = clock.restart();
    restart = true;
    gameOver = false;
}

void Game::initAsteroids()
{
    //Clear the vector of asteroids and generate random ones!
    asteroids.clear();
    for(unsigned int i = 0; i < (unsigned int) rand() % 30 + 1; ++i)
    {
        asteroids.push_back(new Asteroid(Vector(rand() % 50 + 1,rand()%360+1), rand()%100+1, rand()%App.getSize().x+1, rand()%App.getSize().y+1));
    }
}

void Game::init()
{
    //while the player wishes to continue...
    while(restart)
    {
        //Reset to initial..
        gameOver = false;
        restart = true;
        player = Player();
        initAsteroids();

        //Begin the game!
        loop();
    }
}

void Game::runEvents()
{
    sf::Event event;
    while(App.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            restart = false;
            gameOver = true;
            App.close();
            break;
        }
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Escape)
            {
                restart = false;
                gameOver = true;
                App.close();
                break;
            }
        }
    }
}


bool Game::checkColl(sf::FloatRect first, sf::FloatRect second) const
{
    if(first.intersects(second))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Game::restartGame()
{
    //Initialze a font for text
    sf::Font font;
    font.loadFromFile("arial.ttf");

    //to prompt the user on restarting
    sf::Text text("Press Y to restart or N to quit.", font);
    text.setCharacterSize(30);
    text.setPosition(App.getSize().x/2.0, App.getSize().y/2.0);
    text.setStyle(sf::Text::Bold);
    text.setColor(sf::Color::Green);
    // Draw it
    App.draw(text);
    App.display();

    while(true)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
        {
            return true;
            break;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
        {
            return false;
            break;
        }
    }
}

void Game::loop()
{
    while(App.isOpen()&&!gameOver)
    {
        timeFrame = clock.restart();

        //Generic application stuff
        runEvents();
        if(gameOver)
        {
            break;
        }

        //Allow the player to query the state of controls
        player.control(timeFrame);

        //Update the player based on its current state
        player.update(timeFrame);

        for(Asteroid *ast: asteroids)
        {
            if(checkColl(player.getShape().getGlobalBounds(),ast->getCircle().getGlobalBounds()))
            {
                gameOver = true;
                break;
            }
        }

        for(Asteroid *ast: asteroids)
        {
            ast->update(timeFrame);
        }

        App.clear();

        //draw the player
        App.draw(player.getShape());

        if(player.getBullet()->getState())
        {
            App.draw(player.getBullet()->getRect());
        }

        for(Asteroid *ast: asteroids)
        {
            App.draw(ast->getCircle());
        }

        App.display();
    }

    if(App.isOpen())
    {
        restart=restartGame();
    }
}

Game::~Game()
{
    //dtor
}