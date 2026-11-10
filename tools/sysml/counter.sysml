package Stateful {
    private import ScalarValues::*;
    part def Counter {
        in attribute reset : Boolean;
        attribute count : Integer := 0;
        perform action tick {
            if reset { assign count := 0; }
            else { assign count := count + 1; }
        }
        out attribute value : Integer = count;
    }
    part system : Counter;
}
