import in.abhishekmishra.picoturtle.Turtle;
import in.abhishekmishra.picoturtle.TurtleState;

public class tree
{
    public static void tree(Turtle t, double size) {
        if (size < 4) {
            t.forward(size);
            t.forward(-size);
            return;
        }

        t.forward(size / 4);

        t.left(60);
        tree(t, size / 2);
        t.right(60);

        t.forward(size / 4);

        t.right(35);
        tree(t, size / 2);
        t.left(35);

        t.forward(size / 4);

        t.left(30);
        tree(t, size / 2);
        t.right(30);

        t.forward(size / 4);

        t.right(25);
        tree(t, size / 2);
        t.left(25);

        t.forward(-size);
    }

    public static void main(String[] args)
    {
        // Create the turtle before using
        Turtle t = Turtle.CreateTurtle(args);
        
        if (t != null) {
            // Your code goes here

            t.penup();
            t.back(100);
            t.pendown();
            t.pencolour(165, 42, 42);
            tree(t, 128);
            
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
