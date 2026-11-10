package MixedEnvironment {
 private import ScalarValues::*;
 part def Controller {
  in attribute temperatureC : Real;
  in attribute humidityPercent : Real;
  in attribute environmentValid : Boolean;
  in attribute distanceCm : Real;
  in attribute distanceValid : Boolean;
  in attribute elapsedMilliseconds : Integer;
  attribute accumulated : Integer := 0;
  attribute reportPulse : Boolean := false;
  perform action tick {
   first start;
   then assign accumulated := accumulated + elapsedMilliseconds;
   then assign reportPulse := accumulated >= 5000;
   then if reportPulse { assign accumulated := accumulated - 5000; }
  }
  out attribute measuredTemperature : Real = temperatureC;
  out attribute measuredHumidity : Real = humidityPercent;
  out attribute reportDue : Boolean = reportPulse and environmentValid;
  out attribute highTemperature : Boolean = (environmentValid and temperatureC > 32.0) or (distanceValid and distanceCm < 15.0);
 }
 part system : Controller;
}
