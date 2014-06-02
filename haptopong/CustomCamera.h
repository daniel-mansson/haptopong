#pragma once

/**
 * Camera that overrides default rendering behaviour so to not render shadows on transparent objects
 */
class CustomCamera : public chai3d::cCamera
{
public:
    CustomCamera(chai3d::cWorld* parent);
    
    virtual void renderView(const int a_windowWidth,
                            const int a_windowHeight,
                            const int a_displayContext = 0,
                            const chai3d::cEyeMode a_eyeMode = chai3d::C_STEREO_LEFT_EYE);
    
    void addExcludeRecieveShadow(cGenericObject* excludeObject) {
         excludeRecieveShadow.push_back(excludeObject);
    }
    
    void removeExcludeRecieveShadow(cGenericObject* excludeObject) {
        excludeRecieveShadow.erase(std::remove(excludeRecieveShadow.begin(), excludeRecieveShadow.end(), excludeObject), excludeRecieveShadow.end());
    }

private:
    std::vector<cGenericObject*> excludeRecieveShadow;
    
};
