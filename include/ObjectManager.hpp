/** @file ObjectManager.hpp
 *  @brief Class to manage creation of objects
 *
 *
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef OBJECTMANAGER_HPP
#define OBJECTMANAGER_HPP

#include "Object.hpp"
#include "Camera.hpp"

// Purpose:
// This class sets up a full graphics program using SDL
//
//
//
class ObjectManager
{
public:
    // Singleton pattern for having one single ObjectManager
    // class at any given time.
    static ObjectManager &Instance();

    // Destructor
    ~ObjectManager();
    // Add a new object
    void AddObject(Object *&o);
    // Retrieve a reference to an object
    Object &GetObject(unsigned int index);
    // Deletes all of the objects
    void RemoveAll();
    // Update all objects
    void UpdateAll(unsigned int screenWidth, unsigned int screenHeight, Camera *camera, bool flipNormals);
    // Render All Objects
    void RenderAll();
    // Render Specific Objects
    void RenderAt(bool m_showSurface[9], bool showNormals);
    // Get the number of Objects
    unsigned int GetNumObjects();
    // Cubemap Texture Index
    unsigned int m_cubemapTexture;

private:
    // Constructor is private because we should
    // not be able to construct any other managers,
    // this how we ensure only one is ever created
    ObjectManager();
    // Objects in our scene
    std::vector<Object *> m_objects;
};

#endif
