#include "mandelbrot.hpp"

#include <cassert>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

Mandelbrot::Mandelbrot(const std::uint16_t number_of_tries)
   : number_of_tries_{number_of_tries}
{
}

void Mandelbrot::escape_time(const std::complex<double>& c)
{
   auto z = std::complex<double>();
   constexpr auto IMAGINARY_CIRCLE_RADIUS = 2.0;

   for(std::size_t i = 0; i < number_of_tries_; i++)
   {
      if (std::norm(z) > IMAGINARY_CIRCLE_RADIUS)
      {
         escape_time_ = i;
         return;
      }
      z = z * z + c;
   }
}

std::complex<double> Mandelbrot::pixel_to_point(const std::pair<std::uint16_t, std::uint16_t>& bounds,
                                                const std::pair<std::uint16_t, std::uint16_t>& pixel,
                                                const std::complex<double>& upper_left,
                                                const std::complex<double>& lower_right)
{
   const auto width = lower_right.real() - upper_left.real();
   const auto height = lower_right.imag() - upper_left.imag();
   return std::complex<double>
   {
      upper_left.real() + (static_cast<double>(pixel.first) / static_cast<double>(bounds.first)),
      upper_left.imag() + (static_cast<double>(pixel.second) / static_cast<double>(bounds.second)),
   };
}

sf::VertexArray Mandelbrot::createVertices(const std::pair<std::uint16_t, std::uint16_t>& bounds,
                                           const std::complex<double>& upper_left,
                                           const std::complex<double>& lower_right)
{
   sf::VertexArray vertices(sf::Points, static_cast<std::size_t>(bounds.first * bounds.second));

   for (auto row = 0; row < bounds.second; ++row)
   {
      for (auto column = 0; column < bounds.first; ++column)
      {
         const auto point = Mandelbrot::pixel_to_point(bounds, std::pair<std::uint16_t, std::uint16_t>{column, row}, upper_left, lower_right);
         escape_time(point);

         const auto color = sf::Color
         {
            static_cast<std::uint8_t>(255 - escape_time_),
            static_cast<std::uint8_t>(255 - escape_time_),
            static_cast<std::uint8_t>(255 - escape_time_)
         };
         vertices.append(sf::Vertex({sf::Vector2f{static_cast<float>(row), static_cast<float>(column)}, color}));
      }
   }
   return vertices;
}