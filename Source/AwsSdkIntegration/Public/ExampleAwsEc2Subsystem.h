// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

#include "ExampleAwsEc2Subsystem.generated.h"

/**
 * Example implementation of the EC2 module of the aws-sdk-cpp exposed to blueprints
 */
UCLASS()
class AWSSDKINTEGRATION_API UExampleAwsEc2Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Example function which prints all the EC2 instances we have access to
	 */
	UFUNCTION(BlueprintCallable)
	void PrintEc2Instances();

private:
	// Begin UGameInstanceSubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End UGameInstanceSubsystem
};
