// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

#include "AwsHttpClient.h"

std::shared_ptr<Aws::Http::HttpClient> FUnrealHttpClient::CreateHttpClient(const Aws::Client::ClientConfiguration& clientConfiguration) const
{
	// TODO: Requires implementation
	return {};
}

std::shared_ptr<Aws::Http::HttpRequest> FUnrealHttpClient::CreateHttpRequest(const Aws::String& uri, Aws::Http::HttpMethod method, const Aws::IOStreamFactory& streamFactory) const
{
	// TODO: Requires implementation
	return {};
}

std::shared_ptr<Aws::Http::HttpRequest> FUnrealHttpClient::CreateHttpRequest(const Aws::Http::URI& uri, Aws::Http::HttpMethod method, const Aws::IOStreamFactory& streamFactory) const
{
	// TODO: Requires implementation
	return {};
}