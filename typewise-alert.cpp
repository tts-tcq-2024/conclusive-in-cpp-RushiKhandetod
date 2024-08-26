#include "typewise-alert.h"
#include <stdio.h>

// Initialize the static array of TemperatureThresholds inside the class
const TypewiseAlert::TemperatureThresholds* TypewiseAlert::retrieveTemperatureThresholds() {
    static const TemperatureThresholds tempThresholds[] = {
        {CoolingType::PASSIVE_COOLING, 0, 35},
        {CoolingType::HI_ACTIVE_COOLING, 0, 45},
        {CoolingType::MED_ACTIVE_COOLING, 0, 40},
    };
    return tempThresholds;
}

TypewiseAlert::TemperatureThresholds TypewiseAlert::getThresholdsForCoolingType(CoolingType coolingMethod) {
    const TemperatureThresholds* thresholds = retrieveTemperatureThresholds();
    for (int index = 0; index < 3; ++index) {
        if (thresholds[index].coolingType == coolingMethod) {
            return thresholds[index];
        }
    }
    // Default case: return 0, 0 thresholds if cooling type is unknown
    return {coolingMethod, 0, 0};
}

TypewiseAlert::BreachType TypewiseAlert::detectBreach(double tempValue, double minThreshold, double maxThreshold) {
    if (tempValue < minThreshold) {
        return BreachType::TOO_LOW;
    }
    if (tempValue > maxThreshold) {
        return BreachType::TOO_HIGH;
    }
    return BreachType::NORMAL;
}

TypewiseAlert::BreachType TypewiseAlert::evaluateTemperatureBreach(CoolingType coolingMethod, double temperature) {
    TemperatureThresholds thresholds = getThresholdsForCoolingType(coolingMethod);
    return detectBreach(temperature, thresholds.lowerLimit, thresholds.upperLimit);
}

void TypewiseAlert::notifyController(BreachType breachCondition) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, static_cast<int>(breachCondition));
}

void TypewiseAlert::notifyViaEmail(BreachType breachCondition) {
    const char* emailRecipient = "a.b@c.com";
    if (breachCondition != BreachType::NORMAL) {
        printf("To: %s\n", emailRecipient);
        printf("Hi, the temperature is %s\n", breachCondition == BreachType::TOO_LOW ? "too low" : "too high");
    }
}

void TypewiseAlert::monitorAndAlert(AlertTarget alertMode, const BatteryCharacter& batteryInfo, double temperature) {
    BreachType breachCondition = evaluateTemperatureBreach(batteryInfo.coolingType, temperature);
    if (alertMode == AlertTarget::TO_CONTROLLER) {
        notifyController(breachCondition);
    } else {
        notifyViaEmail(breachCondition);
    }
}
