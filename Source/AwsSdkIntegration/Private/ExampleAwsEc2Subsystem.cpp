// Copyright Out-of-the-Box Plugins 2018-2024. All Rights Reserved.

#include "ExampleAwsEc2Subsystem.h"

#include "AwsIntegrationSubsystem.h"

THIRD_PARTY_INCLUDES_START
#include "aws/ec2/EC2Client.h"
#include "aws/ec2/model/DescribeInstancesRequest.h"
#include "aws/ec2/model/Reservation.h"
THIRD_PARTY_INCLUDES_END

void UExampleAwsEc2Subsystem::PrintEc2Instances()
{
	const Aws::EC2::EC2Client ec2Client;
	const Aws::EC2::Model::DescribeInstancesRequest Request;

	Aws::EC2::Model::DescribeInstancesOutcome outcome = ec2Client.DescribeInstances(Request);
	if (outcome.IsSuccess())
	{
		const Aws::Vector<Aws::EC2::Model::Reservation>& reservations = outcome.GetResult().GetReservations();

		for (const Aws::EC2::Model::Reservation& reservation : reservations)
		{
			const Aws::Vector<Aws::EC2::Model::Instance>& instances = reservation.GetInstances();
			for (const Aws::EC2::Model::Instance& instance : instances)
			{
				FString InstanceInfo;

				for (const Aws::EC2::Model::Tag& Tag : instance.GetTags())
				{
					const FString TagKey = UTF8_TO_TCHAR(Tag.GetKey().c_str());
					const FString TagValue = UTF8_TO_TCHAR(Tag.GetValue().c_str());
					InstanceInfo.Appendf(TEXT("%s:%s, "), *TagKey, *TagValue);
				}

				UE_LOG(LogTemp, Warning, TEXT("%s"), *InstanceInfo);
			}
		}
	}
}

void UExampleAwsEc2Subsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// Important: this ensures the AwsIntegrationSubsystem::Initialized is completed before moving further down
	Collection.InitializeDependency<UAwsIntegrationSubsystem>();

	PrintEc2Instances();
}

void UExampleAwsEc2Subsystem::Deinitialize()
{
}