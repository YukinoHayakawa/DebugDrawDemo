﻿#pragma once

#include <Usagi/Camera/Controller/CameraMan.hpp>
#include <Usagi/Graphics/Game/GraphicalGameState.hpp>
#include <Usagi/Interactive/InputMapping.hpp>

namespace usagi
{
class DebugDrawSystem;
class PerspectiveCamera;
class ModelViewCameraController;

class DebugDrawGameState
    : public GraphicalGameState
    , public MouseEventListener
{
    Element *mDebugDrawRoot = nullptr;
    using ModelCameraMan =
        CameraMan<PerspectiveCamera, ModelViewCameraController>;
    ModelCameraMan *mCameraElement = nullptr;
    InputMapping *mInputMap = nullptr;

    void setupInput();
    void setupCamera();

public:
    DebugDrawGameState(Element *parent, std::string name, GraphicalGame *game);

    void update(const Clock &clock) override;
    bool onMouseButtonStateChange(const MouseButtonEvent &e) override;
};
}
