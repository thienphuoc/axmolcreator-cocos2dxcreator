/*
 * Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef TEST_H
#define TEST_H

#include "box2d/box2d.h"
#include "extensions/axmol-ext.h"

#include <cstdlib>

class Test;
struct Settings;

#define RAND_LIMIT 32767
#define DRAW_STRING_NEW_LINE 25

/// Random number in range [-1,1]
inline float RandomFloat()
{
    float r = (float)(std::rand() & (RAND_LIMIT));
    r /= RAND_LIMIT;
    r = 2.0f * r - 1.0f;
    return r;
}

/// Random floating point number in range [lo, hi]
inline float RandomFloat(float lo, float hi)
{
    float r = (float)(std::rand() & (RAND_LIMIT));
    r /= RAND_LIMIT;
    r = (hi - lo) * r + lo;
    return r;
}

// This is called when a joint in the world is implicitly destroyed
// because an attached body is destroyed. This gives us a chance to
// nullify the mouse joint.
class DestructionListener : public b2DestructionListener
{
public:
    void SayGoodbye(b2Fixture* fixture) { B2_NOT_USED(fixture); }
    void SayGoodbye(b2Joint* joint);

    Test* test;
};

const int32 k_maxContactPoints = 2048;

struct ContactPoint
{
    b2Fixture* fixtureA;
    b2Fixture* fixtureB;
    b2Vec2 normal;
    b2Vec2 position;
    b2PointState state;
    float normalImpulse;
    float tangentImpulse;
    float separation;
};
class Test : public b2ContactListener
{
public:
    Test();
    virtual ~Test();

    void DrawTitle(const char* string);
    virtual void Step(Settings& settings);
    virtual void UpdateUI() {}
    virtual void Keyboard(int key) { B2_NOT_USED(key); }
    virtual void KeyboardUp(int key) { B2_NOT_USED(key); }
    void ShiftMouseDown(const b2Vec2& p);
    virtual bool MouseDown(const b2Vec2& p);
    virtual void MouseUp(const b2Vec2& p);
    virtual void MouseMove(const b2Vec2& p);
    void LaunchBomb();
    void LaunchBomb(const b2Vec2& position, const b2Vec2& velocity);

    void SpawnBomb(const b2Vec2& worldPt);
    void CompleteBombSpawn(const b2Vec2& p);

    // Let derived tests know that a joint was destroyed.
    virtual void JointDestroyed(b2Joint* joint) { B2_NOT_USED(joint); }

    // Callbacks for derived classes.
    virtual void BeginContact(b2Contact* contact) override { B2_NOT_USED(contact); }
    virtual void EndContact(b2Contact* contact) override { B2_NOT_USED(contact); }
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
    {
        B2_NOT_USED(contact);
        B2_NOT_USED(impulse);
    }

    void ShiftOrigin(const b2Vec2& newOrigin);

    void initShader(void);
    void DrawString(int x, int y, const char* fmt, ...);
    void DrawString(const b2Vec2& p, const char* fmt, ...);
    void DrawAABB(b2AABB* aabb, const b2Color& color);
    void Flush();

    ax::extension::PhysicsDebugNodeBox2D g_debugDraw;
    ax::DrawNode* debugDrawNode;
    std::string debugString = "";

    b2World* m_world;

protected:
    friend class DestructionListener;
    friend class BoundaryListener;
    friend class ContactListener;

    b2Body* m_groundBody;
    b2AABB m_worldAABB;
    ContactPoint m_points[k_maxContactPoints];
    int32 m_pointCount;
    DestructionListener m_destructionListener;
    int32 m_textLine;

    b2Body* m_bomb;
    b2MouseJoint* m_mouseJoint;
    b2Vec2 m_bombSpawnPoint;
    bool m_bombSpawning;
    b2Vec2 m_mouseWorld;
    int32 m_stepCount;
    int32 m_textIncrement;
    b2Profile m_maxProfile;
    b2Profile m_totalProfile;
};

typedef Test* TestCreateFcn();
int RegisterTest(const char* category, const char* name, TestCreateFcn* fcn);

#endif
