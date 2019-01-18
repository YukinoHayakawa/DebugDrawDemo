﻿#include "DebugDrawGameState.hpp"

#include <Usagi/Camera/Controller/ModelViewCameraController.hpp>
#include <Usagi/Camera/PerspectiveCamera.hpp>
#include <Usagi/Graphics/Game/GraphicalGame.hpp>
#include <Usagi/Graphics/RenderTarget/RenderTargetDescriptor.hpp>
#include <Usagi/Runtime/Graphics/Swapchain.hpp>
#include <Usagi/Runtime/Input/InputManager.hpp>
#include <Usagi/Runtime/Input/Mouse/Mouse.hpp>
#include <Usagi/Runtime/Runtime.hpp>
#include <Usagi/Transform/TransformComponent.hpp>
#include <Usagi/Utility/Functional.hpp>

#include <Usagi/Extension/DebugDraw/DebugDrawSubsystem.hpp>
#include "DebugDrawDemoComponent.hpp"

void usagi::DebugDrawGameState::setupInput()
{
    const auto mouse = mGame->runtime()->inputManager()->virtualMouse();
    mouse->addEventListener(mInputMap);
    mouse->addEventListener(this);
}

void usagi::DebugDrawGameState::setupCamera()
{
    mCameraElement = addChild<ModelCameraMan>(
        "Camera",
        std::make_shared<PerspectiveCamera>(),
        std::make_shared<ModelViewCameraController>(
            Vector3f::Zero(), 10.f
        ));

    auto camact = mInputMap->addActionGroup("Camera");
    camact->setAnalogAction2DHandler("Move", partial_apply(
        &ModelViewCameraController::rotate,
        mCameraElement->cameraController()));
    camact->bindMouseRelativeMovement("Move");
    camact->activate();
}

usagi::DebugDrawGameState::DebugDrawGameState(
    Element *parent,
    std::string name,
    GraphicalGame *game)
    : GraphicalGameState(parent, std::move(name), game)
{
    // this RenderableSubsystem will be registered by subsystemFilter()
    // of GraphicalGameState
    const auto dd = addSubsystem(
        "dd",
        std::make_unique<DebugDrawSubsystem>(game));
    dd->setSizeFunctionsFromRenderWindow(mGame->mainWindow());
    dd->setWorldToNdcFunc([=]() {
        // world -> camera local -> NDC
        return mCameraElement->camera()->localToNDC() *
            mCameraElement->comp<TransformComponent>()->worldToLocal();
    });
    mDebugDrawRoot = addChild("DebugDrawRoot");
    mDebugDrawRoot->addComponent<DebugDrawDemoComponent>();
    mInputMap = addChild<InputMapping>("InputMapping");

    setupInput();
    setupCamera();
}

usagi::DebugDrawGameState::~DebugDrawGameState()
{
    const auto mouse = mGame->runtime()->inputManager()->virtualMouse();
    mouse->removeEventListener(mInputMap);
    mouse->removeEventListener(this);
}

void usagi::DebugDrawGameState::update(const Clock &clock)
{
    // todo better way?
    mCameraElement->camera()->setPerspective(
        degreesToRadians(90.f),
        mGame->mainWindow()->swapchain->aspectRatio(),
        0.5f, 100.f);

    GraphicalGameState::update(clock);
}

bool usagi::DebugDrawGameState::onMouseButtonStateChange(
    const MouseButtonEvent &e)
{
    if(e.button == MouseButtonCode::RIGHT)
    {
        e.mouse->setImmersiveMode(e.pressed);
        return true;
    }
    return false;
}
