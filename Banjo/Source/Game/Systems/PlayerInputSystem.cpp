#pragma once
#include "PlayerInputSystem.h"

#include <iostream>
#include "../../Components/RigidBody.hpp"
#include "../../Components/Transform.hpp"
#include "../../Components/BasicShape.hpp"
#include "../../Components/Player.hpp"
#include "../../Components/Gravity.hpp"
#include "../../Components/Projectile.hpp"
#include "../../Components/SFX.hpp"
#include "../../Core/Systems/AudioSystem.h"
#include "../../Utility/Util.hpp"

extern Coordinator m_Coordinator;

void PlayerInputSystem::Init(std::vector<Mix_Chunk*> laserExplosions)
{
    m_LaserExplosions = laserExplosions;
}

void PlayerInputSystem::Update()
{
    for(auto& entity :m_Entities)
    {
        auto& transformComp = m_Coordinator.GetComponent<Transform>(entity);
        auto& rigidBodyComp = m_Coordinator.GetComponent<RigidBody>(entity);
        auto& playerComp = m_Coordinator.GetComponent<Player>(entity);
        auto& basicShapeComp = m_Coordinator.GetComponent<BasicShape>(entity);
        playerComp.ProjectileSpawnLocation = basicShapeComp.m_Vertices[2];
        transformComp.position += rigidBodyComp.velocity;

        if(rigidBodyComp.rotationSpeed.x > 0)
        {
            HandleRotation(entity, m_Orientation);
        }
        if(m_bIsAccelerating )//&& rigidBodyComp.velocity != (rigidBodyComp.velocity * PLAYER_SPEED) )
        {
            rigidBodyComp.velocity = GetPlayerPointDirection(entity) * PLAYER_SPEED;
        }
    }
}


Vector2 PlayerInputSystem::GetPlayerPointDirection(const Entity& entity)
{
    auto& basicShapeComp = m_Coordinator.GetComponent<BasicShape>(entity);
    SDL_FPoint TipPoint = basicShapeComp.m_Vertices[2];
    // halfway between the other two vertices;
    SDL_FPoint ReferencePoint;
    ReferencePoint.x = (basicShapeComp.m_Vertices[1].x + basicShapeComp.m_Vertices[0].x) / 2;
    ReferencePoint.y = (basicShapeComp.m_Vertices[1].y + basicShapeComp.m_Vertices[0].y) / 2;

    float x = TipPoint.x - ReferencePoint.x;
    float y = TipPoint.y - ReferencePoint.y;
    Vector2 velocity {x, y};
    velocity.normalize();
    return velocity;
}

void PlayerInputSystem::CreatePlayerProjectile()
{
    for(auto& entity : m_Entities)
    {
        auto& playerComponent = m_Coordinator.GetComponent<Player>(entity);
        auto& transformComp = m_Coordinator.GetComponent<Transform>(entity);
        auto& playerRigidBodyComp = m_Coordinator.GetComponent<RigidBody>(entity);
        // straight line based on the 3rd vertex
        std::vector<SDL_FPoint> projectile_verticies;
        Vector2 velocityDirection = GetPlayerPointDirection(entity);
        
        SDL_FPoint startPoint = playerComponent.ProjectileSpawnLocation;
        SDL_FPoint endPoint;
        endPoint.x = startPoint.x  + velocityDirection.x;
        endPoint.y = startPoint.y  + velocityDirection.y;

        projectile_verticies.push_back(startPoint);
        projectile_verticies.push_back(endPoint);
        
        int r=255,g=255,b=255;
        Vector3 ColorWhite(r,g,b);

        float projectileSpawnX = transformComp.position.x;
        float projectileSpawnY = transformComp.position.y;

        // Cycle through our sounds
        if(m_laserIndex == static_cast<int>(m_LaserExplosions.size()))
        {
            m_laserIndex = 0;
        }
        Mix_Chunk* laser = m_LaserExplosions[m_laserIndex];
        ++m_laserIndex;
        
        const Entity projectileEntity = m_Coordinator.CreateEntity();
        m_Coordinator.AddComponent<RigidBody>(projectileEntity, {(velocityDirection * PLAYER_PROJECTILE_SPEED) + playerRigidBodyComp.velocity ,  Vector2(0, 0)});
        m_Coordinator.AddComponent<Transform>(projectileEntity, {Vector2(projectileSpawnX, projectileSpawnY),  Vector2(1, 1), Vector2(0,0)});
        m_Coordinator.AddComponent<BasicShape>(projectileEntity, {projectile_verticies,  ColorWhite});
        m_Coordinator.AddComponent<Projectile>(projectileEntity,{});
        m_Coordinator.AddComponent<SFX>(projectileEntity,{nullptr, laser});
        AudioSystem::PlaySound(laser, -1);
    }
}

void PlayerInputSystem::HandleRotation(Entity entity, ORIENTATION orientation)
{
    auto& basicShapeComp = m_Coordinator.GetComponent<BasicShape>(entity);
    //positive is clockwise
    const double theta = orientation == Clockwise ? -50: 50;
    RotateShape(basicShapeComp.m_Vertices, theta);
}

