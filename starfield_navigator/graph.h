#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

#include <glm/vec3.hpp>

#include "tools.h"


namespace sfn {

   enum class factions{uc, freestar, crimson};
   enum class info_quality{confirmed, speculation, unknown};

   struct system{
      std::string m_name;
      glm::vec3 m_position;
      info_quality m_info_quality = info_quality::unknown;

      // int m_level = 1;
      // std::string m_spectral_class;
      // int m_planet_count;
      // int m_moon_count;
      // factions m_faction;

      explicit system(const glm::vec3& pos, const std::string& name = "");
   };

   struct universe{
      std::vector<system> m_systems;

      [[nodiscard]] auto get_position_by_name(const std::string& name) const -> glm::vec3;
      [[nodiscard]] auto get_index_by_name(const std::string& name) const -> int;
   };




   struct node{
      std::string m_name;
      std::vector<id> m_connections;
      glm::vec3 m_position;
   };

   struct connection{
      int m_node_index0;
      int m_node_index1;
      float m_weight;
      
      [[nodiscard]] auto contains_node_index(const int node_index) const -> bool;
   };
   auto operator==(const connection& a, const connection& b) -> bool;

   struct shortest_path{
      // int m_target_index;
      constexpr static inline float no_distance = std::numeric_limits<float>::max();
      float m_shortest_distance = no_distance;
      int m_previous_vertex_index = -1;
   };
   [[nodiscard]] auto operator==(const shortest_path& a, const shortest_path& b) -> bool;
   

   struct shortest_path_tree{
      int m_source_node_index;
      std::vector<shortest_path> m_entries;

      explicit shortest_path_tree(const int source_node_index, const int node_count);
      [[nodiscard]] auto get_distance_from_source(const int node_index) const -> float;
      friend auto operator<=>(const shortest_path_tree&, const shortest_path_tree&) = default;
   };

   struct neighbor_info{
      float m_distance;
      int m_other_index;
   };

   struct jump_path{
      std::vector<int> m_stops;

      [[nodiscard]] auto contains_connection(const connection& con) const -> bool;
   };

   struct MyHashFunction {
      constexpr auto operator()(const id& p) const -> size_t{
         return p.m_id;
      }
   };

   struct graph
   {
      float m_jump_range = 0.0f;
      std::vector<node> m_nodes;
      std::unordered_map<id, connection, MyHashFunction> m_connections;

      explicit graph() = default;
      explicit graph(const universe& universe, const float jump_range);

      [[nodiscard]] auto get_node_index_by_name(const std::string& name) const -> int;
      auto add_connection(const std::string& name_a, const std::string& name_b, const float weight) -> void;
      [[nodiscard]] auto get_dijkstra(const int source_node_index) const -> shortest_path_tree;
      [[nodiscard]] auto are_neighbors(const int node_index_0, const int node_index_1) const -> bool;
      [[nodiscard]] auto get_neighbor_info(const int node_index_0, const int node_index_1) const -> neighbor_info;
      [[nodiscard]] auto get_jump_path(const std::string& start, const std::string& destination) const -> std::optional<jump_path>;
      
      auto print_path(const jump_path& path) const -> void;
      [[nodiscard]] auto get_closest(const std::string& system) const -> std::vector<int>;
   };

   [[nodiscard]] auto get_min_jump_dist(const universe& universe, const std::string& start, const std::string& destination) -> float;

}