#include "ObjectManager.hpp"
#include "Camera.hpp"

// Constructor is empty
ObjectManager::ObjectManager()
{
}

ObjectManager &ObjectManager::Instance()
{
    static ObjectManager *instance = new ObjectManager();
    return *instance;
}

// Retrieve a reference to an object
Object &ObjectManager::GetObject(unsigned int index)
{
    return *m_objects[index];
}

void ObjectManager::AddObject(Object *&o)
{
    m_objects.push_back(o);
}

void ObjectManager::RemoveAll()
{
    for (int i = 0; i < m_objects.size(); i++)
    {
        delete m_objects[i];
    }
}

void ObjectManager::UpdateAll(unsigned int screenWidth, unsigned int screenHeight, Camera *camera, bool flipNormals)
{
    for (int i = 0; i < m_objects.size(); i++)
    {
        m_objects[i]->Update(screenWidth, screenHeight, camera, m_cubemapTexture, flipNormals);
        m_objects[i]->Update(screenWidth, screenHeight, camera, m_cubemapTexture, flipNormals, false);
    }
}

void ObjectManager::RenderAll()
{
    for (int i = 0; i < m_objects.size(); i++)
    {
        m_objects[i]->Render(m_cubemapTexture);
    }
}

void ObjectManager::RenderAt(bool showSurface[], bool showNormals)
{
    for (int i = 0; i < 9; i++)
    {
        if (showSurface[i] && i < m_objects.size())
        {
            m_objects[i]->Render(m_cubemapTexture);
            if (showNormals)
                m_objects[i]->Render(m_cubemapTexture, false);
        }
    }
}

unsigned int ObjectManager::GetNumObjects()
{
    return m_objects.size();
}