#include "../include/the_cube/cube.h"


the_cube::Cube::Cube() : Cube(glm::vec3(0.f), 1.f){}

the_cube::Cube::~Cube(){
//    delete_buffers();
}

// maybe a good idea to check if non-cube has been passed through vertices
the_cube::Cube::Cube(std::array<glm::vec3, 8> vertices, std::array<int,36> indices){
    m_vertices = vertices;
    m_indices = indices;

    // find length of edge regardless of vertices init order
    float max{vertices[0].x}, min{vertices[0].x};
    float x_avg{0}, y_avg{0}, z_avg{0};
    for(int i=0;i<vertices.size();i++){
        max = std::max(max, vertices[i].x);
        min = std::min(min, vertices[i].x);
        
        x_avg += vertices[i].x;
        y_avg += vertices[i].y;
        z_avg += vertices[i].z;
    }
    a = max - min;

    x_avg /= vertices.size();
    y_avg /= vertices.size();
    z_avg /= vertices.size();
    model_matrix = glm::translate(model_matrix,glm::vec3(x_avg, y_avg, z_avg));

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    scale(0.5f);
}

// position is center of the cube
the_cube::Cube::Cube(glm::vec3 position, float edge_length){
    a = edge_length;
    model_matrix = glm::mat4(1.0f);
    update_position(position);

    // lower 4 vertices
    m_vertices[0] = -glm::vec3(edge_length/2);
    m_vertices[1] = -glm::vec3(edge_length/2, edge_length/2, -edge_length/2);
    m_vertices[2] = -glm::vec3(-edge_length/2, edge_length/2, -edge_length/2);
    m_vertices[3] = -glm::vec3(-edge_length/2, edge_length/2, edge_length/2);

    // upper 4
    m_vertices[4] =  glm::vec3(edge_length/2);
    m_vertices[5] =  glm::vec3(edge_length/2, edge_length/2, -edge_length/2);
    m_vertices[6] =  glm::vec3(-edge_length/2, edge_length/2, -edge_length/2);
    m_vertices[7] =  glm::vec3(-edge_length/2, edge_length/2, edge_length/2);

    // order is important because of face culling
    m_indices = {
        1,2,4,
        1,4,7,

        2,3,5,
        2,5,4,

        3,6,5,
        3,0,6,

        7,6,0,
        7,0,1,

        6,7,4,
        6,4,5,

        2,1,0,
        3,2,0
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    scale(0.5f);
}

glm::vec3 the_cube::Cube::get_position(){
    return glm::vec3(model_matrix[3][0],
                     model_matrix[3][1],
                     model_matrix[3][2]);
}

void the_cube::Cube::reset_position(glm::vec3 pos){
    model_matrix[3][0] = pos.x;
    model_matrix[3][1] = pos.y;
    model_matrix[3][2] = pos.z;
}

void the_cube::Cube::update_position(glm::vec3 pos){
    model_matrix[3][0] += pos.x;
    model_matrix[3][1] += pos.y;
    model_matrix[3][2] += pos.z;
}

glm::mat4 the_cube::Cube::get_model_matrix(){
    return model_matrix;
}

void the_cube::Cube::rotate(int degrees, the_cube::RotationDirection rd){
    float angle = glm::radians(static_cast<float>(degrees));

    glm::vec3 axis;
    glm::vec3 pos = get_position();
    update_position(-pos);
    glm::vec3 direction = glm::vec3(0.f);
    
    float translation_distance = (a/2) * angle;
    switch(rd){
        case the_cube::RotationDirection::FORWARD:
        axis = glm::vec3(0,0,-1.f);
        direction = glm::vec3(1.0f,0.0f,0.0f);
        break;
        case the_cube::RotationDirection::BACKWARD:
        axis = glm::vec3(0,0,1.f);
        direction = glm::vec3(-1.0f,0.0f,0.0f);
       break;
        case the_cube::RotationDirection::LEFT:
        axis = glm::vec3(-1.0f,0,0.f);
        direction = glm::vec3(0.0f,0.0f,-1.0f);
        break;
        case the_cube::RotationDirection::RIGHT:
        axis = glm::vec3(1.0f,0,0.f);
        direction = glm::vec3(0.0f,0.0f,1.0f);
        break;
    }

    model_matrix = glm::rotate(model_matrix, angle, axis);
    update_position(pos); // reset position to old coords
    update_position(translation_distance * direction);
}

// will have to roll it around edges of tiles, not around its own base
void the_cube::Cube::roll(int degrees, the_cube::RotationDirection rd){
    float angle = glm::radians(static_cast<float>(degrees));
    
    glm::vec3 axis;
    glm::vec3 pos = get_position();
    update_position(-pos);
    glm::vec3 direction = glm::vec3(0.f);
    //float translation_distance = (a/2) * angle;

    std::array<glm::vec3, 8> vertices{m_vertices};
    for(int i=0;i<vertices.size();i++){
        vertices[i] = glm::mat3(model_matrix) * vertices[i];
    }

    auto yxz_sort = [](const glm::vec3& a, const glm::vec3& b){
        if(a.y != b.y) return a.y < b.y;
        if(a.x != b.x) return a.x > b.x;

        return a.z < b.z;
    };
    auto yzx_sort = [](const glm::vec3& a, const glm::vec3& b){
        if(a.y != b.y) return a.y < b.y;
        if(a.x != b.x) return a.z < b.z;

        return a.x > b.x;
    };

    std::sort(vertices.begin(), vertices.end(), yxz_sort);
    std::array<glm::vec3, 4> base_edges{vertices[0], vertices[1], vertices[2], vertices[3]};

    switch(rd){
        case the_cube::RotationDirection::FORWARD:
        std::sort(vertices.begin(), vertices.end(), yxz_sort);
        base_edges = {vertices[0], vertices[1], vertices[2], vertices[3]};

        model_matrix = rotateAroundLine(model_matrix, base_edges[1], base_edges[0], angle);
        break;
        case the_cube::RotationDirection::BACKWARD:
        std::sort(vertices.begin(), vertices.end(), yxz_sort);
        base_edges = {vertices[0], vertices[1], vertices[2], vertices[3]};
        
        model_matrix = rotateAroundLine(model_matrix, base_edges[0], base_edges[1], angle);
       break;
        case the_cube::RotationDirection::LEFT:
        std::sort(vertices.begin(), vertices.end(), yzx_sort);
        base_edges = {vertices[0], vertices[1], vertices[2], vertices[3]};
        
        model_matrix = rotateAroundLine(model_matrix, base_edges[1], base_edges[0], angle);
        break;
        case the_cube::RotationDirection::RIGHT:
        std::sort(vertices.begin(), vertices.end(), yzx_sort);
        base_edges = {vertices[0], vertices[1], vertices[2], vertices[3]};
        
        model_matrix = rotateAroundLine(model_matrix, base_edges[0], base_edges[1], angle);
        break;
    }

    update_position(pos); // reset position to old coords
    //std::cout << model_matrix[3][1] << std::endl;
}


glm::mat4 the_cube::Cube::rotateAroundLine(const glm::mat4& modelMatrix, const glm::vec3& point1, const glm::vec3& point2, float angle) {
    // Calculate the direction vector (axis of rotation) and normalize it
    glm::vec3 axis = glm::normalize(point2 - point1);
    
    // Step 1: Translate the model to align the rotation axis with the origin
    glm::mat4 translationToOrigin = glm::translate(glm::mat4(1.0f), -point1);
    
    // Step 2: Rotate around the axis (the line between point1 and point2)
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
    
    // Step 3: Translate the model back to its original position
    glm::mat4 translationBack = glm::translate(glm::mat4(1.0f), point1);
    
    // Combine the transformations: translate to origin, rotate, then translate back
    glm::mat4 result = translationBack * rotation * translationToOrigin * modelMatrix;
    
    return result;
}

void the_cube::Cube::scale(float factor){
    model_matrix = glm::scale(model_matrix, glm::vec3(factor));
}

void the_cube::Cube::render(){
    bind_vao();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void the_cube::Cube::bind_vao(){
    glBindVertexArray(vao);
}

void the_cube::Cube::delete_buffers(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
