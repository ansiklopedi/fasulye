#include "FItem.h"

FItem::FItem(Vector2 position, Texture2D texture)
{
	this->position = position;
	this->texture = texture;

	tag = "item";
	SetBoundingBox(position.x, position.y, texture.width, texture.height);
	ActivateBoundingBox();

	FAnimationClip idle = FAnimationClip("resources/assets/chest.png", 1, 16, 16, 5, false, false);
	m_animation.AddAnimation("idle", idle);

	FAnimationClip open = FAnimationClip("resources/assets/chest_anim.png", 6, 16, 19, 5, false, true);
	m_animation.AddAnimation("open", open);

	m_animation.BindAnimation("idle");
}

FItem::~FItem()
{
	UnloadTexture(texture);
}

void FItem::Draw(float dt)
{
	m_animation.Animate(position, dt);
}

void FItem::Physics(std::string tag)
{
	if (tag == "ammo")
	{
		m_animation.BindAnimation("open");
	}
}
