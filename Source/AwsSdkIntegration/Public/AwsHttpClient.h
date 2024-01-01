// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

#pragma once

THIRD_PARTY_INCLUDES_START
#include "aws/core/http/HttpClientFactory.h"
THIRD_PARTY_INCLUDES_END

/**
 * Allows the aws-sdk-cpp to use Unreal's HTTP module to make API requests.
 * This is important on platforms like Android where standard curl requests have SSL issues or MacOS where native requests are used instead of curl
 */
class FUnrealHttpClient : public Aws::Http::HttpClientFactory
{
	// Begin HttpClientFactory
	virtual std::shared_ptr<Aws::Http::HttpClient> CreateHttpClient(const Aws::Client::ClientConfiguration& clientConfiguration) const override;
	virtual std::shared_ptr<Aws::Http::HttpRequest> CreateHttpRequest(const Aws::String& uri, Aws::Http::HttpMethod method, const Aws::IOStreamFactory& streamFactory) const override;
	virtual std::shared_ptr<Aws::Http::HttpRequest> CreateHttpRequest(const Aws::Http::URI& uri, Aws::Http::HttpMethod method, const Aws::IOStreamFactory& streamFactory) const override;
	// End HttpClientFactory
};
