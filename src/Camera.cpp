
#include <Camera.h>

CameraActor::CameraActor(
    const std::string& name,
    glm::vec3 position,
    glm::quat rotation,
    float aspectRatio,
    float nearPlane,
    float farPlane,
    float fieldOfView,
    float maxMovementSpeed,
    float accelerationSpeed,
    float dampingFactor)
    : Actor(name),
    mMaxMovementSpeed(maxMovementSpeed),
    mAccelerationSpeed(accelerationSpeed),
    mDampingFactor(dampingFactor),
    mAspectRatio(aspectRatio),
    mNearPlane(nearPlane),
    mFarPlane(farPlane),
    mFieldOfView(fieldOfView)
{
    SetPosition(position);
    SetRotation(rotation);
    UpdateProjectionMatrix();
}

void CameraActor::Update(float _dt)
{
    UpdateVelocity(_dt);
    UpdatePosition(_dt);
    UpdateDamping(_dt);
    UpdateAngularVelocity(_dt);
    UpdateRotation(_dt);
    UpdateAngularDamping(_dt);
}

void CameraActor::UpdateVelocity(float _dt)
{
    mVelocity += mAcceleration * _dt;

    // if velocity is faster than max movement speed, limit to max movement speed
    if (glm::length(mVelocity) > mMaxMovementSpeed)
    {
        mVelocity = glm::normalize(mVelocity) * mMaxMovementSpeed;
    }
}

void CameraActor::UpdateDamping(float dt)
{
    // Reduces the acceleration based on itself * damping factor over time 
    float dampingFactor = glm::length(mVelocity) > 0 ? mDampingFactor : 0.0f;

    mVelocity -= mVelocity * dampingFactor * dt;

    // if velocity is low, Set velocity to 0.
    if (glm::length(mVelocity) < 0.001f)
    {
        mVelocity = glm::vec3(0.0f);
    }
}

void CameraActor::UpdateAngularDamping(float dt)
{
    // same as regular damping but for angles.
    mAngularAcceleration = glm::vec2(0.0f);

    if (glm::length(mAngularVelocity) > 0)
    {
        mAngularVelocity -= mAngularVelocity * mAngularDampingFactor * dt;
        if (glm::length(mAngularVelocity) < 0.001f)
        {
            mAngularVelocity = glm::vec2(0.0f);
        }
    }
}

void CameraActor::UpdateAngularVelocity(float dt)
{
    mAngularVelocity += mAngularAcceleration * dt;
    // Clamp angular velocity to the maximum allowed
    mAngularVelocity = glm::clamp(mAngularVelocity, -mMaxAngularSpeed, mMaxAngularSpeed);
}

void CameraActor::UpdateRotation(float dt)
{
    AddYawDegrees(mAngularVelocity.x * dt);
    AddPitchDegrees(mAngularVelocity.y * dt);
}

void CameraActor::UpdatePosition(float dt)
{
    glm::vec3 front = GetFront();
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::cross(right, front);

    SetPosition(GetPosition(Actor::TransformSpace::Local) + mVelocity.x * dt * right + mVelocity.y * dt * up + mVelocity.z * dt * front);
}

void CameraActor::UpdateProjectionMatrix()
{
    mProjectionMatrix = glm::perspectiveRH(glm::radians(mFieldOfView), mAspectRatio, mNearPlane, mFarPlane);
}

void CameraActor::UpdateRotationFromYawPitch()
{
    glm::quat pitchQuat = glm::angleAxis(glm::radians(mPitchDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat yawQuat = glm::angleAxis(glm::radians(mYawDegrees), glm::vec3(0.0f, 1.0f, 0.0f));

    // Combined rotation
    glm::quat newRotation = yawQuat * pitchQuat;
    newRotation = glm::normalize(newRotation); // Ensure the quaternion is normalized

    // Assuming SetRotation directly sets the Transform's rotation
    this->SetRotation(newRotation);
}