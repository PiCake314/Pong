#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>


#define SCRN_HEIGHT 720
#define SCRN_WIDTH 1080
#define PLAYER_SPEED .25f



void playersMovment(sf::RectangleShape &player1, sf::RectangleShape &player2, int player_height){

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)
    && player1.getPosition().y - player_height/2 > 0
    )
    player1.move(0, -PLAYER_SPEED);


    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)
    && player1.getPosition().y + player_height/2 < SCRN_HEIGHT
    )
    player1.move(0, PLAYER_SPEED);



    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad8)
    && player2.getPosition().y - player_height/2 > 0
    )
    player2.move(0, -PLAYER_SPEED);


    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad5)
    && player2.getPosition().y + player_height/2 < SCRN_HEIGHT
    )
    player2.move(0, PLAYER_SPEED);

}

int didCollide(const sf::CircleShape &ball,const sf::RectangleShape player1, const sf::RectangleShape player2){
    sf::Vector2f ballPos = ball.getPosition();
    int r = ball.getRadius();

    sf::Vector2f p1Pos = player1.getPosition();
    sf::Vector2f p1Size = player1.getSize();

    sf::Vector2f p2Pos = player2.getPosition();
    sf::Vector2f p2Size = player2.getSize();

    if(ballPos.x - r <= p1Pos.x + p1Size.x
    && ballPos.y >= p1Pos.y - p1Size.y/2 && ballPos.y <= p1Pos.y + p1Size.y/2)
        return 1;

    if(ballPos.x + r >= p2Pos.x - p2Size.x
    && ballPos.y >= p2Pos.y - p2Size.y/2 && ballPos.y <= p2Pos.y + p2Size.y/2)
        return 2;

    return 0;
}

int didHitWall(const sf::CircleShape &ball){
    int y = ball.getPosition().y;
    int r = ball.getRadius();

    if(y - r <= 0) return 1;
    if(y + r >= SCRN_HEIGHT) return 2;

    return 0;
}

int checkPoint(const sf::CircleShape &ball){
    int x = ball.getPosition().x;
    int r = ball.getRadius();

    if(x + r >= SCRN_WIDTH) return 1;
    if(x - r <= 0) return 2;

    return 0;
}


void generateVel(float &vx, float &vy){
    vx = float(rand() % 200 - 100)/100;
    vy = sqrt(1 - vx*vx);
    vy = vx < 0? -vy : vy;

    vx /= 3;
    vy /= 3;
}



void splashScreen(sf::RenderWindow &win,const sf::Font &font){
    sf::Text welc;
    welc.setFont(font);
    welc.setFillColor(sf::Color::White);
    welc.setCharacterSize(48);
    welc.setString("Welcome to Pong!!");
    welc.setOrigin(welc.getGlobalBounds().width/2, welc.getGlobalBounds().height/2);
    welc.setPosition(SCRN_WIDTH/2, SCRN_HEIGHT/2);


    sf::Text space;
    space.setFont(font);
    space.setFillColor(sf::Color::White);
    space.setCharacterSize(24);
    space.setString("Press Space to Start...");
    space.setOrigin(space.getGlobalBounds().width/2, space.getGlobalBounds().height/2);
    space.setPosition(SCRN_WIDTH/2, SCRN_HEIGHT/1.25f);

    win.draw(welc);
    win.draw(space);
    win.display();

    sf::Event evnt;
    while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        if (win.pollEvent(evnt)){
            if(evnt.type == sf::Event::Closed){
                win.close();
                exit(0);
            }
        }
    }
}


