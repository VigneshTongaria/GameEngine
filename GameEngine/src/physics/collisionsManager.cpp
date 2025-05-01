#include"collisionsManager.hpp"

void CollisionsManager::AddColliderToPhysics(Collider* c)
{
    sceneColliders.push_back(c);
}

void CollisionsManager::resolveGridColliders()
{
    for(auto& col : sceneColliders)
    {
        glm::vec2 coll_xy = col->GetColliderPosition();
        int grid_X = static_cast<int>(coll_xy.x/static_cast<float>(GRID_WIDTH));
        int grid_Y = static_cast<int>(coll_xy.x/static_cast<float>(GRID_HEIGHT));
        gridColliders[grid_X][grid_Y].push_back(col);
    }

    for(int i = -GRID_WIDTH; i<GRID_WIDTH; i++)
    {
        for(int j = - GRID_HEIGHT; j<GRID_HEIGHT; j++)
        {
            for(auto& col_a : gridColliders[i][j])
            {
                for(auto& col_b : gridColliders[i][j])
                {
                    if(col_a != col_b) 
                       TryResolveCollision(col_a,col_b);
                }
            }
        }
    }
}

void CollisionsManager::TryResolveCollision(Collider* a, Collider* b) {
    UnorderedMapKey<Collider*> pair = { a, b };
    
    auto it = activeCollisions.find(pair);
    if (it != activeCollisions.end()) {

        it->second -= collisionCooldown;
        if (it->second <= 0.0f)
            it = activeCollisions.erase(it);
        return; // Already resolved recently
    }

    // Not in set → resolve it
    physicsCollision::ResolveCollisions(a,b);
    activeCollisions[pair] = collisionCooldown;
}

void CollisionsManager::Update() {

    resolveGridColliders();

    for (auto it = activeCollisions.begin(); it != activeCollisions.end(); ) {
        it->second -= collisionCooldown;
        if (it->second <= 0.0f)
            it = activeCollisions.erase(it);
        else
            ++it;
    }
}