#include "SettingsState.h"
#include "../core/Constants.h"
#include "../core/ResourceManager.h"
#include "../core/AudioSettings.h"

using namespace Constants;

void SettingsState::init()
{
    dimOverlay.setSize(sf::Vector2f(static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight)));
    dimOverlay.setFillColor(kSettingsDimColor);
    dimOverlay.setPosition(0.f, 0.f);

    //  Panel image, centered 
    panelTexture = TextureManager::instance().acquire(kSettingsPanelTexturePath);
    panelTexture->setSmooth(true);
    panelSprite.setTexture(*panelTexture);

    sf::Vector2u size = panelTexture->getSize();
    float scale = (size.x > 0) ? kSettingsPanelTargetWidth / static_cast<float>(size.x) : 1.f;
    panelSprite.setScale(scale, scale);
    panelSprite.setOrigin(size.x / 2.f, size.y / 2.f);
    panelSprite.setPosition(kSettingsPanelPos);

    sf::FloatRect panel = panelSprite.getGlobalBounds();


    float sliderX = panel.left + panel.width * kSettingsSliderXRatio;
    float sliderW = panel.width * kSettingsSliderWidthRatio;
    float soundY = panel.top + panel.height * kSettingsSoundRowYRatio;
    float musicY = panel.top + panel.height * kSettingsMusicRowYRatio;

    soundSlider.init(sliderX, soundY, sliderW, kSettingsSliderHeight,
        kSettingsSliderTrack, kSettingsSliderFill, kSettingsSliderKnob,
        AudioSettings::instance().sfxVolume01);

    musicSlider.init(sliderX, musicY, sliderW, kSettingsSliderHeight,
        kSettingsSliderTrack, kSettingsSliderFill, kSettingsSliderKnob,
        AudioSettings::instance().musicVolume01);

    okayTexture = TextureManager::instance().acquire(kSettingsOkayTexturePath);
    okayTexture->setSmooth(true);
    okayPressedTexture = TextureManager::instance().acquire(kSettingsOkayPressedTexturePath);
    okayPressedTexture->setSmooth(true);

    float okayX = panel.left + panel.width / 2.f;
    float okayY = panel.top + panel.height * kSettingsOkayYRatio;
    okayButton.init(okayTexture, okayPressedTexture, kSettingsOkayWidth, okayX, okayY);
}

void SettingsState::onEnter()
{
    soundSlider.setValue(AudioSettings::instance().sfxVolume01);
    musicSlider.setValue(AudioSettings::instance().musicVolume01);
    okayPressed = false;
}

void SettingsState::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    soundSlider.handleEvent(event, window);
    musicSlider.handleEvent(event, window);

 
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        okayPressed = okayButton.contains(mousePos);
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        if (okayPressed && okayButton.contains(mousePos))
        {
            closedFlag = true;
        }
        okayPressed = false;
    }
}

void SettingsState::update(float deltaTime, sf::RenderWindow& window)
{
    (void)deltaTime;

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    okayButton.update(mousePos, 0.9f); 

    AudioSettings::instance().sfxVolume01 = soundSlider.value();
    AudioSettings::instance().musicVolume01 = musicSlider.value();
}

void SettingsState::render(sf::RenderWindow& window)
{
    window.draw(dimOverlay);
    window.draw(panelSprite);
    soundSlider.draw(window);
    musicSlider.draw(window);
    okayButton.draw(window);
}

bool SettingsState::wasClosed()
{
    if (closedFlag)
    {
        closedFlag = false;
        return true;
    }
    return false;
}