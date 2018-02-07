(define Turtle (js-eval "require('../../../src/turtle')"))
(define t (js-invoke Turtle 'createTurtle "turtleCanvas"))

(define (forward d)
    (js-invoke t 'exec "forward" d))

(define (right angle)
    (js-invoke t 'exec "right" angle))

(define (penup)
    (js-invoke t 'exec "penUp"))

(define (pendown)
    (js-invoke t 'exec "penDown"))

(define (pencolor pc)
    (js-invoke t 'exec "color" pc))

(define (backward d)
    (forward (- d)))

(define (left angle)
    (right (- angle)))

(define (home)
    (js-invoke t 'exec "home"))