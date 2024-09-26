#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

// Определение функции
double f(double x) {
    if (x >= -7 && x <= -6) {
        return 1;
    }
    else if (x > -6 && x <= -4) {
        return -x / 2 - 2;
    }
    else if (x > -4 && x <= 0) {
        return std::sqrt(4 - std::pow(x + 2, 2));
    }
    else if (x > 0 && x <= 2) {
        return -std::sqrt(1 - std::pow(x - 1, 2));
    }
    else if (x > 2 && x <= 3) {
        return -x + 2;
    }
    else {
        return 0; // Вне интервала
    }
}

// Определение масштаба и сдвига
const float scaleX = 50.0f;
const float scaleY = 60.0f;
const float offsetX = 400.0f;
const float offsetY = 300.0f;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graphics functional");

    // Фон белый
    window.clear(sf::Color::White);

    std::vector<sf::Vertex> points;

    for (double x = -7.0; x <= 3.0; x += 0.01) {
        double y = f(x);
        float screenX = offsetX + static_cast<float>(x * scaleX);
        float screenY = offsetY - static_cast<float>(y * scaleY);
        points.push_back(sf::Vertex(sf::Vector2f(screenX, screenY), sf::Color::Red));
    }

    // Создаем оси
    sf::Vertex xAxis[] = {
        sf::Vertex(sf::Vector2f(0, offsetY), sf::Color::Black),
        sf::Vertex(sf::Vector2f(800, offsetY), sf::Color::Black)
    };

    sf::Vertex yAxis[] = {
        sf::Vertex(sf::Vector2f(offsetX, 0), sf::Color::Black),
        sf::Vertex(sf::Vector2f(offsetX, 600), sf::Color::Black)
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);  // Фон белый
        window.draw(xAxis, 2, sf::Lines);
        window.draw(yAxis, 2, sf::Lines);
        window.draw(&points[0], points.size(), sf::LineStrip);

        // Добавление делений на оси X
        for (float i = -10; i <= 10; i += 1.0f) {
            float screenX = offsetX + i * scaleX;
            sf::CircleShape tick(2);  // Маленькие точки
            tick.setPosition(screenX - 2, offsetY - 2); // -5 для центрирования
            tick.setFillColor(sf::Color::Black); // Цвет черный
            window.draw(tick);
        }

        // Добавление делений на оси Y
        for (float i = -3; i <= 3; i += 1.0f) {
            float screenY = offsetY - i * scaleY;
            sf::CircleShape tick(2);  // Маленькие точки
            tick.setPosition(offsetX - 2, screenY - 2); // -5 для центрирования
            tick.setFillColor(sf::Color::Black); // Цвет черный
            window.draw(tick);
        }

        window.display();
    }

    return 0;
}
