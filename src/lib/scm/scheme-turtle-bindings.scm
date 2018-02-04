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

(forward 10)
(right -90)
(forward 10)
(right -90)
(penup)
(forward 10)
(pendown)
(forward 10)

(define (circle)
    (define (c n)
         (if (> n 0)
            (begin
               (forward 1)
              (right 1)
              (c (- n 1)))
            #f))
    (c 360))

(circle)
