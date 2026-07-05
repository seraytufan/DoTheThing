#include "BoxTraceProgress.h"
#include <algorithm>

void BoxTraceProgress::init(float x, float y, float width, float height, float inset, float strokeThickness, float cornerRadius, sf::Color color)
{
    thickness = strokeThickness;
    capRadius = cornerRadius;

    float left = x + inset;
    float right = x + width - inset;
    float top = y + inset;
    float bottom = y + height - inset;

  
    segments[0].start = { left, top };     segments[0].end = { right, top };    segments[0].horizontal = true;  // top: L->R
    segments[1].start = { right, top };    segments[1].end = { right, bottom }; segments[1].horizontal = false; // right: T->B
    segments[2].start = { right, bottom }; segments[2].end = { left, bottom };  segments[2].horizontal = true;  // bottom: R->L
    segments[3].start = { left, bottom };  segments[3].end = { left, top };     segments[3].horizontal = false; // left: B->T

    for (auto& seg : segments)
    {
        seg.bar.setFillColor(color);
        seg.startCap.setRadius(capRadius);
        seg.startCap.setOrigin(capRadius, capRadius);
        seg.startCap.setFillColor(color);
        seg.endCap.setRadius(capRadius);
        seg.endCap.setOrigin(capRadius, capRadius);
        seg.endCap.setFillColor(color);
    }
}

void BoxTraceProgress::layoutSegment(int index, float p)
{
    Segment& seg = segments[index];
    float halfThickness = thickness / 2.f; 

    sf::Vector2f current(
        seg.start.x + (seg.end.x - seg.start.x) * p,
        seg.start.y + (seg.end.y - seg.start.y) * p
    );

    if (seg.horizontal)
    {
        float minX = std::min(seg.start.x, current.x);
        float maxX = std::max(seg.start.x, current.x);
        seg.bar.setPosition(minX, seg.start.y - halfThickness);
        seg.bar.setSize(sf::Vector2f(maxX - minX, thickness));
    }
    else
    {
        float minY = std::min(seg.start.y, current.y);
        float maxY = std::max(seg.start.y, current.y);
        seg.bar.setPosition(seg.start.x - halfThickness, minY);
        seg.bar.setSize(sf::Vector2f(thickness, maxY - minY));
    }

    
    seg.startCap.setPosition(seg.start);
    seg.endCap.setPosition(current);
}

void BoxTraceProgress::setProgress(int edgeIndex, float edgeProgress01)
{
    if (edgeProgress01 < 0.f) edgeProgress01 = 0.f;
    if (edgeProgress01 > 1.f) edgeProgress01 = 1.f;

    for (int i = 0; i < 4; ++i)
    {
        float p = (i < edgeIndex) ? 1.f : (i == edgeIndex ? edgeProgress01 : 0.f);
        edgeVisible[i] = p > 0.f;
        layoutSegment(i, p);
    }
}

void BoxTraceProgress::draw(sf::RenderWindow& window)
{
    for (int i = 0; i < 4; ++i)
    {
        if (edgeVisible[i])
        {
            window.draw(segments[i].bar);
            window.draw(segments[i].startCap);
            window.draw(segments[i].endCap);
        }
    }
}