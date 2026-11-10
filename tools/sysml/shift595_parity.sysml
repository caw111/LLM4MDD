package GeneratedModel {
  private import ScalarValues::*;
  part def Controller {
    in attribute levelA : Real;
    in attribute levelB : Real;
    in attribute numberAdc : Real;
    attribute shown : Integer = RealFunctions::floor(numberAdc * 99.0 / 1023.0);
    attribute isEven : Boolean = shown == 2 * RealFunctions::floor(shown / 2.0);
    out attribute displayNumber : Integer = shown;
    out attribute brightnessA : Real = (if isEven ? levelA * 255.0 / 1023.0 else 0.0);
    out attribute brightnessB : Real = (if isEven ? 0.0 else levelB * 255.0 / 1023.0);
  }
  part system : Controller;
}
