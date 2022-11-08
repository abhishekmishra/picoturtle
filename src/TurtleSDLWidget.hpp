#pragma once
// this file is created based on a tutorial at
// https://toffee.neocities.org/posts/sdlgui-qt/

#include <QWidget>
#include <QTimer>
#include <SDL2/SDL.h>

#define FRAME_RATE 60
#define MS_PER_FRAME 1000/FRAME_RATE

class TurtleSDLWidget : public QWidget
{
public :
    TurtleSDLWidget(QWidget* parent);
    virtual ~TurtleSDLWidget() override;

public slots:
    void SDLRepaint();

protected:
    SDL_Window* window;
    SDL_Renderer* renderer;
private:
    /* To be overridden by child classes */
    virtual void Init();
    virtual void Update();
    virtual void OnResize(int w, int h);

    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent*) override;
    QPaintEngine* paintEngine() const override;

    QTimer frameTimer;
};
