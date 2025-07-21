#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <bits/stdc++.h>

const unsigned WINDOW_SIZE = 800U;

// Colour
sf::Color bg_col(sf::Color::Black);
sf::Color snake_body_color(sf::Color::Green);

struct Point {
    float x;
    float y;
    float r;
};

class Snake {
private:
    float d;
    float speed;
    char dir;
    bool paused;
    std::vector<Point> body;

    float distance(Point& p1, Point& p2) {
        return static_cast<float>(sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
    }

    void update_position() {
        switch (dir) {
        case 'N':
            body.front().y -= speed;
            break;
        case 'W':
            body.front().x -= speed;
            break;
        case 'S':
            body.front().y += speed;
            break;
        case 'E':
            body.front().x += speed;
            break;
        default:
            break;
        }
    }

    void apply_dist_constraints() {
        for (size_t i = 1; i < body.size(); i++) {
            Point& p1 = body[i - 1];
            Point& p2 = body[i];
            float d_dash = distance(p1, p2);
            p2.x = p1.x + (p2.x - p1.x) * (d / d_dash);
            p2.y = p1.y + (p2.y - p1.y) * (d / d_dash);
        }
    }

public:
    Snake() : d(20.F), speed(6.F), dir('N'), paused(false) {
        for (float x = d; x < 2 * static_cast<float>(WINDOW_SIZE); x += d) {
            float t = x / (2 * static_cast<float>(WINDOW_SIZE) - 2 * d);
            float radius = (x == d) ? (d) * 0.6F : (d) * (1 - t * 0.8F);
            Point p = {x, static_cast<float>(WINDOW_SIZE / 2), radius};
            body.push_back(p);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (size_t i = 1; i < body.size(); i++) {
            sf::ConvexShape body_part(4);
            Point& b1 = body[i - 1];
            Point& b2 = body[i];
            sf::Vector2f body_vec(b1.x - b2.x, b1.y - b2.y);
            body_part.setPoint(0, {b1.x - body_vec.y * (b1.r / d), b1.y + body_vec.x * (b1.r / d)});
            body_part.setPoint(1, {b1.x + body_vec.y * (b1.r / d), b1.y - body_vec.x * (b1.r / d)});
            body_part.setPoint(2, {b2.x + body_vec.y * (b2.r / d), b2.y - body_vec.x * (b2.r / d)});
            body_part.setPoint(3, {b2.x - body_vec.y * (b2.r / d), b2.y + body_vec.x * (b2.r / d)});
            body_part.setFillColor(snake_body_color);
            window.draw(body_part);
        }

        for (auto& p : body) {
            sf::CircleShape point(p.r);
            point.setPosition({p.x - p.r, p.y - p.r});
            point.setFillColor(snake_body_color);
            window.draw(point);
        }

        if (!paused) {
            update_position();
            apply_dist_constraints();
        }
    }

    void handle_input(const std::optional<sf::Event>& event) {
        if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (key_pressed->scancode) {
            case sf::Keyboard::Scancode::W:
                if (dir != 'S') dir = 'N';
                break;
            case sf::Keyboard::Scancode::A:
                if (dir != 'E') dir = 'W';
                break;
            case sf::Keyboard::Scancode::S:
                if (dir != 'N') dir = 'S';
                break;
            case sf::Keyboard::Scancode::D:
                if (dir != 'W') dir = 'E';
                break;
            case sf::Keyboard::Scancode::Up:
                if (speed < 10) speed++;
                break;
            case sf::Keyboard::Scancode::Down:
                if (speed > 1) speed--;
                break;
            case sf::Keyboard::Scancode::Space:
                paused = !paused;
                break;
            default:
                break;
            }
        }
    }
};

int main() {
    // OpenGL settings
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    // Window Init
    sf::RenderWindow window(sf::VideoMode({WINDOW_SIZE, WINDOW_SIZE}),
                            "My window", sf::Style::Default,
                            sf::State::Windowed,
                            {0, 0, 8}
    );
    window.setFramerateLimit(60);

    // New snake
    Snake snake;

    // Main game loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            snake.handle_input(event);
        }
        window.clear(bg_col);
        snake.draw(window);
        window.display();
    }

    return 0;
}
