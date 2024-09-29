#include "../include/the_cube/tile.h"


// temp constructor
the_cube::Tile::Tile(std::array<glm::vec3, 6> vertices){

      m_vertices = vertices;
      glGenVertexArrays(1, &vao);
      glGenBuffers(1, &vbo);
 
      glBindVertexArray(vao);
 
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices.data(), GL_STATIC_DRAW);
 
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
      glEnableVertexAttribArray(0);

      color = glm::vec3(1,1,1);
}

// pitch and roll angles supported , yaw not (not needed)
the_cube::Tile::Tile(glm::vec3 pos, glm::vec3 angle_vector){

      if(angle_vector.x != 0.f && angle_vector.z != 0.f){
          std::cerr << "YAW NOT SUPPORTED!\n";
          return;
      }
        
      angle_vector = glm::normalize(angle_vector);
      angle_vector *= (tile_length / 2.f);

      glGenVertexArrays(1, &vao);
      glGenBuffers(1, &vbo);
    
      // order is important cos of face culling + vertex indexing
      if(angle_vector.x == 0.f){
          m_vertices[0] = pos + (angle_vector + glm::vec3(-tile_length/2,0,0));
          m_vertices[1] = pos + (angle_vector + glm::vec3(tile_length/2,0,0));
          m_vertices[2] = pos + (-angle_vector + glm::vec3(tile_length/2,0,0));

          m_vertices[3] = pos + (angle_vector + glm::vec3(-tile_length/2,0,0));
          m_vertices[4] = pos + (-angle_vector + glm::vec3(tile_length/2,0,0));
          m_vertices[5] = pos + (-angle_vector + glm::vec3(-tile_length/2,0,0));
      }
      else if(angle_vector.z == 0.f){
          m_vertices[0] = pos + (-angle_vector + glm::vec3(0,0,tile_length/2));
          m_vertices[1] = pos + (angle_vector + glm::vec3(0,0,tile_length/2));
          m_vertices[2] = pos + (angle_vector + glm::vec3(0,0,-tile_length/2));

          m_vertices[3] = pos + (-angle_vector + glm::vec3(0,0,tile_length/2));
          m_vertices[4] = pos + (angle_vector + glm::vec3(0,0,-tile_length/2));
          m_vertices[5] = pos + (-angle_vector + glm::vec3(0,0,-tile_length/2));
      }

      glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);

      glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices.data(), GL_STATIC_DRAW);
 
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
      glEnableVertexAttribArray(0);


      color = glm::vec3(1,1,1);
}

void the_cube::Tile::render(){
    bind_vao();
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

glm::vec3 the_cube::Tile::get_color(){
    return color;
}

void the_cube::Tile::bind_vao(){
    glBindVertexArray(vao);
}