void hasWon(sf::RenderWindow &win, bool p1, const sf::Font &font){
    std::string win_str = p1? "Player 1 has won!!" : "Player 2 has won!!";

    sf::Text winner;
    winner.setFont(font);
    winner.setFillColor(sf::Color::White);
    winner.setString(win_str);
    winner.setCharacterSize(52);
    winner.setOrigin(winner.getGlobalBounds().width/2, winner.getGlobalBounds().height/2);
    winner.setPosition(SCRN_WIDTH/2, SCRN_HEIGHT/2);


    sf::Text space;
    space.setFont(font);
    space.setFillColor(sf::Color::White);
    space.setCharacterSize(24);
    space.setString("Press Space to restart the game...");
    space.setOrigin(space.getGlobalBounds().width/2, space.getGlobalBounds().height/2);
    space.setPosition(SCRN_WIDTH/2, SCRN_HEIGHT/1.25f);


    win.clear();
    win.draw(winner);
    win.draw(space);
    win.display();

    sf::Event evnt;
    while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        if (win.pollEvent(evnt)){
            if(evnt.type == sf::Event::Closed){
                win.close();
                exit(0);
            }
        }
    }
}

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(SCRN_WIDTH, SCRN_HEIGHT), "Pong!", sf::Style::Close);
    sf::Event evnt;


    sf::Font font;
    if(!font.loadFromFile("fonts/CascadiaCodePL-Regular.ttf")) throw("Err Loading Font!!");


    splashScreen(window, font);


    std::string score_string = "0 : 0";
    sf::Text score;
    score.setFont(font);
    score.setFillColor(sf::Color::White);
    score.setString(score_string);
    score.setOrigin(score.getGlobalBounds().width/2, score.getGlobalBounds().height/2);
    score.setPosition(SCRN_WIDTH/2, 50);



    int player_height = SCRN_HEIGHT/5.12f, player_width = SCRN_WIDTH/102.4f;

    // Defining players!
    sf::RectangleShape player1(sf::Vector2f(player_width, player_height));
    player1.setFillColor(sf::Color::White);
    player1.setOrigin(sf::Vector2f(player_width/2, player_height/2));
    player1.setPosition(sf::Vector2f(SCRN_WIDTH/20.0f, SCRN_HEIGHT/2));


    sf::RectangleShape player2(sf::Vector2f(player_width, player_height));
    player2.setOrigin(sf::Vector2f(player_width/2, player_height/2));
    player2.setPosition(sf::Vector2f(9.5*SCRN_WIDTH/10.0f, SCRN_HEIGHT/2));
    player2.setFillColor(sf::Color::White);



    // Defining Ball!
    int ballRadius = player_width;
    sf::CircleShape ball(ballRadius);
    ball.setOrigin(sf::Vector2f(ballRadius, ballRadius));
    ball.setFillColor(sf::Color::White);
    ball.setPosition(sf::Vector2f(SCRN_WIDTH/2, SCRN_HEIGHT/2));



    //Vars
    int p1Score = 0;
    int p2Score = 0;

    float vx, vy;
    generateVel(vx, vy);

    std::cout << vx << ", " << vy << std::endl;
    std::cout << sqrt(vx*vx+vy*vy) << std::endl;

    while(window.isOpen()){
        if (window.pollEvent(evnt)){
            if(evnt.type == sf::Event::Closed) window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) window.close();

        playersMovment(player1, player2, player_height);


        if(didCollide(ball, player1, player2)) vx *= -1;

        if(didHitWall(ball)) vy *= -1;

        ball.move(vx, vy);


        switch(checkPoint(ball))
        {
            case 1:
                p1Score++;
                score_string = std::to_string(p1Score) + " : " + std::to_string(p2Score);
                score.setString(score_string);
                score.setOrigin(score.getGlobalBounds().width/2, score.getGlobalBounds().height/2);
                score.setPosition(SCRN_WIDTH/2, 50);
                vx *= -1;
                break;

            case 2:
                p2Score++;
                score_string = std::to_string(p1Score) + " : " + std::to_string(p2Score);
                score.setString(score_string);
                score.setOrigin(score.getGlobalBounds().width/2, score.getGlobalBounds().height/2);
                score.setPosition(SCRN_WIDTH/2, 50);
                vx *= -1;
        }

        if(p1Score >= 7 || p2Score >= 7){
            hasWon(window, p1Score > p2Score, font);
            p1Score = 0, p2Score = 0;
            score_string = std::to_string(p1Score) + " : " + std::to_string(p2Score);
            score.setString(score_string);
            score.setOrigin(score.getGlobalBounds().width/2, score.getGlobalBounds().height/2);
            score.setPosition(SCRN_WIDTH/2, 50);
            ball.setPosition(SCRN_WIDTH/2, SCRN_HEIGHT/2);
            generateVel(vx, vy);
        }

        window.clear();
        window.draw(score);
        window.draw(player1);
        window.draw(player2);
        window.draw(ball);
        window.display();
    }
}