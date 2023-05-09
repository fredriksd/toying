#include <complex>
#include <iostream>
#include <memory>
#include <utility>

#include "mandelbrot.hpp"

#include <SFML/Graphics.hpp>

static constexpr auto W_HEIGHT = 1000U;
static constexpr auto W_WIDTH = 1000U;

std::unique_ptr<sf::RenderWindow> createWindow()
{
   auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(W_HEIGHT, W_WIDTH), "test");
   window->setPosition(sf::Vector2i(900, 500));
   return window;
}

int main()
{
   auto mandelbrot = Mandelbrot{100};
   
   const auto vertices = mandelbrot.createVertices(std::pair<std::uint16_t, std::uint16_t>{W_WIDTH, W_HEIGHT},
                                                   std::complex<double>(-1.f, .2f),
                                                   std::complex<double>(-1.f, .2f));

   auto window = createWindow();
   window->draw(vertices);

   window->display();

   // run the program as long as the window is open
   while (window->isOpen())
   {
      // check all the window's events that were triggered since the last iteration of the loop
      sf::Event event;
      while (window->pollEvent(event))
      {
         // "close requested" event: we close the window
         if (event.type == sf::Event::Closed)
            window->close();
      }
   }
}