void PlayerInputSystem::RotateShape(std::vector<SDL_FPoint>& vertices, float theta) {
    const int polygonSides = static_cast<int>(vertices.size());
    const std::vector<SDL_FPoint> previousVertices = vertices;
    
    // We need our center point to rotate around
    SDL_FPoint centerPoint;
    centerPoint.x = 0;
    centerPoint.y = 0;
    
    for(const auto& vertex : vertices)
    {
        centerPoint.x += vertex.x;
        centerPoint.y += vertex.y;
    }
    
    centerPoint.x  = centerPoint.x / polygonSides;
    centerPoint.y  = centerPoint.y / polygonSides;

    
    // Translate polygon so that the center point coincides with the origin
    for (int i = 0; i < polygonSides; i++)
    {
        vertices[i].x -= centerPoint.x;
        vertices[i].y -= centerPoint.y;
    }

    // Rotate polygon around the origin
    for (int i = 0; i < polygonSides; i++)
    {
        float x = (vertices[i].x * cos(theta)) - (vertices[i].y * sin(theta));
        float y = (vertices[i].x * sin(theta)) + (vertices[i].y * cos(theta));
        
        vertices[i].x = x;
        vertices[i].y = y;
    }

    // Translate polygon back to its original position
    for (int i = 0; i < polygonSides; i++)
    {
        vertices[i].x += centerPoint.x;
        vertices[i].y += centerPoint.y;
    }

    // Ensure our polygon area is maintained
    ScaleRotatedPoints(previousVertices, vertices, centerPoint);
}

void PlayerInputSystem::ScaleRotatedPoints(const std::vector<SDL_FPoint>& prevVertices,
     std::vector<SDL_FPoint>& currentVertices, const SDL_FPoint& centerPoint)
{
    std::vector<double> previousDistances;
    std::vector<double> currentDistances;

    // vector to hold our scale values for each vertex
    std::vector<double> vertexScale;

    for(const auto& point : prevVertices)
    {
        double previousDistanceToCenter = Geometry::DistanceSquared(point, centerPoint);
        previousDistances.push_back(previousDistanceToCenter);
    }

    for(const auto& point : currentVertices)
    {
        double currentDistanceToCenter = Geometry::DistanceSquared(point, centerPoint);
        currentDistances.push_back(currentDistanceToCenter);
    }

    for(int i =0; i < previousDistances.size(); ++i)
    {
        double scale = previousDistances[i] / currentDistances[i];
        vertexScale.push_back(scale);
    }
    
    int scaleIndex = 0;
    for(auto& point : currentVertices)
    {
        float xoffSet = (point.x - centerPoint.x) * vertexScale[scaleIndex];
        float yoffSet = ((point.y - centerPoint.y) * vertexScale[scaleIndex]);
        point.x = centerPoint.x + xoffSet;
        point.y = centerPoint.y + yoffSet;
        ++scaleIndex;
    }
}

void PlayerInputSystem::MouseButtonEvent(const SDL_MouseButtonEvent& event)
{
    if(event.button == SDL_BUTTON_LEFT)
    {
        SDL_FPoint pos;
        pos.x = event.x;
        pos.y = event.y;
        std::vector<SDL_FPoint> point;
        point.push_back(pos);

        Vector3 ColorWhite;
        ColorWhite.x = 255;
        ColorWhite.y = 255;
        ColorWhite.z = 255;

        Entity dotEntity = m_Coordinator.CreateEntity();
        m_Coordinator.AddComponent<Transform>(dotEntity, {Vector2(event.x, event.y),  Vector2(1, 1), Vector2(0,0)});
        m_Coordinator.AddComponent<BasicShape>(dotEntity, {point,  ColorWhite});
        m_Coordinator.AddDrawnPoint(pos);
    }
}

void PlayerInputSystem::OnEntityDelete(Entity entity)
{
    System::OnEntityDelete(entity);
}


void PlayerInputSystem::HandleInput(const SDL_Event& event)
{
    // Player Input
    for(auto& entity :m_Entities)
    {
        auto& rigidBodyComp = m_Coordinator.GetComponent<RigidBody>(entity);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_a:
                        m_Orientation = CounterClockwise;
                        rigidBodyComp.rotationSpeed = {0.5f,0.5f};
                        break;
                    case SDLK_d:
                        m_Orientation = Clockwise;
                         rigidBodyComp.rotationSpeed = {0.5f,0.5f};
                         break;
                    case SDLK_w:
                        m_bIsAccelerating = true;
                        break;
                    case SDLK_s:
                        break;
                    case SDLK_p:
                        break;
                        // only allow fire once
                    case SDLK_q:
                        //m_Coordinator.GenerateConvexHull();
                        break;
                }
                break;
            // Only touch when we are not moving in a direction already upon release
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_a:
                         rigidBodyComp.rotationSpeed = {0,0}; 
                        break;
                    case SDLK_d:
                         rigidBodyComp.rotationSpeed = {0,0};
                        break;
                    case SDLK_w:
                        m_bIsAccelerating = false;
                         rigidBodyComp.velocity= {0,0};
                         break;
                    case SDLK_s:
                        m_bIsAccelerating = false;
                        rigidBodyComp.velocity= {0,0};
                        break;
                    case SDLK_p:
                        m_Coordinator.TogglePause();
                        break;
                    case SDLK_SPACE:
                        CreatePlayerProjectile();
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
            case SDL_MOUSEBUTTONUP:
                break;
            default:
                break;
        }
    }

    // Gamewide input
    switch(event.type)
    {
    case SDL_KEYUP:
        switch(event.key.keysym.sym)
        {
            case SDLK_r:
                m_Coordinator.ToggleRestartFlag();
                break;
            default:
                break;
        }
        break;
    default:
        break;
    }
}
