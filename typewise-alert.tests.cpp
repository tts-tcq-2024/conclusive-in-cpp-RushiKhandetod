#include "typewise-alert.h"
#include <gtest/gtest.h>

// Test to check the detectBreach function with different scenarios
TEST(TypewiseAlertTest, DetectBreach) {
    EXPECT_EQ(TypewiseAlert::detectBreach(25, 20, 30), TypewiseAlert::BreachType::NORMAL);
    EXPECT_EQ(TypewiseAlert::detectBreach(15, 20, 30), TypewiseAlert::BreachType::TOO_LOW);
    EXPECT_EQ(TypewiseAlert::detectBreach(35, 20, 30), TypewiseAlert::BreachType::TOO_HIGH);
}

// Test to verify the evaluateTemperatureBreach function for different cooling methods
TEST(TypewiseAlertTest, EvaluateTemperatureBreach) {
    EXPECT_EQ(TypewiseAlert::evaluateTemperatureBreach(TypewiseAlert::CoolingType::PASSIVE_COOLING, 30), TypewiseAlert::BreachType::NORMAL);
    EXPECT_EQ(TypewiseAlert::evaluateTemperatureBreach(TypewiseAlert::CoolingType::PASSIVE_COOLING, 40), TypewiseAlert::BreachType::TOO_HIGH);
    EXPECT_EQ(TypewiseAlert::evaluateTemperatureBreach(TypewiseAlert::CoolingType::HI_ACTIVE_COOLING, 50), TypewiseAlert::BreachType::TOO_HIGH);
    EXPECT_EQ(TypewiseAlert::evaluateTemperatureBreach(TypewiseAlert::CoolingType::MED_ACTIVE_COOLING, -5), TypewiseAlert::BreachType::TOO_LOW);
}

// Mocking a test for sending alerts to the controller
TEST(TypewiseAlertTest, MonitorAndAlertToController) {
    // Redirect stdout to a stringstream to check output
    testing::internal::CaptureStdout();
    
    TypewiseAlert::BatteryCharacter batteryInfo = { TypewiseAlert::CoolingType::PASSIVE_COOLING, "BrandA" };
    TypewiseAlert::monitorAndAlert(TypewiseAlert::AlertTarget::TO_CONTROLLER, batteryInfo, 50);
    
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");  // TOO_HIGH is expected to be 2
}

// Mocking a test for sending alerts to email
TEST(TypewiseAlertTest, MonitorAndAlertToEmail) {
    // Redirect stdout to a stringstream to check output
    testing::internal::CaptureStdout();
    
    TypewiseAlert::BatteryCharacter batteryInfo = { TypewiseAlert::CoolingType::PASSIVE_COOLING, "BrandA" };
    TypewiseAlert::monitorAndAlert(TypewiseAlert::AlertTarget::TO_EMAIL, batteryInfo, 50);
    
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}

// Test to ensure normal temperatures do not trigger alerts
TEST(TypewiseAlertTest, NoAlertForNormalTemperature) {
    // Redirect stdout to a stringstream to check output
    testing::internal::CaptureStdout();
    
    TypewiseAlert::BatteryCharacter batteryInfo = { TypewiseAlert::CoolingType::HI_ACTIVE_COOLING, "BrandB" };
    TypewiseAlert::monitorAndAlert(TypewiseAlert::AlertTarget::TO_EMAIL, batteryInfo, 40);
    
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");  // Expect no output since temperature is normal
}
