package GeneratedModel {
  private import ScalarValues::*;
  part def Controller {
    in attribute pressed : Boolean;
    in attribute adc : Integer;
    out attribute duty : Real = (if pressed ? adc * 255.0 / 1023.0 else 0.0);
  }
  part system : Controller;
}
