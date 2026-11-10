"""Accept GPIO-only interrupt brackets; preserve explicit unsupported outcomes."""
import unittest
from tests import shift595_behaviors_unittest as helpers


class InterruptOracleTests(unittest.TestCase):
    check_cases=helpers.ShiftBehaviorTests.check_cases

    def test_gpio_critical_sections_preserve_display_and_reject_wrong_segments(self):
        code=helpers.REPEAT.replace('void display(int n){','void display(int n){noInterrupts();')
        code=code.replace('digitalWrite(A1,HIGH);}', 'digitalWrite(A1,HIGH);delayMicroseconds(34);interrupts();}')
        self.check_cases(179,[(code,'passed'),
                             (code.replace('segments[n%10]','segments[(n+1)%10]'),'failed')])

    def test_interrupt_dependent_or_unbounded_execution_remains_unverified(self):
        code=helpers.REPEAT.replace('void display(int n){','void display(int n){noInterrupts();')
        code=code.replace('digitalWrite(A1,HIGH);}', 'digitalWrite(A1,HIGH);interrupts();}')
        variants=[code.replace('interrupts();}', 'delayMicroseconds(101);interrupts();}'),
                  code.replace('interrupts();}', 'millis();interrupts();}'),
                  code.replace('interrupts();}', 'micros();interrupts();}'),
                  code.replace('interrupts();}', 'delay(1);interrupts();}'),
                  code.replace('interrupts();}', 'for(int i=0;i<129;++i)digitalWrite(A0,LOW);interrupts();}'),
                  '#include <Arduino.h>\nvoid setup(){noInterrupts();} void loop(){}']
        self.check_cases(179,[(v,'oracle_runtime_unverified') for v in variants])


if __name__=='__main__':unittest.main()
