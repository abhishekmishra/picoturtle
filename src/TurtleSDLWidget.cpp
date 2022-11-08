#include "TurtleSDLWidget.hpp"

TurtleSDLWidget::TurtleSDLWidget(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw "SDL_Error: " + std::string(SDL_GetError());
    }

    printf("Creating SDL window\n");

    window = SDL_CreateWindowFrom(reinterpret_cast<void*>(winId()));
    if(window == NULL)
        throw "Can't create window: " + std::string(SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
        throw "Can't create renderer: " + std::string(SDL_GetError());
}

void TurtleSDLWidget::resizeEvent(QResizeEvent*) {
    int x = static_cast<int>(QWidget::width());
    int y = static_cast<int>(QWidget::height());
    OnResize(x, y);
}

void TurtleSDLWidget::showEvent(QShowEvent*) {
    Init();
    /* frameTimer will send signal timeout() every 60th of a second, connect to "repaint" */
    connect(&frameTimer, &QTimer::timeout, this, &TurtleSDLWidget::SDLRepaint);
    frameTimer.setInterval(MS_PER_FRAME);
    frameTimer.start();
}

void TurtleSDLWidget::SDLRepaint() {
    Update();
    SDL_RenderPresent(renderer);
}

TurtleSDLWidget::~TurtleSDLWidget() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

/* Override default system paint engine to remove flickering */
QPaintEngine* TurtleSDLWidget::paintEngine() const {
    return reinterpret_cast<QPaintEngine*>(0);
}

void TurtleSDLWidget::Init() {
    printf("example init called\n");
}

void TurtleSDLWidget::Update() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, new SDL_Rect{ 0,0,500,200 });
}

void TurtleSDLWidget::OnResize(int, int) {
    printf("example resize called\n");
}
