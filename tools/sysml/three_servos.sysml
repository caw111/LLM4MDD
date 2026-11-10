package ThreeServos {
    private import ScalarValues::*;
    part def Controller {
        in attribute pot1 : Integer;
        in attribute pot2 : Integer;
        out attribute angle1 : Real = pot1 * 180.0 / 1023.0;
        out attribute angle2 : Real = pot2 * 180.0 / 1023.0;
        out attribute angle3 : Real = pot2 * 180.0 / 1023.0;
    }
    part system : Controller;
}
