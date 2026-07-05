#include "StoryState.h"
#include "../core/Constants.h"
#include "../core/ResourceManager.h"

using namespace Constants;

void StoryState::init(sf::Font& fontRef)
{
    backgroundTexture = TextureManager::instance().acquire(kStoryBackgroundPath);
    backgroundSprite.setTexture(*backgroundTexture);

    skipTexture = TextureManager::instance().acquire(kStorySkipPath);
    skipPressedTexture = TextureManager::instance().acquire(kStorySkipPressedPath);
    skipButton.init(skipTexture, skipPressedTexture, kStorySkipButtonWidth,
        kStorySkipButtonPos.x, kStorySkipButtonPos.y);

    nextTexture = TextureManager::instance().acquire(kStoryNextPath);
    nextPressedTexture = TextureManager::instance().acquire(kStoryNextPressedPath);
    nextButton.init(nextTexture, nextPressedTexture, kStoryNextButtonWidth,
        kStoryNextButtonPos.x, kStoryNextButtonPos.y);

    clickBuffer = SoundManager::instance().acquire(kUiClickSoundPath);
    clickSound.setBuffer(*clickBuffer);

    typingSoundBuffer = SoundManager::instance().acquire(kStoryTypingSoundPath);
    typingSound.setBuffer(*typingSoundBuffer);
    typingSound.setVolume(kStoryTypingSoundVolume);

    dialogueText.init(fontRef, kStoryTextSize, kStoryTextColor, kStoryTypewriterCharsPerSecond);
    dialogueText.setPosition(kStoryTextPos.x, kStoryTextPos.y);

   
    pages = {
        "Hi! Have you ever heard of ADHD?",
        "ADHD means Attention Deficit Hyperactivity Disorder.",
        "Distractions can turn simple tasks into big challenges.",
        "But Hyperfocus can feel like a superpower for a short time!",
        "In this game, we try to understand this experience.",
        "Run with Right, jump with Up, and collect laundry!",
        "Fill Hyperfocus to speed up.",
        "Use Grounding when overwhelmed.",
        "Wash your laundry before the laundry room closes!",
        "Have fun!",
    };
}

void StoryState::onEnter()
{
    goToPage(0);
}

void StoryState::goToPage(std::size_t pageIndex)
{
    currentPage = pageIndex;
    dialogueText.setFullText(pages[currentPage]);

    typingSound.stop();
    typingSound.setPlayingOffset(sf::seconds(kStoryTypingSoundStartOffset));
    typingSound.play();
}

void StoryState::onExit()
{
    
    typingSound.stop();
}

void StoryState::update(float deltaTime, sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    skipButton.update(mousePos);
    nextButton.update(mousePos);

    dialogueText.update(deltaTime);

    if (dialogueText.isComplete())
    {
        if (typingSound.getStatus() == sf::Sound::Playing)
        {
            typingSound.stop();
        }
    }
    else if (typingSound.getStatus() == sf::Sound::Playing &&
        typingSound.getPlayingOffset().asSeconds() >= kStoryTypingSoundUsableEnd)
    {
       
        typingSound.setPlayingOffset(sf::seconds(kStoryTypingSoundStartOffset));
    }
}

void StoryState::render(sf::RenderWindow& window)
{
    window.draw(backgroundSprite);
    dialogueText.draw(window);
    skipButton.draw(window);
    nextButton.draw(window);
}

void StoryState::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        handleMousePress(mousePos);
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        handleClick(mousePos); 
        handleMouseRelease();
    }
}

void StoryState::handleMousePress(const sf::Vector2f& mousePos)
{
    pressedButton = -1;
    if (skipButton.contains(mousePos))      pressedButton = 0;
    else if (nextButton.contains(mousePos)) pressedButton = 1;

    if (pressedButton != -1)
    {
        playClickSound();
    }
}

void StoryState::handleMouseRelease()
{
    pressedButton = -1;
}

void StoryState::handleClick(const sf::Vector2f& mousePos)
{
    if (pressedButton == 0 && skipButton.contains(mousePos))
    {
        finishedFlag = true;
    }
    else if (pressedButton == 1 && nextButton.contains(mousePos))
    {
        if (currentPage + 1 < pages.size())
        {
            goToPage(currentPage + 1);
        }
        else
        {
            
            finishedFlag = true;
        }
    }
}

void StoryState::playClickSound()
{
    if (clickBuffer && clickBuffer->getSampleCount() > 0)
    {
        clickSound.setPitch(kUiClickPitch);
        clickSound.setVolume(kUiClickVolume);
        clickSound.play();
    }
}

bool StoryState::wasFinished()
{
    if (finishedFlag)
    {
        finishedFlag = false;
        return true;
    }
    return false;
}