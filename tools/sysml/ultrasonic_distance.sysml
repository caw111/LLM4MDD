package GeneratedModel {
  private import ScalarValues::*;
  part def Controller {
    in attribute distanceCm : Real;
    in attribute distanceValid : Boolean;
    out attribute distanceDisplay : Integer = (if distanceValid ? RealFunctions::floor(distanceCm) else 10000);
  }
  part system : Controller;
}
