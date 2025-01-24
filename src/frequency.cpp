/* 
 * Copyright (C) 2025 toytoi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "frequency.h"
#include <fstream>
#include <iostream>

using namespace std::string_literals;

static constexpr std::string_view default_dict_path{"/usr/share/gd-frequency/jpdb.json"};
static constexpr std::string_view default_bin_path{"/usr/share/gd-frequency/dict.bin"};
static constexpr std::string_view help_text = R"EOF(usage: gd-frequency [OPTIONS]

Find frequency.

OPTIONS  
   --dict-path /path/to/dict      path to frequency dictionary
   --bin-path /path/to/bin        path to save bin data
   --word WORD                    search term

EXAMPLES
   gd-frequency --dict-path ./dict.json --word 歌う
   gd-frequency --word 鹿
)EOF";
static constexpr std::string_view css_style = R"EOF(
   <style>
   .gd_frequency {
      font-size: 1.5rem;
      border-left: 4px solid black;
      padding-left: 10px;
   }

   .gd_word_frequency {
      text-decoration: underline;
   }
   </style>
   )EOF"; 

struct frequency_params {
   std::string path_to_dict{ default_dict_path };
   std::string path_to_bin{ default_bin_path };
   std::string gd_word{};

   void assign (int const argc, char* argv[]) {
      if (argc < 3 || argc % 2 == 0) {
         throw std::invalid_argument("Invalid arguments.");
      }
      for (int i = 1; i < argc; i+=2) {
         std::string key = argv[i];
         if (i + 1 >= argc) {
            throw std::invalid_argument("Missing value for argument: " + key);
         }

         std::string value = argv[i + 1];

         if (key == "--dict-path") {
            path_to_dict = value;
         } else if (key == "--bin-path") {
            path_to_bin = value;
         } else if (key == "--word") {
            gd_word = value;
         } else {
            throw std::invalid_argument("Unknown argument: " + key);
         }
      }
   }
};

// load bin file of map
auto load_frequency_map(const std::string& filepath) {
   frequency_map map;
   std::ifstream in(filepath, std::ios::binary);
   try {
      if (in.is_open()) {
         size_t size;
         in.read(reinterpret_cast<char*>(&size), sizeof(size));
         for (size_t i = 0; i < size; ++i) {
            size_t key_size;
            in.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
            std::string key(key_size, '\0');
            in.read(key.data(), key_size);
            int64_t value;
            in.read(reinterpret_cast<char*>(&value), sizeof(value));
            map[key] = value;
         }
      } else {
      }
   } catch (const std::exception& e) {
      std::cerr << "Error loading map bin: " << e.what() << std::endl;
   }
   return map;
}

// save map as bin
void save_frequency_map(const frequency_map& map, const std::string& filepath) {
   try {
      std::ofstream out(filepath, std::ios::binary);
      if (out.is_open()) {
         size_t size = map.size();
         out.write(reinterpret_cast<const char*>(&size), sizeof(size));
         for (const auto& [key, value] : map) {
            size_t key_size = key.size();
            out.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
            out.write(key.data(), key_size);
            out.write(reinterpret_cast<const char*>(&value), sizeof(value));
         }
      } else {
         std::cerr << "Failed to open file for saving." << std::endl;
      }
   } catch (const std::exception& e) {
      std::cerr << "Error saving map: " << e.what() << std::endl;
   }
}

auto build_frequency_map(const json& frequency_dict) {
   frequency_map map;
   for (const auto& entry : frequency_dict) {
      std::optional<std::string> word;
      std::optional<int64_t> frequency;
      if (entry[0].is_string()) {
         word = entry[0];
      }
      if (entry[2].contains("value")) {
         frequency = entry[2]["value"];
      } else if (entry[2]["frequency"].contains("value")) {
         frequency = entry[2]["frequency"]["value"];
      }

      if (word && frequency) {
         // discard duplicates
         if (map.find(*word) == map.end()) {
            map[*word] = *frequency;
         }
      } else {
         std::cerr << "Invalid entry: " << entry.dump() << std::endl;
      }
   }
   return map;
}

void search_map(const frequency_map& map, const std::string& word) {
   auto it = map.find(word);
   if (it != map.end()) {
      std::cout << "<div class=\"gd_frequency\">" <<
      word << ": <span class=\"gd_word_frequency\">" << it->second << "</span></div>";
      std::cout << css_style;
   }
   else {
      std::cout << "<div class=\"gd_frequency\">" <<
      "No matching entries." << "</div>" << std::endl;
      std::cout << css_style;
   }
}

void find_frequency(frequency_params const& params) {
   frequency_map frequency_map = load_frequency_map(params.path_to_bin);
   // if bin does not exist create new map
   if (frequency_map.empty()) {
      std::ifstream input(params.path_to_dict);
      if (!input.is_open()) {
         std::cerr << "Failed to open file: " << params.path_to_dict << std::endl;
      }
      json frequency_dict;
      input >> frequency_dict;

      frequency_map = build_frequency_map(frequency_dict);
      save_frequency_map(frequency_map, params.path_to_bin);
   }

   search_map(frequency_map, params.gd_word);
}

void frequency(int const argc, char* argv[]) {
   try {
      frequency_params params;
      params.assign(argc, argv);

      find_frequency(params);
   } catch (const std::invalid_argument& e) {
      std::cout << help_text;
   }
}

