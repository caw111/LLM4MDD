package ADCDisplay {
    private import ScalarValues::*;
    part def Controller {
        in attribute adc : Integer;
        out attribute displayValue : Integer = RealFunctions::floor(adc * 9999.0 / 1023.0);
    }
    part system : Controller;
}
