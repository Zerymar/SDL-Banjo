﻿#pragma once
#include "PlayerInputSystem.h"

#include <iostream>

#include "../../Components/RigidBody.hpp"
#include "../../Components/Transform.hpp"
#include "../../Components/BasicShape.hpp"
#include "../../Components/Player.hpp"
#include "../../Components/Gravity.hpp"
#include "../Coordinator.hpp"
#include "../../Components/Projectile.hpp"

extern Coordinator m_Coordinator;

void PlayerInputSystem::Init()
{
}

void PlayerInputSystem::Update()
{
    for(auto& entity :m_Entities)
    {
        auto& transformComp = m_Coordinator.GetComponent<Transform>(entity);
        auto& basicShapeComp = m_Coordinator.GetComponent<BasicShape>(entity);
        auto& rigidBodyComp = m_Coordinator.GetComponent<RigidBody>(entity);
        transformComp.position += rigidBodyComp.velocity;

        if(rigidBodyComp.rotationSpeed.x > 0)
        {
            HandleRotation(entity, m_Orientation);
        }
        
        /*
        for(int i = 0; i < m_targetVertices.size(); ++i)
        {
            SDL_Point& currentVertex = basicShapeComp.m_Vertices[i];
            SDL_Point targetVertex = m_targetVertices[i];

            // spin positive
            if(currentVertex.x < targetVertex.x)
            {
                currentVertex.x += rigidBodyComp.rotationSpeed.x;
            }
            else
            {
                currentVertex.x += rigidBodyComp.rotationSpeed.x * -1;
            }

            if(currentVertex.y < targetVertex.y)
            {
                currentVertex.y += rigidBodyComp.rotationSpeed.y;
            }
            else
            {
                currentVertex.y += rigidBodyComp.rotationSpeed.y * -1;
            }
        }
        */
    }
}


void PlayerInputSystem::CreatePlayerProjectile()
{
    for(auto& entity : m_Entities)
    {
        auto& playerComponent = m_Coordinator.GetComponent<Player>(entity);
        auto& transformComp = m_Coordinator.GetComponent<Transform>(entity);
        // straight line based on the 3rd vertix
        std::vector<SDL_Point> projectile_verticies;
        SDL_Point startPoint = playerComponent.ProjectileSpawnLocation;
        SDL_Point endPoint;
        endPoint.x = startPoint.x + 3;
        endPoint.y = startPoint.y ;

        projectile_verticies.push_back(startPoint);
        projectile_verticies.push_back(endPoint);
        
        int r=255,g=255,b=255;
        Vector3 ColorWhite(r,g,b);

        int projectileSpawnX = transformComp.position.x;
        int projectileSpawnY = transformComp.position.y;
        
        Entity projectileEntity = m_Coordinator.CreateEntity();
        m_Coordinator.AddComponent<RigidBody>(projectileEntity, {Vector2(5, 0),  Vector2(0, 0)});
        m_Coordinator.AddComponent<Transform>(projectileEntity, {Vector2(projectileSpawnX, projectileSpawnY),  Vector2(1, 1), Vector2(0,0)});
        m_Coordinator.AddComponent<BasicShape>(projectileEntity, {projectile_verticies,  ColorWhite});
        m_Coordinator.AddComponent<Gravity>(projectileEntity,{Vector2(0, 0)});
        m_Coordinator.AddComponent<Projectile>(projectileEntity,{});
    }
    
}

void PlayerInputSystem::HandleRotation(Entity entity, ORIENTATION orientation)
{
    auto& basicShapeComp = m_Coordinator.GetComponent<BasicShape>(entity);
    //positive is clockwise
    const double theta = orientation == Clockwise ? -50: 50;
    RotateShape(basicShapeComp.m_Vertices, theta);
}



void PlayerInputSystem::RotateShape(std::vector<SDL_Point>& vertices, double theta) {
    const int polygonSides = vertices.size();
    const std::vector<SDL_Point> previousVertices = vertices;
    
    // We need our center point to rotate around
    SDL_Point centerPoint;
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
        int x = (vertices[i].x * cos(theta)) - (vertices[i].y * sin(theta));
        int y = (vertices[i].x * sin(theta)) + (vertices[i].y * cos(theta));
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

void PlayerInputSystem::ScaleRotatedPoints(const std::vector<SDL_Point>& prevVertices,
     std::vector<SDL_Point>& currentVertices, const SDL_Point& centerPoint)
{
    std::vector<double> previousDistances;
    std::vector<double> currentDistances;

    // vector to hold our scale values for each vertex
    std::vector<double> vertexScale;

    for(const auto& point : prevVertices)
    {
        int previousDistanceToCenter = Geometry::DistanceSquared(point, centerPoint);
        previousDistances.push_back(previousDistanceToCenter);
    }

    for(const auto& point : currentVertices)
    {
        int currentDistanceToCenter = Geometry::DistanceSquared(point, centerPoint);
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
        point.x = centerPoint.x + (point.x - centerPoint.x) * vertexScale[scaleIndex];
        point.y = centerPoint.y + (point.y - centerPoint.y) * vertexScale[scaleIndex];
        ++scaleIndex;
    }
}

void PlayerInputSystem::MouseButtonEvent(const SDL_MouseButtonEvent& event)
{
    if(event.button == SDL_BUTTON_LEFT)
    {
        SDL_Point pos;
        pos.x =event.x;
        pos.y = event.y;
        std::vector<SDL_Point> point;
        point.push_back(pos);

        Vector3 ColorWhite;
        ColorWhite.x = 255;
        ColorWhite.y = 255;
        ColorWhite.z = 255;

        Entity dotEntity = m_Coordinator.CreateEntity();
        m_Coordinator.AddComponent<Transform>(dotEntity, {Vector2(event.x, event.y),  Vector2(1, 1), Vector2(0,0)});
        m_Coordinator.AddComponent<BasicShape>(dotEntity, {point,  ColorWhite});
        std:: cout << "Created Entity at "<< pos.x << "," << pos.y << std::endl;
        m_Coordinator.AddDrawnPoint(pos);
    }
}

void PlayerInputSystem::OnEntityDelete(Entity entity)
{
    System::OnEntityDelete(entity);
}


void PlayerInputSystem::HandleInput(const SDL_Event& event)
{
    for(auto& entity :m_Entities)
    {
        auto& rigidBodyComp = m_Coordinator.GetComponent<RigidBody>(entity);
        auto& transformComp = m_Coordinator.GetComponent<Transform>(entity);

        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_a:
                        m_Orientation = CounterClockwise;
                        rigidBodyComp.rotationSpeed = {1,1};
                        break;
                    case SDLK_d:
                        m_Orientation = Clockwise;
                         rigidBodyComp.rotationSpeed = {1,1};
                         break;
                    case SDLK_w:
                        rigidBodyComp.velocity.x = m_ScalarVelocity;
                        break;
                    case SDLK_s:
                        rigidBodyComp.velocity.x = -m_ScalarVelocity;
                        break;
                    case SDLK_SPACE:
                        CreatePlayerProjectile();
                            break;
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
                        if(rigidBodyComp.velocity.x > 0){ rigidBodyComp.velocity.x = 0; }
                         break;
                    case SDLK_s:
                        if(rigidBodyComp.velocity.x < 0){ rigidBodyComp.velocity.x = 0; }
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                //MouseButtonEvent(event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                break;
            default:
                break;
        }
    }
}
