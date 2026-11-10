package EnvironmentReport {
    private import ScalarValues::*;
    part def Controller {
        in attribute temperatureC : Real;
        in attribute humidityPercent : Real;
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
        out attribute reportDue : Boolean = reportPulse;
        out attribute highTemperature : Boolean = temperatureC > 30.0;
    }
    part system : Controller;
}
