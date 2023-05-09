#pragma once

#include <complex>
#include <cstdint>
#include <cstddef>
#include <optional>
#include <utility>

#include <SFML/Graphics/VertexArray.hpp>

class Mandelbrot {
   public:
      explicit Mandelbrot(const std::uint16_t number_of_tries);
      
      static std::complex<double> pixel_to_point(const std::pair<std::uint16_t, std::uint16_t>& bounds,
                                                 const std::pair<std::uint16_t, std::uint16_t>& pixel,
                                                 const std::complex<double>& upper_left,
                                                 const std::complex<double>& lower_right);

      sf::VertexArray createVertices(const std::pair<std::uint16_t, std::uint16_t>& bounds,
                                     const std::complex<double>& upper_left,
                                     const std::complex<double>& lower_right);
   private:
      void escape_time(const std::complex<double>& c);

      std::uint16_t number_of_tries_;
      std::size_t escape_time_{0};
};