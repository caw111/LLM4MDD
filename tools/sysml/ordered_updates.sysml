package Sequential {
    private import ScalarValues::*;
    part def Controller {
        attribute a : Integer := 0;
        attribute b : Integer := 0;
        perform action tick {
            first start;
            then assign a := a + 1;
            then assign b := a + 2;
        }
        out attribute value : Integer = b;
    }
    part system : Controller;
}
