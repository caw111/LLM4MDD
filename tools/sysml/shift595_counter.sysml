package GeneratedModel {
  private import ScalarValues::*;
  part def Controller {
    in attribute pressed : Boolean;
    attribute count : Integer := 0;
    attribute previousPressed : Boolean := false;
    perform action tick {
      first start;
      then if pressed and not previousPressed {
        first start;
        then assign count := count + 37;
        then if count > 99 {
          first start;
          then assign count := 0;
        }
      }
      then assign previousPressed := pressed;
    }
    out attribute displayNumber : Integer = count;
  }
  part system : Controller;
}
