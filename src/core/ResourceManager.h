#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>


template <typename ResourceT>
class ResourceManager
{
public:
    
    static ResourceManager& instance()
    {
        static ResourceManager manager;
        return manager;
    }

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

   
    std::shared_ptr<ResourceT> acquire(const std::string& filePath)
    {
        auto cached = cache.find(filePath);
        if (cached != cache.end())
        {
            if (std::shared_ptr<ResourceT> stillAlive = cached->second.resource.lock())
            {
                return stillAlive;
            }
        }

     
        auto resource = std::make_shared<ResourceT>();
        if (!resource->loadFromFile(filePath))
        {
            std::cerr << "[ResourceManager] Failed to load resource: " << filePath << std::endl;
        }

        cache[filePath] = CacheEntry{ resource, sf::Time::Zero };
        return resource;
    }

    void update(sf::Time deltaTime)
    {
        std::unordered_set<std::string> toEvict;

        for (auto& [filePath, entry] : cache)
        {
            std::shared_ptr<ResourceT> locked = entry.resource.lock();

            if (!locked)
            {
                
                toEvict.insert(filePath);
                continue;
            }

            if (locked.use_count() == 1)
            {
               
                entry.unusedFor += deltaTime;
                if (entry.unusedFor > unusedResourceLifetime())
                {
                    toEvict.insert(filePath);
                }
            }
            else
            {
               
                entry.unusedFor = sf::Time::Zero;
            }
        }

        for (const auto& filePath : toEvict)
        {
            cache.erase(filePath);
        }
    }

private:
    ResourceManager() = default;

    struct CacheEntry
    {
        std::weak_ptr<ResourceT> resource;
        sf::Time unusedFor;
    };

    static sf::Time unusedResourceLifetime() { return sf::seconds(5.f); }

    std::unordered_map<std::string, CacheEntry> cache;
};


using TextureManager = ResourceManager<sf::Texture>;
using FontManager = ResourceManager<sf::Font>;
using SoundManager = ResourceManager<sf::SoundBuffer>;
