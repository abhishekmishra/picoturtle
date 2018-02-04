(define Turtle (js-eval "require('../../../src/turtle')"))
(define t (js-invoke Turtle 'createTurtle "turtleCanvas"))

(define (forward d)
    (js-invoke t 'forward d))

(define (right angle)
    (js-invoke t 'right angle))

(define (penup)
    (js-invoke t 'penUp))

(define (pendown)
    (js-invoke t 'penDown))

(define (pencolor pc)
    (js-invoke t 'color pc))