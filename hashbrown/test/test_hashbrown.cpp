#define CATCH_CONFIG_MAIN // This tells Catch to provide a main()

#include "catch2/catch.hpp"

#include <hashbrown.hpp>

TEST_CASE("Hash map can insert", "[hashbrown]") {
  auto map = HashBrown<int, int>();
  map.insert(1, 42);
  const auto value = map.get(1);

  REQUIRE(value != nullptr);
  REQUIRE(*value == 42);

  SECTION("Inserting on already inserted area is okay") {
    const auto old_value = *map.get(1);
    map.insert(1, 66);

    REQUIRE(old_value == 42);

    const auto new_value = map.get(1);
    REQUIRE(new_value != nullptr);
    REQUIRE(*new_value == 66);
  }
}

#ifdef REMOVE_ELEM_TEST
TEST_CASE("Can remove element from hash map", "[hashbrown]") {
  auto map = HashBrown<int, int>();
  map.insert(1, 42);
  map.erase(1);
  const auto value = map.get(1);
  REQUIRE(value == nullptr);
}
#endif

TEST_CASE("Hashmap is empty by default", "[hashbrown]") {
  auto map = HashBrown<int, int>();
  auto begin = map.begin();
  auto end = map.end();
  REQUIRE(begin == end);
  REQUIRE(map.empty());
}
