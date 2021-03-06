// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class ILiveLinkClient;
class IModularFeature;

/** References the live link client */
struct LIVELINK_API FLiveLinkClientReference
{
public:
	ILiveLinkClient* GetClient() const;
};