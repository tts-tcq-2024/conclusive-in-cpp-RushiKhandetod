#pragma once
#include <string>

class TypewiseAlert {
public:
    enum class CoolingType {
        PASSIVE_COOLING,
        HI_ACTIVE_COOLING,
        MED_ACTIVE_COOLING
    };

    enum class BreachType {
        NORMAL,
        TOO_LOW,
        TOO_HIGH
    };

    enum class AlertTarget {
        TO_CONTROLLER,
        TO_EMAIL
    };

    struct BatteryCharacter {
        CoolingType coolingMethod;
        std::string brand;
    };

    static BreachType evaluateTemperatureBreach(CoolingType coolingMethod, double temperature);
    static void monitorAndAlert(AlertTarget alertMode, const BatteryCharacter& batteryInfo, double temperature);
    static BreachType detectBreach(double tempValue, double minThreshold, double maxThreshold);

private:
    struct TemperatureThresholds {
        CoolingType coolingMethod;
        int lowerLimit;
        int upperLimit;
    };

    static TemperatureThresholds getThresholdsForCoolingType(CoolingType coolingMethod);
    static void notifyController(BreachType breachCondition);
    static void notifyViaEmail(BreachType breachCondition);

    // Static method to initialize the temperature thresholds
    static const TemperatureThresholds* retrieveTemperatureThresholds();
};
