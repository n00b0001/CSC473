#include <atlas/utils/Application.hpp>
#include <atlas/gl/ErrorCheck.hpp>
#include "ClothScene.hpp"

int main()
{
    APPLICATION.createWindow(800, 600, "Assignment2 : Spring Cloth + Spline Camera");
    APPLICATION.addScene(new ClothScene);
    APPLICATION.runApplication();

    return 0;
}
