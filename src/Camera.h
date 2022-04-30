#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "glm/glm.hpp"

const float PITCH           =  0.0f;
const float YAW             = -90.0f;
const float SPEED           =  2.5f;
const float SENSITIVITY     =  0.05f;
const float ZOOM            =  45.0f;

enum CameraMovement
{
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT
};

class Camera
{
public:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;

    float m_fYaw;
    float m_fPitch;
    float m_fMovementSpeed;
    float m_fMouseSensitivity;
    float m_fZoom;

public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
            float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    ~Camera();

    glm::mat4 GetViewMatrix();
    float Zoom() { return m_fZoom; }

    void ProcessKeyInput(CameraMovement direction, float deltaTime);
    void ProcessMouseMove(float xoffset, float yoffset, bool canstraintsPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};


#endif