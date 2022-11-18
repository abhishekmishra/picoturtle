#ifndef TURTLE_CANVAS_WIDGET_H
#define TURTLE_CANVAS_WIDGET_H

#include <QWidget>
#include "PicoTurtle.hpp"
#include "TurtleGLWidget.hpp"
// #include "TurtleSDLWidget.hpp"

namespace turtle
{
    class TurtleCanvasWidget : public QWidget
    {
        Q_OBJECT

    public:
        TurtleCanvasWidget(QWidget *parent = nullptr);
        ~TurtleCanvasWidget();
        void set_turtle(turtle::PicoTurtle *t);
        virtual QSize sizeHint() const override;

    public slots:
        void draw_turtle();

    protected:
        virtual void resizeEvent(QResizeEvent *event) override;

    private:
        TurtleGLWidget *gl_widget;
        // TurtleSDLWidget* sdl_widget;
        void fixup();
    };
};
#endif // TURTLE_CANVAS_WIDGET_H
