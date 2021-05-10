#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>

class Animation 
{
public:
    Animation() = default;
    Animation(int x, int y,int width, int height) 
    {
        texture.loadFromFile("prof.png");
        for (int i = 0; i < nFrames; i++) 
        {
            frames[i] = { x + i * width,y,width,height };
        }
    }
    void ApplyToSprite(sf::Sprite& s) const 
    {
        s.setTextureRect(frames[iFrame]);//rect means rectangle
        s.setTexture(texture);
    }
    void Update(float dt) 
    {
        time += dt;
        while (time >= holdTime)
        {
            time -= holdTime;
            Advance();
        }
    }
private:
    void Advance() 
    {
        if (++iFrame >= nFrames) 
        {
            iFrame = 0;
        }
    }
    static constexpr float  holdTime = 0.1f;
    static constexpr int nFrames = 8;
    sf::Texture texture;
    sf::IntRect frames[nFrames];
    int iFrame = 0;
    float time = 0.0f;
};

class Character 
{
private:
    enum class AnimationIndex
    {
        walkUp,
        walkDown,
        walkLeft,
        walkRight,
        Count
    };
public:
    
    Character(const sf::Vector2f& pos)
        :pos(pos)
    {
        sprite.setTextureRect({ 0,0,64,64 });//set the square. 
        animations[int(AnimationIndex::walkUp)] = Animation(64, 0, 64, 64);
        animations[int(AnimationIndex::walkDown)] = Animation(64, 128, 64, 64);
        animations[int(AnimationIndex::walkLeft)] = Animation(64, 64, 64, 64);
        animations[int(AnimationIndex::walkRight)] = Animation(64, 192, 64, 64);
    }
    void Draw(sf::RenderTarget& rt) const //tomove
    {
        rt.draw(sprite);
    }
    void SetDirection(const sf::Vector2f& dir) 
    {
        velocity = dir * speed; // speed
        if (dir.x > 0.0f) 
        {
            curAnimation = AnimationIndex::walkRight;
        }
        else if (dir.x < 0.0f) 
        {
            curAnimation = AnimationIndex::walkLeft;
        }
        else if (dir.y < 0.0f)
        {
            curAnimation = AnimationIndex::walkUp;
        }
        else if (dir.y > 0.0f)
        {
            curAnimation = AnimationIndex::walkDown;
        }
    }
    void Update(float deltatime) 
    {
        pos += velocity * deltatime; // updation of the position
        animations[int(curAnimation)].Update(deltatime);
        animations[int(curAnimation)].ApplyToSprite(sprite);

        
        sprite.setPosition(pos);//moving of the sprite
    }   
private:
    static constexpr float speed = 100.0f;
    sf::Vector2f pos;   
    sf::Vector2f velocity;  
    sf::Sprite sprite;
    Animation animations[int(AnimationIndex::Count)];
    AnimationIndex curAnimation = AnimationIndex::walkUp;//first animation of all animation.
};

   int main()   
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    Character professor({ 100.0f,100.0f}); //initial position
    //timepoint for delta time measurement
    auto tp = std::chrono::steady_clock::now(); //start the time point
    
    while (window.isOpen())
    {   
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //get deltatime
        float dt;
        {
        const auto new_tp = std::chrono::steady_clock::now(); //start the time point
        dt = std::chrono::duration<float>(new_tp - tp).count();
        tp = new_tp;
        }
        //HandleInput
        sf::Vector2f dir(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            dir.y -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            dir.y += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            dir.x -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            dir.x += 1.0f;
        }
        professor.SetDirection  (dir);
        //update model
        professor.Update(dt);
        window.clear();
        professor.Draw(window);
        window.display();
    }
    return EXIT_SUCCESS;
}