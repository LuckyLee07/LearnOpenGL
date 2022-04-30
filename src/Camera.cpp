#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_fZoom(ZOOM), m_fMovementSpeed(SPEED), m_fMouseSensitivity(SENSITIVITY), m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
{
    m_Position = position;
    m_WorldUp = up;
    m_fYaw = yaw;
    m_fPitch = pitch;
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
    m_Position = glm::vec3(posX, posY, posZ);
    m_WorldUp = glm::vec3(upX, upY, upZ);
    m_fYaw = yaw;
    m_fPitch = pitch;
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_WorldUp);
}

void Camera::ProcessKeyInput(CameraMovement direction, float deltaTime)
{
    float velocity = m_fMovementSpeed * deltaTime;
    if(direction == MOVE_FORWARD)
        m_Position += m_Front * velocity;

    if(direction == MOVE_BACKWARD)
        m_Position -= m_Front * velocity;

    if(direction == MOVE_LEFT)
        m_Position -= glm::normalize(glm::cross(m_Front, m_WorldUp)) * velocity;

    if(direction == MOVE_RIGHT)
        m_Position += glm::normalize(glm::cross(m_Front, m_WorldUp)) * velocity;
    
    //m_Position.y = 0.0f; //this one-liner keeps the user at the ground level (xz plane)
}

void Camera::ProcessMouseMove(float xoffset, float yoffset, bool canstraintsPitch)
{
    xoffset *= m_fMouseSensitivity;
    yoffset *= m_fMouseSensitivity;

    m_fYaw     += xoffset;
    m_fPitch   += yoffset;

    if (canstraintsPitch)
    {    
        if (m_fPitch > 89.0f)
            m_fPitch = 89.0f;
        if (m_fPitch < -89.0f)
            m_fPitch = -89.0f;    
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (m_fZoom >= 1.0f && m_fZoom <= 90.0f)
        m_fZoom -= yoffset;

    if (m_fZoom <= 1.0f) m_fZoom = 1.0f;
    if (m_fZoom >= 90.0f) m_fZoom = 90.0f;
}

void Camera::updateCameraVectors()
{
    glm::vec3 frontTemp;
    frontTemp.x = cos(glm::radians(m_fPitch)) * cos(glm::radians(m_fYaw));
    frontTemp.y = sin(glm::radians(m_fPitch));
    frontTemp.z = cos(glm::radians(m_fPitch)) * sin(glm::radians(m_fYaw));
    m_Front = glm::normalize(frontTemp);
}
