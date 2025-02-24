#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>

const int W = 640;
const int H = 480;

class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    double radius;

    Ball(double r, sf::Vector2f position, sf::Vector2f vel, sf::Color color, double outlineThickness = 0.0f) {
        radius = r;
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(position);
        shape.setFillColor(color);
        velocity = vel;
        shape.setOutlineThickness(outlineThickness);
        shape.setOutlineColor(sf::Color::White);
    }

    void update(float deltaTime) {
        shape.move(velocity * deltaTime);
        checkBoundaryCollision();
    }

    void checkBoundaryCollision() {
        sf::Vector2f position = shape.getPosition();
        if (position.x + radius > W || position.x - radius < 0) {
            velocity.x = -velocity.x;
        }
        if (position.y + radius > H || position.y - radius < 0) {
            velocity.y = -velocity.y;
        }
    }

    void checkCollision(Ball& other) {
        sf::Vector2f pos1 = shape.getPosition();
        sf::Vector2f pos2 = other.shape.getPosition();

        double dx = pos1.x - pos2.x;
        double dy = pos1.y - pos2.y;
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance < radius + other.radius) {
            
            double nx = dx / distance;
            double ny = dy / distance;

            
            double relativeVelocityX = velocity.x - other.velocity.x;
            double relativeVelocityY = velocity.y - other.velocity.y;

            
            double speedAlongNormal = relativeVelocityX * nx + relativeVelocityY * ny;

            if (speedAlongNormal > 0) return; 

           
            float impulse = -2.0f * speedAlongNormal / (1.0f / radius + 1.0f / other.radius);
            velocity.x += impulse * nx / radius;
            velocity.y += impulse * ny / radius;
            other.velocity.x -= impulse * nx / other.radius;
            other.velocity.y -= impulse * ny / other.radius;
        }
    }

    void checkInsideBigBall(const Ball& bigBall) {
        sf::Vector2f pos = shape.getPosition();
        sf::Vector2f bigBallPos = bigBall.shape.getPosition();

        double dx = pos.x - bigBallPos.x;
        double dy = pos.y - bigBallPos.y;
        double distance = std::sqrt(dx * dx + dy * dy);

        
        if (distance + radius > bigBall.radius) {
           
            double nx = dx / distance;
            double ny = dy / distance;

            
            float dot = velocity.x * nx + velocity.y * ny;
            velocity.x -= 2 * dot * nx;
            velocity.y -= 2 * dot * ny;

            
            shape.setPosition(bigBallPos.x + nx * (bigBall.radius - radius),
                bigBallPos.y + ny * (bigBall.radius - radius));
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(W, H), "By Sc1d6x");
    std::cout << "Ball physics simulation" << std::endl;
    std::cout << "write the radius of the first ball" << std::endl;
    std::cout << "Notes radius < 70" << std::endl;
    double n; std::cin >> n;

    if (n > 70) {
        std::cout << "Not" << std::endl;
        n = 70;
     }

    std::cout << "write the radius of the second ball" << std::endl;
    std::cout << "Notes radius < 70" << std::endl;

    double c; std::cin >> c;

    if (c > 70) {
        std::cout << "Not" << std::endl;
        c = 70;
    }

    std::cout << "write the radius of the third ball" << std::endl;
    std::cout << "Notes radius < 70" << std::endl;
   
    double m; std::cin >> m;

    
    if (m > 70) {
        std::cout << "Not" << std::endl;
        m = 70;
    }

    

    

    std::cout << "Circle radii "  << '\n' << "1)" << n << '\n' << "2)" << c << "\n" << "3)" << m << std::endl;
    std::cout << "Stop" << " " << "escape button" << std::endl;
    std::cout << "TG: Viruz Studio" << std::endl;

    Ball ball1(n, sf::Vector2f(320, 240), sf::Vector2f(100,120), sf::Color(255, 0, 255), 0.0f);
    Ball ball2(c, sf::Vector2f(W / 2, H / 2), sf::Vector2f(120,100), sf::Color(255, 255, 0), 0.0f);
    Ball ball4(m, sf::Vector2f(W / 2, H / 2), sf::Vector2f(110, 100), sf::Color(255, 255, 255), 0.0f);
    Ball ball3(200.0f, sf::Vector2f(W / 2, H / 2), sf::Vector2f(0, 0), sf::Color(0, 0, 0, 0), 1.0f);

    sf::Clock clock;
    window.setFramerateLimit(60); 

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        float deltaTime = clock.restart().asSeconds();

       
        ball1.update(deltaTime);
        ball2.update(deltaTime);
        ball4.update(deltaTime);
        
        ball1.checkCollision(ball2);
        ball2.checkCollision(ball1);
        ball4.checkCollision(ball1);
        ball4.checkCollision(ball2);
        
        ball1.checkInsideBigBall(ball3);
        ball2.checkInsideBigBall(ball3);
        ball4.checkInsideBigBall(ball3);
       
        window.clear();
        ball1.draw(window);
        ball2.draw(window);
        ball3.draw(window);
        ball4.draw(window);
        window.display();
    }

    return 0;
}
