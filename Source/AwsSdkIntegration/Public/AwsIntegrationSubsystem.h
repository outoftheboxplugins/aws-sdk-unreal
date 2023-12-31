// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

THIRD_PARTY_INCLUDES_START
#include "aws/core/Aws.h"
THIRD_PARTY_INCLUDES_END

#include "AwsIntegrationSubsystem.generated.h"

/**
 * Handles aws-cpp-sdk initialization and shutdown
 */
UCLASS()
class AWSSDKINTEGRATION_API UAwsIntegrationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	// Begin UGameInstanceSubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End UGameInstanceSubsystem

	Aws::SDKOptions Options;
	std::shared_ptr<Aws::Utils::Logging::LogSystemInterface> AwsLogger;
	std::shared_ptr<Aws::Utils::Memory::MemorySystemInterface> AwsMemoryManager;
};
