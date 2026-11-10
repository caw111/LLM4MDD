package ConditionalOrder {
    private import ScalarValues::*;
    part def Controller {
        in attribute reset : Boolean;
        attribute count : Integer := 0;
        attribute doubled : Integer := 0;
        perform action tick {
            if reset {
                first start;
                then assign count := 0;
                then assign doubled := 0;
            } else {
                first start;
                then assign count := count + 1;
                then assign doubled := count * 2;
            }
        }
        out attribute value : Integer = doubled;
    }
    part system : Controller;
}
