#include <hashbrown.hpp>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <iostream>

int main()
{
   nlohmann::json j;
   HashBrown<int, std::string> dict; // = {{1, "one"}, {2, "two"}};
   dict.insert({3, "three"});
   dict.insert(std::make_pair(4, "four"));
   dict.insert({{4, "another four"}, {5, "five"}});
   
   std::cout << "contents:\n";
   for(auto& p: dict)
       std::cout << " " << p.first << " => " << p.second << '\n';
}
