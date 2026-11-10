package GeneratedModel {
  private import ScalarValues::*;
  part def Controller {
    in attribute distanceCm : Real;
    in attribute distanceValid : Boolean;
    out attribute distanceDisplay : Integer = (if distanceValid ? RealFunctions::floor(distanceCm) else 10000);
    out attribute nearby : Boolean = distanceValid and distanceCm < 10.0;
  }
  part system : Controller;
}
