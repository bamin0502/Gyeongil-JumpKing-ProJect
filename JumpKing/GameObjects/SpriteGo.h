#pragma once

class SpriteGo : public GameObject
{
protected:
	std::string textureId;

public:
	sf::Sprite sprite;
	SpriteGo(const std::string& name = "");

	sf::FloatRect GetLocalBounds() override;
	sf::FloatRect GetGlobalBounds() override;
	
	
	void SetTexture(const std::string& textureId);

	void SetPosition(const sf::Vector2f& pos) override;
	void Translate(const sf::Vector2f& delta) override;
	void SetRotation(float r) override;


	void SetOrigin(Origins newOrigin) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;
	void SetScale(const sf::Vector2f& scale) override;
	void SetFlipX(bool filp) override;
	void SetFlipY(bool filp) override;
	void SetAlpha(float alpha);

	const sf::Texture* GetSprite() const { return sprite.getTexture(); }

	void Reset() override;
	void Draw(sf::RenderWindow& window) override;

	
};

