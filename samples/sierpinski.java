import in.abhishekmishra.picoturtle.Turtle;
import in.abhishekmishra.picoturtle.TurtleState;

public class sierpinski
{
    public static void draw_sierpinski(Turtle t, 
            double length, int depth) {
        if(depth == 0) {
            t.pendown();
            for(int i = 0; i < 3; i++) {
                t.forward(length);
                t.left(120);
            }
            t.penup();
        }
        else {
            double half = length/2.0;
            draw_sierpinski(t, half, depth - 1);
            t.forward(half);
            draw_sierpinski(t, half, depth - 1);
            t.forward(-half);
            t.left(60);
            t.forward(half);
            t.right(60);
            draw_sierpinski(t, half, depth - 1);
            t.left(60);
            t.forward(-half);
            t.right(60);
        }
    }

    public static void main(String[] args)
    {
        // Create the turtle before using
        Turtle t = Turtle.CreateTurtle(args);
        
        if (t != null) {
            // Your code goes here

            t.penup();
            t.left(90);
            t.forward(200);
            t.right(90);
            t.forward(-200);
            t.pencolour(255, 69, 0);
            t.penwidth(2);
            t.pendown();
            t.right(90);
            draw_sierpinski(t, 400, 5);
            
            // Your code ends here

            // Always stop the turtle
            t.stop();
        }
        else {
            System.out.println("Error: Unable to create a turtle.");
            System.exit(-1);
        }
    }
}
