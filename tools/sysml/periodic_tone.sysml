package PeriodicTone {
    private import ScalarValues::*;
    part def Controller {
        in attribute deltaMs : Integer;
        attribute elapsed : Integer := 0;
        attribute sounding : Boolean := false;
        perform action tick {
            first start;
            then assign elapsed := elapsed + deltaMs;
            then if elapsed >= 1000 {
                first start;
                then assign elapsed := elapsed - 1000;
                then assign sounding := true;
            }
            then if sounding and elapsed >= 100 {
                assign sounding := false;
            }
        }
        out attribute frequency : Integer = if sounding ? 1000 else 0;
    }
    part system : Controller;
}
