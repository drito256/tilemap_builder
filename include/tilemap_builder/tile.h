#ifndef TILE_H
#define TILE_H

#include <array>
#include <iostream>

#include "../glad/glad.h"
#include "../glm/glm.hpp"

namespace tilemap_builder{
    class Tile{
        private:
            std::array<glm::vec3, 6> m_vertices;

            unsigned int vao;
            unsigned int vbo;

            glm::vec3 color;
            float tile_length = 0.5f;

            void bind_vao();


        public:
            Tile(std::array<glm::vec3, 6> vertices);
            Tile(glm::vec3 pos, glm::vec3 angle_vector = glm::vec3(1,0,0));

            void render();
            glm::vec3 get_color();
    };
}


#endif
