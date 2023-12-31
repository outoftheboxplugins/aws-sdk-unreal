// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

#include "AwsIntegrationSubsystem.h"

#include "AwsLogger.h"
#include "AwsMemoryManager.h"

void UAwsIntegrationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// TODO: We should have some settings to disable the automatic init if required

	AwsLogger = std::make_shared<FUnrealLogger>();
	AwsMemoryManager = std::make_shared<FUnrealMemoryManager>();

	Options.memoryManagementOptions.memoryManager = AwsMemoryManager.get();
	Options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Trace;
	Options.loggingOptions.logger_create_fn = [this]()
	{
		return AwsLogger;
	};

	// TODO: Uncomment this when the HTTP client is implemented
	/*
	Options.httpOptions.httpClientFactory_create_fn = []()
	{
		return Aws::MakeShared<FUnrealHttpClient>();
	};
	*/

	Aws::InitAPI(Options);
}

void UAwsIntegrationSubsystem::Deinitialize()
{
	Aws::ShutdownAPI(Options);

	AwsLogger.reset();
	AwsMemoryManager.reset();
}