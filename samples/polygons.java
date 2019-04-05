import in.abhishekmishra.picoturtle.Turtle;
import in.abhishekmishra.picoturtle.TurtleState;

public class polygons {
    /* a simple turtle program to print a polygon */
    public static void poly(Turtle t, int num, int side, double angle) {
        for (int i = 0; i < num; i++) {
            t.forward(side);
            t.right(angle);
        }
    }

    public static void main(String[] args) {
        // Create the turtle before using
        Turtle t = Turtle.CreateTurtle(args);

        if (t != null) {
            // Your code goes here

            // draw a hexagon in red
            t.penwidth(2);
            t.pendown();
            t.pencolour(255, 0, 0);
            poly(t, 6, 50, 60);

            // move up 100
            t.penup();
            t.forward(-100);
            t.right(90);

            // draw a pentagon in blue
            t.pendown();
            t.pencolour(0, 0, 255);
            poly(t, 5, 50, 72);

            // Your code ends here

            // Always stop the turtle
            t.stop();
        } else {
            System.out.println("Error: Unable to create a turtle.");
            System.exit(-1);
        }
    }
}
