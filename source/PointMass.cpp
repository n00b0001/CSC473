#include "PointMass.hpp"
#include "ShaderPaths.hpp"
#include <iostream>
#include <atlas/core/Float.hpp>
#include <atlas/core/Log.hpp>

PointMass::PointMass(glm::vec3 pos) :
    currForce(0.0f),
    mPMass(0.2f, 5, 5),
    isAnchor(false),
    mVelocity(0.0f),
    currPos(pos),
    mass(0.01f)
{
    USING_ATLAS_MATH_NS;
    USING_ATLAS_GL_NS;

    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

    std::vector<ShaderInfo> shaders
    {
        { GL_VERTEX_SHADER, shaderDir + "ball.vs.glsl" },
        { GL_FRAGMENT_SHADER, shaderDir + "ball.fs.glsl" }
    };

    mShaders.push_back(ShaderPointer(new Shader));
    mShaders[0]->compileShaders(shaders);
    mShaders[0]->linkShaders();

    mUniforms.insert(UniformKey("mvpMat",
        mShaders[0]->getUniformVariable("mvpMat")));

    mPMass.createBuffers();

    mModel = glm::translate(Matrix4(1.0f), currPos);

    mShaders[0]->disableShaders();
}

PointMass::~PointMass() { }

void PointMass::updateGeometry(const atlas::utils::Time &t)
{
    USING_ATLAS_MATH_NS;

    // Move a pointmass based on currForce and t.deltatime
    if (!isAnchor)
    {
        Vector accel = currForce * mass;
        mVelocity = mVelocity + t.deltaTime * accel;
        currPos = currPos + t.deltaTime * mVelocity;
        mModel = glm::translate(Matrix4(1.0f), currPos);
    }
}

void PointMass::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view)
{
    mShaders[0]->enableShaders();

    auto mvpMat = projection * view * mModel;
    glUniformMatrix4fv(mUniforms["mvpMat"], 1, GL_FALSE, &mvpMat[0][0]);

    mPMass.drawPrimitive();

    mShaders[0]->disableShaders();
}

void PointMass::transformGeometry(atlas::math::Matrix4 const& t)
{
    mModel = t;
}

const glm::vec3 PointMass::getPos()
{
    return currPos;
}

void PointMass::setForce(glm::vec3 f)
{
    currForce = f;
}

void PointMass::addForce(glm::vec3 f)
{
    currForce += f;
}

void PointMass::setAnchor(bool a)
{
    isAnchor = a;
}

const bool PointMass::isAnchored()
{
    return isAnchor;
}

const glm::vec3 PointMass::getVelocity()
{
    return mVelocity;
}

void PointMass::setVelocity(atlas::math::Vector v)
{
    mVelocity = v;
}
