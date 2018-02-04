(define (circle)
    (define (c n)
         (if (> n 0)
            (begin
               (forward 1)
              (right 1)
              (c (- n 1)))
            #f))
    (c 360))