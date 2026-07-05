#include "Hyperfocus.h"
#include "../core/Constants.h"

using namespace Constants;

void Hyperfocus::init(sf::Font& font)
{
    label.setFont(font);
    label.setCharacterSize(30);
    label.setFillColor(kHyperfocusLabelColor);
    label.setPosition(kHyperfocusLabelPos);
    label.setString("Hyperfocus");

    bar.init(kHyperfocusBarPos.x, kHyperfocusBarPos.y,
        kHyperfocusBarWidth, kHyperfocusBarHeight,
        /* cornerRadius */ 0.f,
        kHyperfocusBarBackground, kHyperfocusBarFill);
}

void Hyperfocus::update(float deltaTime, int& score)
{
    (void)score; // update() only reads the meter/timer; score changes happen in tryActivate()

    if (active)
    {
        timer += deltaTime;
        bar.setProgress(1.f); // full bar while hyperfocus is active

        if (timer >= kHyperfocusDuration)
        {
            active = false;
            timer = 0.f;
            progressScore = 0;
            bar.setProgress(0.f);
        }
    }
    else
    {
        bar.setProgress(static_cast<float>(progressScore) / static_cast<float>(kHyperfocusMaxProgress));
    }
}

void Hyperfocus::tryActivate(int& score, int gain)
{
    score += gain;

    if (active) return;

    progressScore += gain;

    if (progressScore >= kHyperfocusMaxProgress)
    {
        progressScore = kHyperfocusMaxProgress;
        active = true;
        timer = 0.f;
    }
}

void Hyperfocus::draw(sf::RenderWindow& window)
{
    window.draw(label);
    bar.draw(window);
}

void Hyperfocus::reset()
{
    active = false;
    timer = 0.f;
    progressScore = 0;
    bar.setProgress(0.f);
}