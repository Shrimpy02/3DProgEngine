#pragma once
#include <string>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include "Transform.h"
#include "Tag.h"

class Actor
{
public:
	// ---------- Global Variables --------------

	enum class TransformSpace
	{
		Local,
		Global
	};

private:
	// ---------- Local Variables --------------

	Actor* mParent = nullptr;
	std::vector<Actor*> mChildren;

	TagUnique mTag;
	Transform mTransform{};

public:
	// ---------- Global functions --------------

	// name can go into tag at some point
	Actor(const std::string& _name) : mTag(_name), mParent(nullptr) {};

	~Actor() = default;

	virtual void Update(float dt);

	void AddChild(Actor* _child);
	void RemoveChild(Actor* _child);

private:
	// ---------- Local functions --------------



public:
	// ---------- Getters and setters --------------

	// Getters
	const std::string& GetTag() { return mTag.GetValue(); }

	void SetParent(Actor* _parent);
	void SetTransform(const Transform& _transform);
	void SetPosition(const glm::vec3& _position, Actor::TransformSpace _type = Actor::TransformSpace::Local);
	void SetRotation(const glm::quat& _rotation, Actor::TransformSpace _type = Actor::TransformSpace::Local);
	void SetScale(const glm::vec3& _scale, Actor::TransformSpace _type = Actor::TransformSpace::Local);

	const glm::vec3& GetPosition(Actor::TransformSpace _type) const;

	const glm::quat& GetRotation(Actor::TransformSpace _type) const;

	const glm::vec3& GetScale(Actor::TransformSpace _type) const;

	const Transform& GetTransform() const;
	const glm::mat4 GetTransformMatrix(Actor::TransformSpace _type) const;

	Transform* GetTransform() { return &mTransform; }

	std::vector<Actor*>& GetChildren();
	glm::vec3 GetRight() const;

	template <typename T>
	void Query(std::vector<Actor*>& actors)
	{
		if (dynamic_cast<T*>(this))
			actors.emplace_back(this);

		for (auto child : this->mChildren)
			child->Query<T>(actors);
	}

};
