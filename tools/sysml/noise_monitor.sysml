package NoiseMonitorExample {
    private import ScalarValues::*;
    part def NoiseMonitor {
        in attribute mic : Real;
        in attribute threshold : Real;
        out attribute level : Real = mic * mic;
        out attribute alarm : Boolean = level > threshold;
    }
    part system : NoiseMonitor;
}
