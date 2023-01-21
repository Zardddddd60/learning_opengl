#include <cmath>
#include <iostream>
#include <algorithm>
#include "camera.h"
#include "vendors/glm/gtc/matrix_transform.hpp"

const float DEFALUT_SPEED = 5.0f;
const float DEFALUT_FOV = 45.0f;
const float DEFALUT_SENSITIVYTY = 0.4f;

Camera::Camera()
    : m_Fov(DEFALUT_FOV),
        m_MovementSpeed(DEFALUT_SPEED),
        m_MouseSensitivity(DEFALUT_SENSITIVYTY)
{
    m_Position = glm::vec3(0.0f);
    m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_Yaw = -90.0f;
    m_Pitch = 0.0f;
    updateCameraVectors();
}

Camera::Camera(glm::vec3 position)
    : m_Fov(DEFALUT_FOV),
        m_MovementSpeed(DEFALUT_SPEED),
        m_MouseSensitivity(DEFALUT_SENSITIVYTY)
{
    m_Position = position;
    m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_Yaw = 45.0f;
    m_Pitch = -10.0f;
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // 计算Camera的朝向，根据pitch和yaw
    glm::vec3 front(1.0);
    front.y = sin(glm::radians(m_Pitch));
    float cosPitch = cos(glm::radians(m_Pitch));
    front.x = cosPitch * cos(glm::radians(m_Yaw));
    front.z = cosPitch * sin(glm::radians(m_Yaw));

    m_Front = glm::normalize(front);
    // 根据front和worldUp的叉乘求right，注意方向（叉乘顺序）
    m_CameraRight = glm::normalize(glm::cross(m_Front, m_WorldUp));
    // 根据right和
    m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_Front));
}

glm::mat4 Camera::getViewMatrix() const
{
    // view矩阵与postion，front和cameraup有关，其中up与front有关（worldup固定）
    return glm::lookAt(m_Position, m_Position +  m_Front, m_WorldUp);
}

// 改变camera postion
void Camera::processKeyBoard(CameraMovement direction, float deltatime)
{
    float velocity = deltatime * m_MovementSpeed;
    if (direction == FORWARD)
    {
        m_Position += velocity * m_Front;
    }
    if (direction == BACKWARD)
    {
        m_Position -= velocity * m_Front;
    }
    if (direction == LEFT)
    {
        m_Position -= velocity * m_CameraRight;
    }
    if (direction == RIGHT)
    {
        m_Position += velocity * m_CameraRight;
    }
    // m_Position.y = 0.0f;
}

void Camera::processMouseScroll(float yOffset)
{
    float tempFov = m_Fov - yOffset;
    m_Fov = std::max(std::min(90.0f, tempFov), 1.0f);
}

// 通过mouse移动时，x/y的差值，计算新的pitch和yaw，从而得到新的camera front
void Camera::processMouseMove(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= m_MouseSensitivity;
    yOffset *= m_MouseSensitivity;

    m_Pitch += yOffset;
    m_Yaw += xOffset;

    if (constrainPitch)
    {
        m_Pitch = std::max(std::min(m_Pitch, 89.0f), -89.0f);
    }

    // 触发front变化，是一个间接的过程，position变化是直接变化
    updateCameraVectors();
}

float Camera::getFov() const
{
    return m_Fov;
}

glm::vec3 Camera::getPosision() const
{
    return m_Position;
}
