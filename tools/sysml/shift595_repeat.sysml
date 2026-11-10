package GeneratedModel {
  private import ScalarValues::*;
  part def Controller {
    in attribute up : Boolean;
    in attribute down : Boolean;
    in attribute reset : Boolean;
    in attribute speedAdc : Real;
    in attribute dt : Integer;
    attribute count : Integer := 0;
    attribute age : Integer := 0;
    attribute previousDirection : Integer := 0;
    attribute direction : Integer = (if up and not down ? 1 else (if down and not up ? -1 else 0));
    attribute intervalMs : Integer = 100 + RealFunctions::floor(speedAdc * 900.0 / 1023.0);
    perform action tick {
      first start;
      then if reset {
        first start;
        then assign count := 0;
        then assign age := 0;
        then assign previousDirection := 0;
      } else {
        first start;
        then if direction == 0 {
          first start;
          then assign age := 0;
        } else {
          first start;
          then if direction != previousDirection {
            first start;
            then assign count := count + direction;
            then if count > 99 {
              first start;
              then assign count := 0;
            }
            then if count < 0 {
              first start;
              then assign count := 99;
            }
            then assign age := 0;
          } else {
            first start;
            then assign age := age + dt;
            then if age >= intervalMs {
              first start;
              then assign count := count + direction;
              then if count > 99 {
                first start;
                then assign count := 0;
              }
              then if count < 0 {
                first start;
                then assign count := 99;
              }
              then assign age := age - intervalMs;
            }
          }
        }
        then assign previousDirection := direction;
      }
    }
    out attribute displayNumber : Integer = count;
  }
  part system : Controller;
}
