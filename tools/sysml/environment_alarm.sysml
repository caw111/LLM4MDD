package GeneratedModel {
  private import ScalarValues::*;
  part def Controller {
    in attribute distanceCm : Real;
    in attribute distanceValid : Boolean;
    in attribute temperatureC : Real;
    in attribute temperatureValid : Boolean;
    in attribute elapsed : Integer;
    attribute displayElapsed : Integer := 0;
    attribute showDistance : Boolean := false;
    attribute alarmElapsed : Integer := 0;
    attribute reportElapsed : Integer := 0;
    attribute hot : Boolean := false;
    attribute near : Boolean := false;
    attribute wasAlarm : Boolean := false;
    attribute previousHot : Boolean := false;
    attribute previousNear : Boolean := false;
    attribute sendReport : Boolean := false;
    perform action tick {
      first start;
      then assign displayElapsed := displayElapsed + elapsed;
      then if displayElapsed >= 2000 {
        first start;
        then assign displayElapsed := displayElapsed - 2000;
        then assign showDistance := not showDistance;
      }
      then assign hot := temperatureValid and temperatureC > 32.0;
      then assign near := distanceValid and distanceCm < 15.0;
      then assign sendReport := false;
      then if hot or near {
        first start;
        then if not wasAlarm {
          first start;
          then assign alarmElapsed := 0;
          then assign reportElapsed := 0;
          then assign sendReport := true;
        } else {
          first start;
          then assign alarmElapsed := alarmElapsed + elapsed;
          then assign reportElapsed := reportElapsed + elapsed;
        }
        then if alarmElapsed >= 1000 {
          first start;
          then assign alarmElapsed := alarmElapsed - 1000;
        }
        then if reportElapsed >= 1000 {
          first start;
          then assign reportElapsed := reportElapsed - 1000;
          then assign sendReport := true;
        }
        then if hot != previousHot or near != previousNear {
          first start;
          then assign sendReport := true;
        }
      } else {
        first start;
        then assign alarmElapsed := 0;
        then assign reportElapsed := 0;
      }
      then assign wasAlarm := hot or near;
      then assign previousHot := hot;
      then assign previousNear := near;
    }
    out attribute displayNumber : Integer = (if showDistance ? (if distanceValid ? RealFunctions::floor(distanceCm) else 10000) else (if temperatureValid ? RealFunctions::floor(temperatureC) else 10000));
    out attribute alarmHz : Integer = (if (hot or near) and alarmElapsed < 500 ? 1000 else 0);
    out attribute temperatureReportDue : Boolean = sendReport and hot;
    out attribute temperatureReason : Boolean = hot;
    out attribute distanceReportDue : Boolean = sendReport and near;
    out attribute distanceReason : Boolean = near;
  }
  part system : Controller;
}
