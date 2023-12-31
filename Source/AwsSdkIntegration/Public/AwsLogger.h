// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

#pragma once

THIRD_PARTY_INCLUDES_START
#include <aws/core/utils/logging/FormattedLogSystem.h>
THIRD_PARTY_INCLUDES_END

/**
 * Forwards the output logs from the aws-cpp-sdk into Unreal Engine based on their verbosity
 */
class FUnrealLogger final : public Aws::Utils::Logging::LogSystemInterface
{
	// Begin LogSystemInterface
	virtual Aws::Utils::Logging::LogLevel GetLogLevel() const override;
	virtual void Log(Aws::Utils::Logging::LogLevel logLevel, const char* tag, const char* formatStr, ...) override;
	virtual void LogStream(Aws::Utils::Logging::LogLevel logLevel, const char* tag, const Aws::OStringStream& messageStream) override;
	virtual void Flush() override;
	// End LogSystemInterface

	/**
	 * Logs a formatted message & tag into Unreal system
	 */
	void LogInternalMessage(Aws::Utils::Logging::LogLevel Verbosity, const FString& Tag, const FString& Message);
};
