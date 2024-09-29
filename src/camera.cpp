#include "../include/the_cube/camera.h"


glm::vec3 Camera::get_position(){
    return m_pos;
}

glm::mat4 Camera::get_view_matrix(){
      return glm::lookAt(this->get_position(),
                         glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::get_projection_matrix(){
      return glm::perspective(glm::radians(m_fov),
                              static_cast<float>(Screen::width)/Screen::height, 0.1f, 100.f);
}

void Camera::update_position(){
}
