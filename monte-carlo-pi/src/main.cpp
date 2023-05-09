#include <cmath>
#include <cstdint>
#include <iostream>
#include <optional>
#include <random>

#include <SFML/Graphics.hpp>

struct Point
{
   float x;
   float y;
};

void draw(const Point &point, sf::RenderWindow &window, sf::Color color)
{
   const auto [window_width, window_height] = window.getSize();
   const sf::Vector2f pos{ point.x + (static_cast<float>(window_width) / 2.0f),
      point.y + (static_cast<float>(window_height) / 2.0f) - 20 };
   const sf::Vertex vertex{ pos, color };

   window.draw(&vertex, 1, sf::Points);
}

class Square
{
 public:
   Square(std::uint16_t width, std::uint16_t height) : _width(width), _height(height) {}

   void draw(sf::RenderWindow &window) const
   {
      const sf::Vector2f dimensions{ static_cast<float>(_width), static_cast<float>(_height) };
      sf::RectangleShape rectangle{ dimensions };

      const auto [window_width, window_height] = window.getSize();
      rectangle.move(sf::Vector2f{ static_cast<float>(window_width / 4), static_cast<float>(window_height / 4) });
      rectangle.setFillColor(sf::Color::Transparent);
      rectangle.setOutlineThickness(1.0f);
      rectangle.setOutlineColor(sf::Color::White);

      window.draw(rectangle);
   }

   std::uint16_t width() const { return _width; }

   std::uint16_t height() const { return _height; }

 private:
   std::uint16_t _width;
   std::uint16_t _height;
};

class Diameter
{
 public:
   explicit Diameter(float value) : _value(value) {}

   float get() const { return _value; }

 private:
   float _value;
};

class Radius
{
 public:
   explicit Radius(float value) : _value(value) {}

   explicit Radius(Diameter diameter) : Radius(diameter.get() / 2) {}

   float get() const { return _value; }

 private:
   float _value;
};

class Circle
{
 public:
   explicit Circle(Radius radius) : _radius(radius) {}
   explicit Circle(float radius) : Circle(Radius(radius)) {}

   explicit Circle(Diameter diameter) : Circle(Radius(diameter)) {}

   void draw(sf::RenderWindow &window) const
   {
      sf::CircleShape circle{ _radius.get() };
      const auto [window_width, window_height] = window.getSize();
      circle.setFillColor(sf::Color::Transparent);
      circle.move(sf::Vector2f{ static_cast<float>(window_width / 4.0), static_cast<float>(window_height / 4.0) });
      circle.setOutlineThickness(1.0f);
      circle.setOutlineColor(sf::Color::White);
      window.draw(circle);
   }

   bool hasPoint(const Point &point) const
   {
      return static_cast<float>(std::sqrt(std::pow(point.x, 2) + std::pow(point.y, 2))) < _radius.get();
   }

 private:
   Radius _radius;
};

Point randomPoint(std::uniform_real_distribution<> &dis, std::mt19937 &gen)
{
   const auto x = dis(gen);
   const auto y = dis(gen);

   return { static_cast<float>(x), static_cast<float>(y) };
}

float calculatePi(std::uint64_t iterations, sf::RenderWindow &window)
{
   const auto [width, height] = window.getSize();

   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_real_distribution<> dis(-(static_cast<float>(width) / 4.0f), static_cast<float>(width) / 4.0f);

   const Circle circle{ static_cast<float>(width) / 4.0f };
   const Square square{ static_cast<std::uint16_t>(width / 2), static_cast<std::uint16_t>(width / 2) };

   circle.draw(window);
   square.draw(window);

   auto points_in_circle = 0;
   auto points_total = 0;

   for (std::uint64_t i = 0; i < iterations; ++i) {
      const auto point = randomPoint(dis, gen);

      if (circle.hasPoint(point)) {
         draw(point, window, sf::Color::Green);
         points_in_circle++;
      } else {
         draw(point, window, sf::Color::Blue);
      }
      points_total++;
   }

   const auto pi = 4 * static_cast<float>(points_in_circle) / static_cast<float>(points_total);
   return pi;
}

int main()
{
   static constexpr auto WIDTH = 800;
   static constexpr auto HEIGHT = 800;

   sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

   window.clear();

   const auto iterations = 100000;
   const auto pi = calculatePi(iterations, window);

   std::cout << "Monte carlo calculation of pi with " << iterations << ": " << pi << "\n";

   window.display();

   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed) window.close();
      }
   }

   return 0;
}
