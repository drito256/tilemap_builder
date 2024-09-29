#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <array>
#include <algorithm>

#include "../glad/glad.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "constants.h"

namespace the_cube{

    enum class RotationDirection{
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    class Cube{
        private:
            std::array<glm::vec3, 8> m_vertices;
            std::array<int, 36> m_indices;

            float a{0}; // edge length
            glm::mat4 model_matrix{glm::mat4(1.f)}; // identity
            
            unsigned int vbo; 
            unsigned int vao; 
            unsigned int ebo; 
            
            glm::mat4 rotateAroundLine(const glm::mat4& model, const glm::vec3& point1,
                                                               const glm::vec3& point2,
                                                               float angle);
            glm::vec3 get_rotation_axis(RotationDirection rd);

            void delete_buffers();
            void bind_vao();

            // kasnije boja,tekstura ...

        public:
            Cube();
            ~Cube();
            Cube(std::array<glm::vec3,8> vertices, std::array<int,36> indices);
            Cube(glm::vec3 position, float edge_length = 1.0f);

            void rotate(int degrees, RotationDirection rd);
            void roll(int degrees, RotationDirection rd);
            void reset_position(glm::vec3 pos = glm::vec3(0.0f));
            void update_position(glm::vec3 pos);
            void scale(float factor);

            glm::vec3 get_position();
            glm::mat4 get_model_matrix();

            void render();
    };
}

#endif
