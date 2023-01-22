#pragma once

#include "vendors/glm/glm.hpp"

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

class Camera
{
private:
    // 计算look at
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_WorldUp;

    // 当前look at的右向量和上向量
    glm::vec3 m_CameraUp;
    glm::vec3 m_CameraRight;

    // 欧拉角
    float m_Pitch;
    float m_Yaw;

    // 变化分量
    float m_Fov; // zoom
    float m_MovementSpeed; // postion变化
    float m_MouseSensitivity; // pitch/yaw变化

    // 私有函数
    void updateCameraVectors();
public:
    Camera();
    Camera(glm::vec3 position);
    glm::mat4 getViewMatrix() const;
    void processKeyBoard(CameraMovement direction, float deltatime);
    void processMouseScroll(float yOffset);
    void processMouseMove(float xOffset, float yOffset, bool constrainPitch = true);
    float getFov() const;
    glm::vec3 getPosision() const;
    glm::vec3 getFront() const;
};
