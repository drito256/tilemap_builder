#ifndef CAMERA_H
#define CAMERA_H

#include "../glm/glm.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "constants.h"

class Camera{
    private:
        glm::vec3 m_pos;
        float m_fov;

    public:
        Camera(glm::vec3 position, float fov)
        :m_pos{position}
        ,m_fov{fov}
        {}
        
        glm::mat4 get_view_matrix();
        glm::mat4 get_projection_matrix();
        glm::vec3 get_position();

        void update_position();
};

#endif
